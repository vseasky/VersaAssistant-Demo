#include "VersaTabBar.h"
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <VersaConfig.h>
#include <QLabel>
#include <QDebug>

VersaTabBar::VersaTabBar(QWidget *parent) :
		QWidget(parent),
		tabWidth(64)
{
	// 添加一个容器
	QWidget *pVersaTabBarQWidget = new QWidget(this);
	pVersaTabBarQWidget->setContentsMargins(0, 0, 0, 0);
	
	QVBoxLayout *pVersaTabBarLayout = new QVBoxLayout(this);
	pVersaTabBarLayout->setSpacing(0);
	pVersaTabBarLayout->setContentsMargins(0, 0, 0, 0);
	pVersaTabBarLayout->addWidget(pVersaTabBarQWidget);
	
	this->setLayout(pVersaTabBarLayout);
	this->setContentsMargins(0, 0, 0, 0);
	this->setMinimumWidth(tabWidth);
	
	// 打开设备按钮
	pOpenButton = new QPushButton(pVersaTabBarQWidget);
	pOpenButton->setMinimumSize(tabWidth, tabWidth);
	pOpenButton->setMaximumSize(tabWidth, tabWidth);
	pOpenButton->setStyleSheet("QPushButton{height:64px;width:64px;}");
	pOpenButton->setIconSize(QSize(tabWidth * 0.75, tabWidth * 0.75));
	pOpenButton->setIcon(ThemeIcon("IconDisConnect", "AppBackgroundColor", tabWidth * 0.75));
	pOpenButton->setCheckable(true);
	
	connect(pOpenButton, &QPushButton::released, [=]()
	{
		if (pOpenButton->isChecked())
		{
			pOpenButton->setChecked(false);
		}
		// 按下事件
		emit openReleased();
	});
	
	// 使能协议按钮
	pProtocolButton = new QPushButton(pVersaTabBarQWidget);
	pProtocolButton->setMinimumSize(tabWidth, tabWidth);
	pProtocolButton->setMaximumSize(tabWidth, tabWidth);
	pProtocolButton->setStyleSheet("QPushButton{height:64px;width:64px;}");
	pProtocolButton->setIconSize(QSize(tabWidth * 0.75, tabWidth * 0.75));
	pProtocolButton->setIcon(ThemeIcon("IconAvalanche2", "AppBackgroundColor", tabWidth * 0.75));
	pProtocolButton->setCheckable(true);
	
	connect(pProtocolButton, &QPushButton::released, [=]()
	{
		emit protocolEnableChanged(pProtocolButton->isChecked());
	});
	
	// 物理层
	QWidget *pPhysicalWidget = new QWidget(pVersaTabBarQWidget);
	pPhysicalWidget->setContentsMargins(0, 0, 0, 0);
	{
		QVBoxLayout *pPhysicalButtonLayout = new QVBoxLayout(pVersaTabBarQWidget);
		pPhysicalButtonLayout->setSpacing(0);
		pPhysicalButtonLayout->setContentsMargins(0, 0, 0, 0);
		for (uint8_t i = 0; i < 3; i++)
		{
			QPushButton *pPhysicalQPushButton = new QPushButton(pPhysicalWidget);
			pPhysicalQPushButton->setMinimumSize(tabWidth, tabWidth);
			pPhysicalQPushButton->setMaximumSize(tabWidth, tabWidth);
			pPhysicalQPushButton->setStyleSheet("QPushButton{height:64px;width:64px;}");
			pPhysicalQPushButton->setIconSize(QSize(tabWidth * 0.75, tabWidth * 0.75));
			
			pPhysicalQPushButton->setAutoExclusive(true);
			pPhysicalQPushButton->setCheckable(true);
			pPhysicalButtonLayout->addWidget(pPhysicalQPushButton);
			connect(pPhysicalQPushButton, &QPushButton::released, this, [=]()
			{
				currentIndexChanged(i);
			});
			
			switch (i)
			{
				case 0:
				{
					pPhysicalQPushButton->setIcon(ThemeIcon("IconPhysicalSerial", "AppBackgroundColor", tabWidth * 0.75));
					break;
				}
				case 1:
				{
					pPhysicalQPushButton->setIcon(ThemeIcon("IconPhysicalInternet", "AppBackgroundColor", tabWidth * 0.75));
					break;
				}
				case 2:
				{
					pPhysicalQPushButton->setIcon(ThemeIcon("IconPhysicalUSB", "AppBackgroundColor", tabWidth * 0.75));
					break;
				}
			}
		}
		pPhysicalWidget->setLayout(pPhysicalButtonLayout);
	}
	
	QVBoxLayout *pTabBarLayout = new QVBoxLayout();
	pTabBarLayout->setSpacing(8);
	pTabBarLayout->setContentsMargins(0, 0, 0, 0);
	
	pTabBarLayout->addWidget(pOpenButton);
	pTabBarLayout->addWidget(pProtocolButton);
	pTabBarLayout->addWidget(pPhysicalWidget);
	pTabBarLayout->addStretch(1);
	pVersaTabBarQWidget->setLayout(pTabBarLayout);
}

void VersaTabBar::refreshOpenState(bool checked)
{
	pOpenButton->setChecked(checked);
}

VersaTabBar::~VersaTabBar()
{
	if (!pOpenButton.isNull())
	{
		pOpenButton->deleteLater();
		pOpenButton = nullptr;
	}
	
	if (!pProtocolButton.isNull())
	{
		pProtocolButton->deleteLater();
		pProtocolButton = nullptr;
	}
	
}

