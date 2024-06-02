#include "QFramelessHelper.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qdebug.h"
#include "QDrawShadowHelper.h"
#include <QPainter>
#include <QScreen>

#ifdef Q_OS_WIN

#include <QApplication>
#include <QDesktopWidget>
#include <QWinEventNotifier>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <windows.h>
#include <QEvent>
#include <dwmapi.h>

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "dwmapi.lib")
#endif

#define TIMEMS qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))


QFramelessHelper::~QFramelessHelper()
{
	if (!m_widget.isNull())
	{
		m_widget->deleteLater();
		m_widget = nullptr;
	}
	
	if (!m_titleBar.isNull())
	{
		m_titleBar->deleteLater();
		m_titleBar = nullptr;
	}
	
	if (!m_drawShadow.isNull())
	{
		m_drawShadow->deleteLater();
		m_drawShadow = nullptr;
	}
};

QFramelessHelper::QFramelessHelper(QWidget *w,
                                   bool resizeEnable,
                                   bool shadowBorder,
                                   bool winNativeEvent,
                                   QObject *parent)
		: QObject(parent),
		  m_widget(w),
		  m_resizeEnable(resizeEnable),
		  m_shadowBorder(shadowBorder),
		  m_winNativeEvent(winNativeEvent),
		  m_borderResizeEnable(resizeEnable)
{
	m_padding = 8;
	m_moveEnable = true;
#ifdef Q_OS_WIN
	if (m_winNativeEvent)
	{
		QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
		QApplication::setAttribute(Qt::AA_NativeWindows, false);
	}
#else
	m_winNativeEvent = false;
#endif
	m_drawShadow = 0;
	
	m_mousePressed = false;
	m_mouseGlobalPoint = QPoint(0, 0);
	m_mouseRect = QRect(0, 0, 0, 0);
	
	for (int i = 0; i < 8; ++i)
	{
		m_pressedArea << false;
		m_pressedRect << QRect(0, 0, 0, 0);
	}
	
	m_isMin = false;
	m_flags = m_widget->windowFlags();
	m_titleBar = 0;
	
	//设置无边框属性，且保留任务栏图标点击切换最小化
	// if (m_widget->inherits("QMainWindow"))
	// {
	// 	qDebug() << "QMainWindow: " << m_widget->windowFlags();
	// }
	// else if (m_widget->inherits("QDialog"))
	// {
	// 	qDebug() << "QDialog: " << m_widget->windowFlags();
	// }
	// else if (m_widget->inherits("QWidget"))
	// {
	// 	qDebug() << "QWidget: " << m_widget->windowFlags();
	// }

#ifdef Q_OS_WIN
	m_widget->setWindowFlags((m_widget->windowFlags())
	                         | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
#else
	m_widget->setWindowFlags ((m_widget->windowFlags() & (~Qt::WindowMinMaxButtonsHint) & (~Qt::Dialog))
											| Qt::FramelessWindowHint | Qt::Window);

	qDebug() << "windowFlags: " << m_widget->windowFlags();
#endif
	
	//安装事件过滤器识别拖动
	m_widget->installEventFilter(this);
	
	//设置属性产生win窗体效果,移动到边缘半屏或者最大化等
	//设置以后会产生标题栏需要在下面拦截消息重新去掉
#ifdef Q_OS_WIN
	if (m_winNativeEvent)
	{
		m_widget->setAttribute(Qt::WA_DontCreateNativeAncestors);
		HWND hwnd = (HWND) m_widget->winId();
		DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
		if (resizeEnable)
		{
			::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
		}
		else
		{
			::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION);
		}
		if (m_shadowBorder)
		{
			//we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
			const MARGINS shadow = {1, 1, 1, 1};
			typedef HRESULT (WINAPI *DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS *pMarInset);
			HMODULE module = LoadLibraryW(L"dwmapi.dll");
			if (module)
			{
				DwmExtendFrameIntoClientAreaPtr dwm_extendframe_into_client_area_;
				dwm_extendframe_into_client_area_ = reinterpret_cast<DwmExtendFrameIntoClientAreaPtr>(GetProcAddress(
						module, "DwmExtendFrameIntoClientArea"));
				if (dwm_extendframe_into_client_area_)
				{
					dwm_extendframe_into_client_area_(HWND(m_widget->winId()), &shadow);
				}
			}
		}
	}
#endif
	
	if (m_winNativeEvent == false)
	{
		m_widget->setAttribute(Qt::WA_Hover, true);
		if (m_shadowBorder)
		{
			m_drawShadow = new QDrawShadowHelper(m_widget, 10, this);
			setPadding(m_padding + 5);
		}
	}
}

void QFramelessHelper::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter painter(m_widget);
	painter.save();
	if (m_drawShadow)
	{
		m_drawShadow->paint(&painter);
	}
	painter.restore();
}

void QFramelessHelper::doShowEvent(QEvent *event)
{
	Q_UNUSED(event)
	//解决有时候窗体重新显示的时候假死不刷新的BUG
	m_widget->setAttribute(Qt::WA_Mapped);
	updateDrawShadowState();
}

void QFramelessHelper::updateDrawShadowState()
{
	if (m_drawShadow)
	{
		if (m_widget->windowState() == Qt::WindowMaximized ||
		    m_widget->windowState() == Qt::WindowFullScreen)
		{
			m_drawShadow->hide();
		}
		else
		{
			m_drawShadow->show();
		}
	}
}

void QFramelessHelper::doWindowStateChange(QEvent *event)
{
	Q_UNUSED(event)
	//非最大化才能移动和拖动大小
	if (m_widget->windowState() == Qt::WindowNoState)
	{
		m_moveEnable = true;
		m_borderResizeEnable = true;
	}
	else
	{
		m_moveEnable = false;
		m_borderResizeEnable = false;
	}
	
	updateDrawShadowState();
	
	// 发出最大化最小化等改变事件,以便界面上更改对应的信息比如右上角图标和文字
	emit maximizedChanged(!m_moveEnable);
	
	//解决mac系统上无边框最小化失效的BUG
#ifdef Q_OS_MACOS
	if (m_widget->windowState() & Qt::WindowMinimized) {
		m_isMin = true;
	} else {
		if (m_isMin) {
			//设置无边框属性
			m_widget->setWindowFlags(flags | Qt::FramelessWindowHint);
			m_widget->setVisible(true);
			m_isMin = false;
		}
	}
#endif
}

void QFramelessHelper::doResizeEvent(QEvent *event)
{
	//非win系统的无边框拉伸,win系统上已经采用了nativeEvent来处理拉伸
	//为何不统一用计算的方式因为在win上用这个方式往左拉伸会发抖的
#ifdef Q_OS_WIN
	if (m_winNativeEvent)
	{
		return;
	}
#endif
	if (event->type() == QEvent::Resize)
	{
		QResizeEvent *resizeEvent = (QResizeEvent *) event;
		qDebug() << resizeEvent;
		//重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
		int width = m_widget->width();
		int height = m_widget->height();
		
		//左侧描点区域
		m_pressedRect[0] = QRect(0, m_padding, m_padding, height - m_padding * 2);
		//右侧描点区域
		m_pressedRect[1] = QRect(width - m_padding, m_padding, m_padding, height - m_padding * 2);
		//上侧描点区域
		m_pressedRect[2] = QRect(m_padding, 0, width - m_padding * 2, m_padding);
		//下侧描点区域
		m_pressedRect[3] = QRect(m_padding, height - m_padding, width - m_padding * 2, m_padding);
		
		//左上角描点区域
		m_pressedRect[4] = QRect(0, 0, m_padding, m_padding);
		//右上角描点区域
		m_pressedRect[5] = QRect(width - m_padding, 0, m_padding, m_padding);
		//左下角描点区域
		m_pressedRect[6] = QRect(0, height - m_padding, m_padding, m_padding);
		//右下角描点区域
		m_pressedRect[7] = QRect(width - m_padding, height - m_padding, m_padding, m_padding);
	}
	else if (event->type() == QEvent::HoverMove)
	{
		//设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
		QHoverEvent *hoverEvent = (QHoverEvent *) event;
		QPoint point = hoverEvent->pos();
		if (m_resizeEnable && m_borderResizeEnable)
		{
			if (m_pressedRect.at(0).contains(point))
			{
				m_widget->setCursor(Qt::SizeHorCursor);
			}
			else if (m_pressedRect.at(1).contains(point))
			{
				m_widget->setCursor(Qt::SizeHorCursor);
			}
			else if (m_pressedRect.at(2).contains(point))
			{
				m_widget->setCursor(Qt::SizeVerCursor);
			}
			else if (m_pressedRect.at(3).contains(point))
			{
				m_widget->setCursor(Qt::SizeVerCursor);
			}
			else if (m_pressedRect.at(4).contains(point))
			{
				m_widget->setCursor(Qt::SizeFDiagCursor);
			}
			else if (m_pressedRect.at(5).contains(point))
			{
				m_widget->setCursor(Qt::SizeBDiagCursor);
			}
			else if (m_pressedRect.at(6).contains(point))
			{
				m_widget->setCursor(Qt::SizeBDiagCursor);
			}
			else if (m_pressedRect.at(7).contains(point))
			{
				m_widget->setCursor(Qt::SizeFDiagCursor);
			}
			else
			{
				m_widget->setCursor(Qt::ArrowCursor);
			}
		}
	}
	else if (event->type() == QEvent::MouseButtonPress)
	{
		//记住鼠标按下的坐标+窗体区域
		QMouseEvent *mouseEvent = (QMouseEvent *) event;
		QPoint point = mouseEvent->pos();
		QPoint globalPoint = mouseEvent->globalPos();
		
		m_mouseGlobalPoint = globalPoint;
		m_mouseRect = m_widget->frameGeometry();
		
		//判断按下的手柄的区域位置
		if (m_pressedRect.at(0).contains(point))
		{
			m_pressedArea[0] = true;
		}
		else if (m_pressedRect.at(1).contains(point))
		{
			m_pressedArea[1] = true;
		}
		else if (m_pressedRect.at(2).contains(point))
		{
			m_pressedArea[2] = true;
		}
		else if (m_pressedRect.at(3).contains(point))
		{
			m_pressedArea[3] = true;
		}
		else if (m_pressedRect.at(4).contains(point))
		{
			m_pressedArea[4] = true;
		}
		else if (m_pressedRect.at(5).contains(point))
		{
			m_pressedArea[5] = true;
		}
		else if (m_pressedRect.at(6).contains(point))
		{
			m_pressedArea[6] = true;
		}
		else if (m_pressedRect.at(7).contains(point))
		{
			m_pressedArea[7] = true;
		}
		else
		{
			//点击标题栏，才可以移动窗体，如果希望点击窗体即可移动，则去掉下面判断条件
			QRect titleRect = m_titleBar->rect();
			titleRect = QRect(m_titleBar->mapTo(m_widget, titleRect.topLeft()), titleRect.size());
			if (titleRect.contains(point))
			{
				m_mousePressed = true;
			}
		}
	}
	else if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent *mouseEvent = (QMouseEvent *) event;
		QPoint point = mouseEvent->pos();
		QPoint globalPoint = mouseEvent->globalPos();
		
		//根据当前鼠标位置,计算XY轴移动了多少
		int offsetX = globalPoint.x() - m_mouseGlobalPoint.x();
		int offsetY = globalPoint.y() - m_mouseGlobalPoint.y();
//        qDebug() << "offsetX: " << offsetX;
//        qDebug() << "offsetY: " << offsetY;
		
		//根据按下处的位置判断是否是移动控件还是拉伸控件
		if (m_moveEnable && m_mousePressed)
		{
			m_widget->move(m_mouseRect.left() + offsetX, m_mouseRect.top() + offsetY);
		}
		
		//最大化时，点击标题栏或者顶部锚点，移动鼠标，则还原窗口
		if ((m_pressedArea[2] || m_mousePressed) &&
		    m_widget->windowState() == Qt::WindowMaximized)
		{
			QRect normalRect = m_widget->normalGeometry();
			float fx = (float) point.x() / (float) m_widget->rect().width();
			int offsetY = point.y();
#ifdef Q_OS_WIN
			switchMaximizedNormal();
			normalRect = m_widget->frameGeometry();
#else
			m_widget->setWindowState(Qt::WindowNoState);
			m_widget->setGeometry(normalRect);
#endif
			
			int x1 = (int) (fx * (float) normalRect.width());
			x1 = globalPoint.x() - x1;
			int y1 = 0;
			//点击顶部锚点时候，必须将y坐标向下移动一点，这样窗口还原后，鼠标不会继续触发顶部锚点
			if (m_pressedArea[2])
			{
				y1 -= 8;
				if (m_drawShadow)
				{
					y1 -= m_drawShadow->shadowSize();
				}
			}
			m_widget->move(x1, y1);
			//重新计算移动起点和大小
			m_mouseGlobalPoint = globalPoint;//m_widget->mapFromGlobal(point);
			m_mouseRect = m_widget->frameGeometry();
			m_mousePressed = true;
			m_pressedArea[2] = false;
		}
		
		if (m_resizeEnable && m_borderResizeEnable)
		{
			int rectX = m_mouseRect.x();
			int rectY = m_mouseRect.y();
			int rectW = m_mouseRect.width();
			int rectH = m_mouseRect.height();
			
			//左侧+右侧+上侧+下侧+左上侧+右上侧+左下侧+右下侧
			if (m_pressedArea.at(0))
			{
				//左侧
				int resizeW = rectW - offsetX;
				if (m_widget->minimumWidth() <= resizeW)
				{
					m_widget->setGeometry(rectX + offsetX, rectY, resizeW, rectH);
				}
			}
			else if (m_pressedArea.at(1))
			{
				//右侧
				m_widget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
			}
			else if (m_pressedArea.at(2))
			{
				//上侧
				int resizeH = rectH - offsetY;
				if (m_widget->minimumHeight() <= resizeH)
				{
					m_widget->setGeometry(rectX, rectY + offsetY, rectW, resizeH);
				}
			}
			else if (m_pressedArea.at(3))
			{
				//下侧
				m_widget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
			}
			else if (m_pressedArea.at(4))
			{
				//左上侧
				int resizeW = rectW - offsetX;
				int resizeH = rectH - offsetY;
				
				if (m_widget->minimumWidth() > resizeW)
				{
					offsetX = rectW - m_widget->minimumWidth();
					resizeW = m_widget->minimumWidth();
				}
				if (m_widget->minimumHeight() > resizeH)
				{
					offsetY = rectH - m_widget->minimumHeight();
					resizeH = m_widget->minimumHeight();
				}
				m_widget->setGeometry(rectX + offsetX, rectY + offsetY, resizeW, resizeH);
			}
			else if (m_pressedArea.at(5))
			{
				//右上侧
				int resizeW = rectW + offsetX;
				int resizeH = rectH - offsetY;
				
				if (m_widget->minimumWidth() > resizeW)
				{
					offsetX = rectW - m_widget->minimumWidth();
					resizeW = m_widget->minimumWidth();
				}
				if (m_widget->minimumHeight() > resizeH)
				{
					offsetY = rectH - m_widget->minimumHeight();
					resizeH = m_widget->minimumHeight();
				}
				m_widget->setGeometry(rectX, rectY + offsetY, resizeW, resizeH);
			}
			else if (m_pressedArea.at(6))
			{
				//左下侧
				int resizeW = rectW - offsetX;
				int resizeH = rectH + offsetY;
				if (m_widget->minimumWidth() > resizeW)
				{
					offsetX = rectW - m_widget->minimumWidth();
					resizeW = m_widget->minimumWidth();
				}
				if (m_widget->minimumHeight() > resizeH)
				{
					offsetY = rectH - m_widget->minimumHeight();
					resizeH = m_widget->minimumHeight();
				}
				m_widget->setGeometry(rectX + offsetX, rectY, resizeW, resizeH);
				
			}
			else if (m_pressedArea.at(7))
			{
				//右下侧
				int resizeW = rectW + offsetX;
				int resizeH = rectH + offsetY;
				if (m_widget->minimumWidth() > resizeW)
				{
					offsetX = rectW - m_widget->minimumWidth();
					resizeW = m_widget->minimumWidth();
				}
				if (m_widget->minimumHeight() > resizeH)
				{
					offsetY = rectH - m_widget->minimumHeight();
					resizeH = m_widget->minimumHeight();
				}
				m_widget->setGeometry(rectX, rectY, resizeW, resizeH);
			}
		}
		
	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		//恢复所有
		m_widget->setCursor(Qt::ArrowCursor);
		m_mousePressed = false;
		for (int i = 0; i < 8; ++i)
		{
			m_pressedArea[i] = false;
		}
	}
}

void QFramelessHelper::switchMaximizedNormal()
{
	if (isMaximized())
	{
		showNormal();
		emit maximizedChanged(false);
	}
	else
	{
		showMaximized();
		emit maximizedChanged(true);
	}
}

bool QFramelessHelper::eventFilter(QObject *watched, QEvent *event)
{
	
	if (watched == m_widget)
	{
		if (event->type() == QEvent::WindowStateChange)
		{
			doWindowStateChange(event);
		}
		else if (event->type() == QEvent::Show)
		{
			doShowEvent(event);
		}
		else
		{
			doResizeEvent(event);
		}
	}
	else if (watched == m_titleBar)
	{
		//双击标题栏发出双击信号给主界面
		//下面的 *result = HTCAPTION; 标志位也会自动识别双击标题栏
		if (m_resizeEnable)
		{
#ifdef Q_OS_WIN
			if (m_winNativeEvent == false)
#endif
			{
				if (event->type() == QEvent::MouseButtonDblClick)
				{
					switchMaximizedNormal();
				}
				else if (event->type() == QEvent::NonClientAreaMouseButtonDblClick)
				{
					switchMaximizedNormal();
				}
			}
		}
	}
	
	return false;
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
bool FramelessHelper::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else

void printWidgetChildren(QWidget *widget)
{
	const QObjectList &children = widget->children();
	for (QObject *child: children)
	{
		const QString &typeName = child->metaObject()->className();
		qDebug() << "Child type:" << typeName;
		
		// // 如果该子对象是一个 QWidget，则递归打印它的子对象
		// if (QWidget *childWidget = qobject_cast<QWidget *>(child))
		// {
		// 	printWidgetChildren(childWidget);
		// }
	}
}


bool QFramelessHelper::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
	if (eventType == "windows_generic_MSG")
	{
#ifdef Q_OS_WIN
		if (m_winNativeEvent == false)
		{
			return false;
		}
		MSG *msg = static_cast<MSG *>(message);
		msg->hwnd;
		
		// qDebug() << TIMEMS << "nativeEvent" << QString("0x%1").arg(msg->wParam, 0, 16)
		//          << QString("0x%1").arg(msg->message, 0, 16);
		
		HWND hWndTarget = msg->hwnd;
		if ((reinterpret_cast<intptr_t>(hWndTarget) == m_widget->winId()) ||
		    (reinterpret_cast<intptr_t>(hWndTarget) == m_titleBar->winId()))
		{
			switch (msg->message)
			{
				case WM_NCCALCSIZE:
				{
					NCCALCSIZE_PARAMS &params = *reinterpret_cast<NCCALCSIZE_PARAMS *>(msg->lParam);
					if (params.rgrc[0].top != 0)
					{
						params.rgrc[0].top -= 1;
					}
					
					//this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
					*result = WVR_REDRAW;
					return true;
					break;
				}
				case WM_NCACTIVATE:
				{
					*result = 1;
					return true;
				}
				case WM_NCHITTEST:
				{
					// 鼠标移动到非客户端区域事件
					
					// 计算鼠标对应的屏幕坐标
					QPoint m_Global_Point(LOWORD(msg->lParam), HIWORD(msg->lParam));
					QPoint m_Widget_Point = m_widget->mapFromGlobal(m_Global_Point);
					// 判断当前鼠标位置在哪个区域
					bool left = m_Widget_Point.x() < m_padding;
					bool right = m_Widget_Point.x() > m_widget->width() - m_padding;
					bool top = m_Widget_Point.y() < m_padding;
					bool bottom = m_Widget_Point.y() > m_widget->height() - m_padding;
					// 鼠标移动到四个角,这个消息是当鼠标移动或者有鼠标键按下时候发出的
					// *result = 0;
					if (m_resizeEnable)
					{
						if (top)
						{
							if (left)
							{
								// 左上角
								*result = HTTOPLEFT;
							}
							else if (right)
							{
								// 右上角
								*result = HTTOPRIGHT;
							}
							else
							{
								// 顶部边框
								*result = HTTOP;
							}
						}
						else if (bottom)
						{
							if (left)
							{
								// 左下角
								*result = HTBOTTOMLEFT;
							}
							else if (right)
							{
								// 右下角
								*result = HTBOTTOMRIGHT;
							}
							else
							{
								// 底部边框
								*result = HTBOTTOM;
							}
						}
						else
						{
							if (left)
							{
								// 左边框
								*result = HTLEFT;
							}
							else if (right)
							{
								// 右边框
								*result = HTRIGHT;
							}
							else
							{
							
							}
						}
					}
					
					//先处理掉拉伸
					if (0 != *result)
					{
						return true;
					}
					
					QPoint m_titleBar_Point = m_titleBar->mapFromGlobal(m_Global_Point);
					// 识别标题栏拖动产生半屏全屏效果
					if (m_titleBar != 0 && m_titleBar->rect().contains(m_titleBar_Point))
					{
						QPoint titleBarTopLeft = m_titleBar->mapTo(m_widget, QPoint(0, 0));
						QRect rcTitleBar = QRect(titleBarTopLeft, QSize(m_titleBar->width(), m_titleBar->height()));
						if (rcTitleBar.contains(m_Widget_Point))
						{
							QPoint posInTitleBar = m_titleBar->mapFrom(m_widget, m_Widget_Point);
							QWidget *child = m_titleBar->childAt(posInTitleBar);
							if ((child == 0) ||
							    ((!child->inherits("QAbstractButton")) &&
							     (!child->inherits("QMenuBar"))))
							{
								*result = HTCAPTION;
								return true;
							}
							
						}
					}
					else
					{
						return false;
					}
					*result = HTCLIENT;
					return true;
					break;
				}
				
				case WM_GETMINMAXINFO:
				{
					if (::IsZoomed(msg->hwnd))
					{
						RECT frame = {0, 0, 0, 0};
						AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
						frame.left = abs(frame.left);
						frame.top = abs(frame.bottom);
						m_widget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
					}
					else
					{
						m_widget->setContentsMargins(0, 0, 0, 0);
					}
					// return true;
					return false;
				}
				case PBT_APMSUSPEND:
				{
					// 系统休眠的时候自动最小化可以规避程序可能出现的问题
					m_widget->showMinimized();
					return true;
				}
				case WM_POWERBROADCAST:
				{
					// 系统休眠的时候自动最小化可以规避程序可能出现的问题
					m_widget->showMinimized();
					return true;
				}
				case PBT_APMRESUMEAUTOMATIC:
				{
					// 休眠唤醒后自动打开
					// this->showNormal();
					return true;
				}
				default:
				{
					// 若非以上消息，交由 QMainWindow 处理
					return QObject::eventFilter(this, reinterpret_cast<QEvent *>(message));
				}
			}
		}
		else
		{
			return QObject::eventFilter(this, reinterpret_cast<QEvent *>(message));
		}
#endif
	}
	
	else if (eventType == "NSEvent")
	{
#ifdef Q_OS_MACOS
#endif
	}
	else if (eventType == "xcb_generic_event_t")
	{
#ifdef Q_OS_LINUX
#endif
	}
	return false;
}

void QFramelessHelper::setPadding(int padding)
{
	this->m_padding = padding;
}

void QFramelessHelper::setMoveEnable(bool moveEnable)
{
	this->m_moveEnable = moveEnable;
}

void QFramelessHelper::setResizeEnable(bool resizeEnable)
{
	this->m_resizeEnable = resizeEnable;
	m_borderResizeEnable = resizeEnable;
}

void QFramelessHelper::setTitleBar(QWidget *titleBar)
{
	this->m_titleBar = titleBar;
	this->m_titleBar->installEventFilter(this);
}

void QFramelessHelper::showMinimized()
{
	if (m_widget)
	{
#ifdef Q_OS_MACOS
		m_widget->setWindowFlags(m_widget->windowFlags() & ~Qt::FramelessWindowHint);
#endif
		m_widget->showMinimized();
	}
}

bool QFramelessHelper::isMaximized()
{
	if (m_widget)
	{
		return m_widget->isMaximized();
	}
	return false;
}

void QFramelessHelper::showMaximized()
{
	if (m_widget)
	{
		m_widget->showMaximized();
	}
}

void QFramelessHelper::showNormal()
{
	if (m_widget)
	{
		m_widget->showNormal();
	}
}


