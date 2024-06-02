#include "VersaAppBar.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <VersaConfig.h>
#include <QStyle>
#include <QRegularExpression>
#include <QApplication>
#include <QDebug>

VersaAppBar::VersaAppBar(int height, int icon_size, QWidget *parent)
		: QWidget(parent),
		  menuBar(new QMenuBar(this)),
		  titleHeight(height),
		  iconSize(icon_size)
{
	this->setAutoFillBackground(true);
	this->setMaximumHeight(titleHeight);
	this->setMaximumHeight(titleHeight);
	this->setFixedHeight(titleHeight);
	
	QHBoxLayout *pVersaAppBarLayout = new QHBoxLayout(this);
	pVersaAppBarLayout->setSpacing(0);
	pVersaAppBarLayout->setContentsMargins(0, 0, 0, 0);
	this->setContentsMargins(0, 0, 0, 0);
	
	{
		QPushButton *pMaterialQPushButton = new QPushButton(this);
		pMaterialQPushButton->setMinimumSize(titleHeight, titleHeight);
		pMaterialQPushButton->setMaximumSize(titleHeight, titleHeight);
		pMaterialQPushButton->setFixedHeight(titleHeight);
		pMaterialQPushButton->setFixedWidth(titleHeight);
		pMaterialQPushButton->setIconSize(QSize(iconSize, iconSize));
		logoPushButton = pMaterialQPushButton;
		logoPushButton->setObjectName("logoButton");
		
		QFile fileSvgIcon(qApp->applicationDirPath() + "/VersaAssistant.svg");
		if (fileSvgIcon.exists())
		{
			logoPushButton->setIcon(QIcon(qApp->applicationDirPath() + "/VersaAssistant.svg"));
		}
		else
		{
			logoPushButton->setIcon(QIcon(":/VersaAssistant.svg"));
		}
	}
	pVersaAppBarLayout->addWidget(logoPushButton);
	{
		appNameLabel = new QLabel(tr("VersaAssistant"), this);
		appNameLabel->setContentsMargins(0, 0, 0, 0);
		appNameLabel->setFont(ThemeQFont("FontAppName"));
		appNameLabel->setStyleSheet(QString("color:%1").arg(ThemeColor("AppBackgroundColor").name()));
	}
	pVersaAppBarLayout->addWidget(appNameLabel);
	
	
	
	menuBar->setMinimumHeight(titleHeight);
	menuBar->setStyleSheet(QString("QMenuBar {\n"
	                               "min-height: %1px;\n"
	                               "height: %2px;\n"
	                               "\t }")
			                       .arg(titleHeight)
			                       .arg(titleHeight));
	menuBar->setContentsMargins(0, 0, 0, 0);
	
	pVersaAppBarLayout->addWidget(menuBar);
	pVersaAppBarLayout->addStretch(1);
	// 协议层
	QWidget *pStateWidget = new QWidget(this);
	pStateWidget->setContentsMargins(16, 0, 0, 0);
	{
		QHBoxLayout *pStateLayout = new QHBoxLayout();
		pStateLayout->setSpacing(0);
		pStateLayout->setContentsMargins(0, 0, 0, 0);
		for (uint8_t i = 0; i < 2; i++)
		{
			QLabel *pStateQLabel = new QLabel(pStateWidget);
			pStateQLabel->setMinimumSize((int) (titleHeight * 3), titleHeight);
			pStateQLabel->setMaximumSize((int) (titleHeight * 3), titleHeight);
			pStateQLabel->setStyleSheet("QLabel{color:white;font-size:20px;}");
			pStateLayout->addWidget(pStateQLabel);
			if (i == 0)
			{
				stateTxQLabel = pStateQLabel;
			}
			else
			{
				stateRxQLabel = pStateQLabel;
			}
		}
		pStateWidget->setLayout(pStateLayout);
	}
	pVersaAppBarLayout->addWidget(pStateWidget);
	
	for (uint8_t button_i = 0; button_i < 3; button_i++)
	{
		QPushButton *pMaterialFlatButton = new QPushButton(this);
		pMaterialFlatButton->setMinimumSize((int) (titleHeight * 1.618), titleHeight);
		pMaterialFlatButton->setMaximumSize((int) (titleHeight * 1.618), titleHeight);
		pMaterialFlatButton->setFixedHeight(titleHeight);
		pMaterialFlatButton->setFixedWidth((int) (titleHeight * 1.618));
		pMaterialFlatButton->setIconSize(QSize(iconSize, iconSize));
		switch (button_i)
		{
			case 0:
			{
				minPushButton = pMaterialFlatButton;
				minPushButton->setObjectName("minToolButton");
				minPushButton->setIcon(ThemeIcon("IconAppMinCl", "AppBackgroundColor", iconSize));
				pVersaAppBarLayout->addWidget(minPushButton);
			}
				break;
			case 1:
			{
				maxPushButton = pMaterialFlatButton;
				maxPushButton->setObjectName("maxToolButton");
				maxPushButton->setProperty("MaxState", false);
				maxPushButton->setIcon(ThemeIcon("IconAppMaxMa", "AppBackgroundColor", iconSize));
				pVersaAppBarLayout->addWidget(maxPushButton);
			}
				break;
			case 2:
			{
				closePushButton = pMaterialFlatButton;
				closePushButton->setObjectName("closeToolButton");
				closePushButton->setIcon(ThemeIcon("IconAppClose", "AppBackgroundColor", iconSize));
			}
				break;
		}
		pVersaAppBarLayout->addWidget(pMaterialFlatButton);
	}
	
	QWidget *pVersaAppBarWidget = new QWidget(this);
	pVersaAppBarWidget->setContentsMargins(0, 0, 0, 0);
	pVersaAppBarWidget->setLayout(pVersaAppBarLayout);
	
	QHBoxLayout *pVersaAppBarWidgetLayout = new QHBoxLayout(this);
	pVersaAppBarWidgetLayout->setSpacing(0);
	pVersaAppBarWidgetLayout->setContentsMargins(0, 0, 0, 0);
	pVersaAppBarWidgetLayout->addWidget(pVersaAppBarWidget);
	this->setLayout(pVersaAppBarWidgetLayout);
}

void VersaAppBar::slotMaximizedChanged(bool bMax)
{
	if (bMax)
	{
		maxPushButton->setIcon(ThemeIcon("IconAppMaxNo", "AppBackgroundColor", iconSize));
	}
	else
	{
		maxPushButton->setIcon(ThemeIcon("IconAppMaxMa", "AppBackgroundColor", iconSize));
	}
}

VersaAppBar::~VersaAppBar()
{
	if (!minPushButton.isNull())
	{
		delete minPushButton;
		minPushButton = nullptr;
	}
	
	if (!maxPushButton.isNull())
	{
		delete maxPushButton;
		maxPushButton = nullptr;
	}
	
	if (!closePushButton.isNull())
	{
		delete closePushButton;
		closePushButton = nullptr;
	}
	
	if (!logoPushButton.isNull())
	{
		delete logoPushButton;
		logoPushButton = nullptr;
	}
	
	if (!appNameLabel.isNull())
	{
		delete appNameLabel;
		appNameLabel = nullptr;
	}
	
	if (!menuBarWidget.isNull())
	{
		delete menuBarWidget;
		menuBarWidget = nullptr;
	}
	
	if (!menuBar.isNull())
	{
		delete menuBar;
		menuBar = nullptr;
	}
	
	if (!stateTxQLabel.isNull())
	{
		delete stateTxQLabel;
		stateTxQLabel = nullptr;
	}
	
	if (!stateRxQLabel.isNull())
	{
		delete stateRxQLabel;
		stateRxQLabel = nullptr;
	}
}
