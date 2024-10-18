/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaReceiveWidget.cpp
 * @Description  : 
 */


#include "VersaReceiveWidget.h"
#include <QScrollBar>
#include <QTextCodec>
#include <QRegularExpression>
#include <QWheelEvent>
#include <QVBoxLayout>
#include <QString>
#include <QDebug>

#define minValue(a, b) ((a) < (b) ? (a) : (b))
#define maxValue(a, b) ((a) > (b) ? (a) : (b))
#define clampValue(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

VersaReceiveWidget::VersaReceiveWidget(QWidget *parent) :
		QWidget(parent),
		textEdit(parent),
		verticalScroll(Qt::Vertical, parent)
{
	textEdit.setContentsMargins(0, 0, 0, 0);
	verticalScroll.setContentsMargins(0, 0, 0, 0);
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->setSpacing(4);
	pLayout->setContentsMargins(0, 0, 0, 0);
	
	textEdit.setUndoRedoEnabled(false);
	textEdit.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	textEdit.setReadOnly(true);
	textEdit.setAcceptRichText(true);
	textEdit.clear();
	textEdit.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	pLayout->addWidget(&textEdit);
	pLayout->addWidget(&verticalScroll);
	
	verticalScroll.setMaximum(1024);
	verticalScroll.setFixedWidth(24);
	connect(&verticalScroll, &QScrollBar::actionTriggered, this, &VersaReceiveWidget::actionTriggered);
	
	this->setLayout(pLayout);
	
	timerBase.setInterval(75);
	timerBase.setTimerType(Qt::PreciseTimer);
	timerBase.stop();
	connect(&timerBase, &QTimer::timeout, this, &VersaReceiveWidget::timerout);
	timerBase.start();
}

VersaReceiveWidget::~VersaReceiveWidget()
{
	enableRefresh = false;
	timerBase.stop();
	pTextBuffer = nullptr;
}

void VersaReceiveWidget::timerout()
{
	if (enableRefresh == true)
	{
		// 如果需要移动到最后
		if (refreshMove == QTextCursor::End)
		{
			refreshText();
			textEdit.moveCursor(QTextCursor::End);
		}
		else if (refreshMove == QTextCursor::Start)
		{
			refreshText();
			textEdit.moveCursor(QTextCursor::Start);
		}
		else
		{
			refreshText();
		}
		// 加载完成
		refreshMove = QTextCursor::NoMove;
		enableRefresh = false;
	}
}


// 设置数据缓冲
void VersaReceiveWidget::setTextBuffer(QByteArray *buffer)
{
	pTextBuffer = buffer;
	refreshBuffer();
}

// 使能hex显示
void VersaReceiveWidget::setHexEnable(bool enable)
{
	enableHex = enable;
	enableRefresh = true;
}

// 有新的数据,实时更新到末尾
void VersaReceiveWidget::refreshBuffer()
{
	if (pTextBuffer != nullptr)
	{
		enableRefresh = true;
		refreshMove = QTextCursor::End;
		enableRefreshScroll = true;//更新滑块
		// 需要显示的数据长度
		indexStart = pTextBuffer->size() - indexMaxLength;
		indexLength = minValue(pTextBuffer->size(), indexMaxLength);
		indexStart = clampValue(indexStart, 0, pTextBuffer->size() - indexLength);
	}
}

void VersaReceiveWidget::refreshText()
{
	if (pTextBuffer != nullptr)
	{
		QRegularExpression regexRx("\\[(\\d{2}:\\d{2}:\\d{2}\\.\\d{3}\\.\\d{3})\\](Rx):"); // 匹配时间戳的正则表达式
		QRegularExpression regexTx("\\[(\\d{2}:\\d{2}:\\d{2}\\.\\d{3}\\.\\d{3})\\](Tx):");
		if (enableHex == true)
		{
			// 显示
			textEdit.setHtml(QString(pTextBuffer->mid(indexStart, indexLength).toHex())
					                 .toUpper()
					                 .replace(QRegularExpression("(..)"), "\\1 ")
					                 .replace(regexRx, "<font color='#e53935'>[\\1]\\2&lt;-:</font>")
					                 .replace(regexTx, "<font color='#43a047'>[\\1]\\2-&gt;:</font>")
					                 .replace("\n", "<br>"));
		}
		else
		{
			// 显示统一使用 UTF-8 编码格式
			textEdit.setHtml(QTextCodec::codecForName(textCode)->toUnicode(
							pTextBuffer->mid(indexStart, indexLength))
					                 .replace(QChar('\0'), QString("<font color='gray'>\\0</font>"))
					                 .replace(regexRx, "<font color='#e53935'>[\\1]\\2&lt;-:</font>")
					                 .replace(regexTx, "<font color='#43a047'>[\\1]\\2-&gt;:</font>")
					                 .replace("\n", "<br>"));
			
			
		}
		refreshScroll();
	}
}

void VersaReceiveWidget::resizeEvent(QResizeEvent *event)
{
	int textEditWidth = textEdit.viewport()->width();
	bytesPerLine = (textEditWidth / this->fontMetrics().averageCharWidth());
	QScrollBar *verticalScrollBar = textEdit.verticalScrollBar();
	verticalScrollBar->setPageStep(bytesPerLine);
	QWidget::resizeEvent(event);
}

void VersaReceiveWidget::wheelEvent(QWheelEvent *event)
{
	if (pTextBuffer == nullptr)
	{
		// 让事件继续传递
		event->accept();
		return;
	}
	int delta = event->angleDelta().y();
	// 获取滚动的角度
	// 根据滚动方向进行相应的处理
	if (textEdit.verticalScrollBar()->value() - delta >= 0)
	{
		textEdit.verticalScrollBar()->setValue(textEdit.verticalScrollBar()->value() - delta);
	}
	else
	{
		textEdit.verticalScrollBar()->setValue(0);
	}
	// 如果已经完成刷新
	if ((enableRefresh == false))
	{
		if (textEdit.verticalScrollBar()->value() == 0)
		{
			// 如果继续向上滑动
			if (delta > 0)
			{
				indexStart -= (delta / 40) * bytesPerLine;
				indexLength = minValue(pTextBuffer->size(), indexMaxLength);
				indexStart = clampValue(indexStart, 0, pTextBuffer->size() - indexLength);
				enableRefresh = true;
				refreshMove = QTextCursor::Start;
				enableRefreshScroll = true;
			}
		}
		else if (textEdit.verticalScrollBar()->value() == textEdit.verticalScrollBar()->maximum())
		{
			// 如果继续向下滚动
			if (delta < 0)
			{
				indexStart -= (delta / 40) * bytesPerLine;
				indexLength = minValue(pTextBuffer->size(), indexMaxLength);
				indexStart = clampValue(indexStart, 0, pTextBuffer->size() - indexLength);
				enableRefresh = true;
				refreshMove = QTextCursor::End;
				enableRefreshScroll = true;
			}
		}
	}
	// 让事件继续传递
	event->accept();
}

void VersaReceiveWidget::refreshScroll()
{
	if (enableRefreshScroll == true)
	{
		verticalScroll.setMaximum(pTextBuffer->size() - indexLength);
		verticalScroll.setValue(indexStart);
		verticalScroll.setPageStep(indexLength);
	}
}

void VersaReceiveWidget::setTextCodec(const QByteArray &codeText)
{
	enableRefresh = false;
	textCode = codeText;
	QTimer::singleShot(1, this, [=]()
	{
		enableRefresh = true;
	});
}

void VersaReceiveWidget::actionTriggered(int action)
{
	if (enableRefresh == false)
	{
		indexStart = verticalScroll.value();
		indexLength = minValue(pTextBuffer->size(), indexMaxLength);
		indexStart = clampValue(indexStart, 0, pTextBuffer->size() - indexLength);
		enableRefresh = true;
		enableRefreshScroll = false;
	}
}