#include "qtmaterialappbar.h"
#include "qtmaterialappbar_p.h"
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QPainter>
#include <QMouseEvent>
#include <QSize>
#include "lib/qtmaterialstyle.h"
#include "qtmaterialiconbutton.h"
#include <qtmaterialflatbutton.h>

/*!
 *  \class MATERIAL_EXPORT QtMaterialAppBarPrivate
 *  \internal
 */

/*!
 *  \internal
 */
QtMaterialAppBarPrivate::QtMaterialAppBarPrivate(QtMaterialAppBar *q)
		: q_ptr(q)
{
}

/*!
 *  \internal
 */
QtMaterialAppBarPrivate::~QtMaterialAppBarPrivate()
{
}

/*!
 *  \internal
 */
void QtMaterialAppBarPrivate::init()
{
	Q_Q(QtMaterialAppBar);
	useThemeColors = true;
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(11);
	effect->setColor(QColor(0, 0, 0, 50));
	effect->setOffset(0, 3);
	q->setGraphicsEffect(effect);
	// QHBoxLayout *layout = new QHBoxLayout;
	// q->setLayout(layout);
}

/*!
 *  \class MATERIAL_EXPORT QtMaterialAppBar
 */

QtMaterialAppBar::QtMaterialAppBar(int height, int icon_size, QWidget *parent)
		: QWidget(parent),
		  d_ptr(new QtMaterialAppBarPrivate(this))
{
	moveQWidget = parent;
	titleHeight = height;
	iconSize = icon_size;
	d_func()->init();
	
	this->setMaximumHeight(titleHeight);
	this->setMaximumHeight(titleHeight);
	this->setFixedHeight(titleHeight);
	
	QHBoxLayout *pQhBoxLayout = new QHBoxLayout(this);
	pQhBoxLayout->setSpacing(0);
	pQhBoxLayout->setContentsMargins(0, 0, 0, 0);
	
	{
		QtMaterialFlatButton *pMaterialFlatButton = new QtMaterialFlatButton(this);
		pMaterialFlatButton->setHaloVisible(false);
		pMaterialFlatButton->setRole(Material::Primary);
		// 叠加样式
		pMaterialFlatButton->setOverlayStyle(Material::TintedOverlay);
		// 波纹风格
		pMaterialFlatButton->setRippleStyle(Material::CenteredRipple);
		// 文本对齐
		pMaterialFlatButton->setTextAlignment(Qt::AlignCenter);
		// 背景模式
		pMaterialFlatButton->setBackgroundMode(Qt::OpaqueMode);
		// 设置大小
		pMaterialFlatButton->setMinimumSize(titleHeight, titleHeight);
		pMaterialFlatButton->setMaximumSize(titleHeight, titleHeight);
		pMaterialFlatButton->setFixedHeight(titleHeight);
		pMaterialFlatButton->setFixedWidth(titleHeight);
		// 边框样式
		pMaterialFlatButton->setCornerRadius(0);
		pMaterialFlatButton->setFixedRippleRadius(0);
		pMaterialFlatButton->setHasFixedRippleRadius(false);
		// Icon对齐
		pMaterialFlatButton->setIconPlacement(Material::RightIcon);
		// Icon大小
		pMaterialFlatButton->setIconSize(QSize(iconSize, iconSize));
		// 设置蒙版不透明度
		pMaterialFlatButton->setBaseOpacity(0.5);
		
		logoButton = pMaterialFlatButton;
		logoButton->setIcon(QtMaterialTheme::icon("://MingCute/contact/send_plane_line.svg"));
		logoButton->setObjectName("logoButton");
		logoButton->setBaseOpacity(0.5);
	}
	pQhBoxLayout->addWidget(logoButton);
	
	{
		textLabel = new QLabel("VersaAssistant");
		textLabel->setAttribute(Qt::WA_TranslucentBackground);
		textLabel->setForegroundRole(QPalette::Foreground);
		textLabel->setContentsMargins(0, 0, 0, 0);
		
		QPalette palette = textLabel->palette();
		palette.setColor(textLabel->foregroundRole(), Qt::white);
		textLabel->setPalette(palette);
		textLabel->setFont(QFont("Roboto", 12, QFont::Normal));
	}
	pQhBoxLayout->addWidget(textLabel);
	
	// 菜单栏
	menuBar = new QMenuBar(this);
	pQhBoxLayout->addWidget(menuBar);
	
	pQhBoxLayout->addStretch(1);
	for (uint8_t button_i = 0; button_i < 3; button_i++)
	{
		QtMaterialFlatButton *pMaterialFlatButton = new QtMaterialFlatButton(this);
		pMaterialFlatButton->setHaloVisible(false);
		pMaterialFlatButton->setRole(Material::Primary);
		// 叠加样式
		pMaterialFlatButton->setOverlayStyle(Material::TintedOverlay);
		// 波纹风格
		pMaterialFlatButton->setRippleStyle(Material::CenteredRipple);
		// 文本对齐
		pMaterialFlatButton->setTextAlignment(Qt::AlignCenter);
		// 背景模式
		pMaterialFlatButton->setBackgroundMode(Qt::OpaqueMode);
		// 设置大小
		pMaterialFlatButton->setMinimumSize((int) (titleHeight * 1.618), titleHeight);
		pMaterialFlatButton->setMaximumSize((int) (titleHeight * 1.618), titleHeight);
		pMaterialFlatButton->setFixedHeight(titleHeight);
		pMaterialFlatButton->setFixedWidth((int) (titleHeight * 1.618));
		// 边框样式
		pMaterialFlatButton->setCornerRadius(0);
		pMaterialFlatButton->setFixedRippleRadius(0);
		pMaterialFlatButton->setHasFixedRippleRadius(false);
		// Icon对齐
		pMaterialFlatButton->setIconPlacement(Material::RightIcon);
		// Icon大小
		pMaterialFlatButton->setIconSize(QSize(iconSize, iconSize));
		// 设置蒙版不透明度
		pMaterialFlatButton->setBaseOpacity(0.5);
		
		switch (button_i)
		{
			case 0 :
			{
				minToolButton = pMaterialFlatButton;
				minToolButton->setObjectName("minToolButton");
				minToolButton->setBaseOpacity(0.5);
				minToolButton->setIcon(
						QtMaterialTheme::icon(":/icons/icons/content/svg/production/ic_remove_24px.svg"));
				
				connect(minToolButton, &QtMaterialFlatButton::clicked, moveQWidget->window(), &QWidget::showMinimized);
				
				pQhBoxLayout->addWidget(minToolButton);
			}
				break;
			case 1:
			{
				maxToolButton = pMaterialFlatButton;
				maxToolButton->setObjectName("maxToolButton");
				maxToolButton->setBaseOpacity(0.5);
				maxToolButton->setProperty("MaxState", false);
				maxToolButton->setIcon(
						QtMaterialTheme::icon(
								":/icons/icons/toggle/svg/production/ic_check_box_outline_blank_24px.svg"));
				connect(maxToolButton, &QtMaterialIconButton::clicked, this, &QtMaterialAppBar::toggleMaxState);
				pQhBoxLayout->addWidget(maxToolButton);
			}
				break;
			case 2:
			{
				closeToolButton = pMaterialFlatButton;
				closeToolButton->setObjectName("closeToolButton");
				closeToolButton->setBaseOpacity(0.75);
				closeToolButton->setOverlayColor(QColor("#ff0000"));
				closeToolButton->setIcon(
						QtMaterialTheme::icon(":/icons/icons/navigation/svg/production/ic_close_24px.svg"));
				connect(closeToolButton, &QtMaterialIconButton::clicked, moveQWidget->window(), &QWidget::close);
			}
				break;
		}
		pQhBoxLayout->addWidget(pMaterialFlatButton);
	}
	this->setLayout(pQhBoxLayout);
}

QtMaterialAppBar::~QtMaterialAppBar()
{

}

void QtMaterialAppBar::toggleMaxState()
{
	if (moveQWidget == nullptr)
	{
		return;
	}
	if (moveQWidget->window()->isMaximized())
	{
		moveQWidget->window()->showNormal();
		maxToolButton->setIcon(
				QtMaterialTheme::icon(":/icons/icons/toggle/svg/production/ic_check_box_outline_blank_24px.svg"));
	}
	else
	{
		moveQWidget->window()->showMaximized();
		maxToolButton->setIcon(QtMaterialTheme::icon(":/icons/icons/image/svg/production/ic_filter_none_24px.svg"));
	}
}

void QtMaterialAppBar::mousePressEvent(QMouseEvent *event)
{
	if (moveQWidget == nullptr)
	{
		event->ignore();
		return;
	}
	if (event->buttons() & Qt::LeftButton)
	{
		if (event->pos().y() > 8)
		{
			mousePressed = true;
			topLeftPoint = moveQWidget->frameGeometry().topLeft();
			mousePoint = event->globalPos();
		}
	}
	event->ignore();
	return;
}

void QtMaterialAppBar::mouseMoveEvent(QMouseEvent *event)
{
	if (moveQWidget == nullptr)
	{
		event->ignore();
		return;
	}
	if (mousePressed == true)
	{
		if (moveQWidget->window()->isMaximized())
		{
			QRect normalSize(moveQWidget->window()->normalGeometry());
			QRect currentSize(moveQWidget->window()->frameGeometry());
			moveQWidget->window()->showNormal();
			maxToolButton->setIcon(
					QtMaterialTheme::icon(":/icons/icons/toggle/svg/production/ic_check_box_outline_blank_24px.svg"));
			QPoint movePoint((float) mousePoint.x() - (((float) mousePoint.x() - (float) topLeftPoint.x()) *
			                                           ((float) normalSize.width() / (float) currentSize.width())), 0);
			
			moveQWidget->move(movePoint + (event->globalPos() - mousePoint));
			topLeftPoint = movePoint + (event->globalPos() - mousePoint);
			mousePoint = event->globalPos();
		}
		else
		{
			moveQWidget->move(topLeftPoint + (event->globalPos() - mousePoint));
		}
	}
	event->ignore();
	return;
}

void QtMaterialAppBar::mouseReleaseEvent(QMouseEvent *event)
{
	if (moveQWidget == nullptr)
	{
		event->ignore();
		return;
	}
	if (!(event->buttons() & Qt::LeftButton))
	{
		mousePressed = false;
	}
	Q_UNUSED(event);
	event->ignore();
	return;
}

void QtMaterialAppBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	toggleMaxState();
}

QSize QtMaterialAppBar::sizeHint() const
{
	return QSize(-1, 64);
}

void QtMaterialAppBar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	
	QPainter painter(this);
	
	painter.fillRect(rect(), backgroundColor());
}

void QtMaterialAppBar::setUseThemeColors(bool value)
{
	Q_D(QtMaterialAppBar);
	
	if (d->useThemeColors == value)
	{
		return;
	}
	
	d->useThemeColors = value;
	update();
}

bool QtMaterialAppBar::useThemeColors() const
{
	Q_D(const QtMaterialAppBar);
	
	return d->useThemeColors;
}

void QtMaterialAppBar::setForegroundColor(const QColor &color)
{
	Q_D(QtMaterialAppBar);
	d->foregroundColor = color;
	if (d->useThemeColors == true)
	{
		d->useThemeColors = false;
	}
	update();
}

QColor QtMaterialAppBar::foregroundColor() const
{
	Q_D(const QtMaterialAppBar);
	if (d->useThemeColors || !d->foregroundColor.isValid())
	{
		return QtMaterialStyle::instance().themeColor("primary1");
	}
	else
	{
		return d->foregroundColor;
	}
}

void QtMaterialAppBar::setBackgroundColor(const QColor &color)
{
	Q_D(QtMaterialAppBar);
	
	d->backgroundColor = color;
	if (d->useThemeColors == true)
	{
		d->useThemeColors = false;
	}
	update();
}

QColor QtMaterialAppBar::backgroundColor() const
{
	Q_D(const QtMaterialAppBar);
	
	if (d->useThemeColors || !d->backgroundColor.isValid())
	{
		return QtMaterialStyle::instance().themeColor("primary1");
	}
	else
	{
		return d->backgroundColor;
	}
}
