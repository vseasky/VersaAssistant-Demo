/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaQColorWidget.cpp
 * @Description  : 
 */


#include "VersaQColorWidget.h"
#include "VersaConfig.h"
#include "ImageSvgLoad.h"
#include "VersaMessageBox.h"
#include <QGridLayout>
#include <QPushButton>
#include <QClipboard>
#include <QApplication>
#include <QTabWidget>
#include <QDirIterator>
#include <QDebug>
#include <QPalette>
#include <QScrollArea>
#include <QTabBar>

VersaQColorWidget::VersaQColorWidget(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *pVseraStyleQVBoxLayout = new QVBoxLayout();
	
	QTabWidget *pVseraStyleQTabWidget = new QTabWidget(this);
	
	pVseraStyleQVBoxLayout->addWidget(pVseraStyleQTabWidget);
	pVseraStyleQVBoxLayout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(pVseraStyleQVBoxLayout);
	
	QScrollArea *pQColorStyleArea = new QScrollArea(pVseraStyleQTabWidget);
	QScrollArea *pIconStyleArea = new QScrollArea(pVseraStyleQTabWidget);
	
	pVseraStyleQTabWidget->addTab(pQColorStyleArea, tr("颜色"));
	pVseraStyleQTabWidget->addTab(pIconStyleArea, tr("Icon"));
	// pVseraStyleQTabWidget->tabBar()->tabRect(1).setHeight(48);
	// pVseraStyleQTabWidget->
	pQColorStyleArea->setWidgetResizable(true);
	pIconStyleArea->setWidgetResizable(true);
	pQColorStyleArea->setAlignment(Qt::AlignCenter);
	pIconStyleArea->setAlignment(Qt::AlignCenter);
	// 颜色
	{
		QWidget *pQColorQWidget = new QWidget(pQColorStyleArea);//QWidget子类
		pQColorStyleArea->setWidget(pQColorQWidget);
		pQColorQWidget->setMaximumWidth(8 * 20 + 64 * 16);
		QGridLayout *pQColorQGridLayout = new QGridLayout();
		pQColorQGridLayout->setSpacing(8);
		pQColorQGridLayout->setContentsMargins(4, 4, 4, 48);
		pQColorQWidget->setContentsMargins(4, 4, 4, 4);
		pQColorQWidget->setLayout(pQColorQGridLayout);
		
		clipboard = QApplication::clipboard();
		uint16_t indexOfPalette = 0;
		for (indexOfPalette = 0; indexOfPalette < sizeof(paletteQColor) / sizeof(QColor); indexOfPalette += 2)
		{
			QPushButton *pColorShowBtn = new QPushButton(this);
			pColorShowBtn->setAutoFillBackground(true);
			pColorShowBtn->setStyleSheet(
					QString("QPushButton{background:%1;}").arg(paletteQColor[indexOfPalette].name()));
			pColorShowBtn->setMinimumHeight(48);
			pColorShowBtn->setMaximumHeight(48);
			QPushButton *pTextShowBtn = new QPushButton(this);
			pTextShowBtn->setText(paletteQColor[indexOfPalette].name());
			pTextShowBtn->setMinimumHeight(48);
			pTextShowBtn->setMaximumWidth(96);
			connect(pTextShowBtn, &QPushButton::released, this, [=]()
			        {
				        clipboard->setText(pTextShowBtn->text());
				        VersaMessageShowFun(pTextShowBtn->text());
			        }
			);
			pQColorQGridLayout->addWidget(pColorShowBtn, indexOfPalette / 4, ((indexOfPalette % 4)) + 0, 1, 1);
			pQColorQGridLayout->addWidget(pTextShowBtn, indexOfPalette / 4, ((indexOfPalette % 4)) + 1, 1, 1);
		}
	}
	
	{    // Icon
		QWidget *pIconQWidget = new QWidget(pIconStyleArea);
		pIconStyleArea->setWidget(pIconQWidget);
		pIconQWidget->setMaximumWidth(8 * 20 + 64 * 16);
		
		const uint8_t iconNum = 16;
		QGridLayout *pIconQGridLayout = new QGridLayout();
		pIconQWidget->setLayout(pIconQGridLayout);
		pIconQGridLayout->setSpacing(8);
		pIconQGridLayout->setContentsMargins(4, 4, 4, 48);
		pIconQWidget->setContentsMargins(4, 4, 4, 4);
		uint32_t IconIndex = 0, IconCount = 0;
		// 添加 iconMingCuteButton
		QPushButton *iconMingCuteButton = new QPushButton(pIconQWidget);
		pIconQGridLayout->addWidget(iconMingCuteButton, IconIndex / iconNum, 0, 1, iconNum);
		IconIndex += iconNum;
		
		IconCount = 0;
		QDirIterator MingCute(":MingCute/", QDirIterator::Subdirectories);
		while (MingCute.hasNext())
		{
			QString IconMingCuteFile(MingCute.next());
			if (!IconMingCuteFile.contains(".svg", Qt::CaseInsensitive))
			{
				continue;
			}
			
			QPushButton *iconMingCuteIndexButton = new QPushButton(pIconQWidget);
			iconMingCuteIndexButton->setMinimumSize(64, 64);
			iconMingCuteIndexButton->setMaximumSize(64, 64);
			iconMingCuteIndexButton->setFixedSize(QSize(64, 64));
			iconMingCuteIndexButton->setIconSize(QSize(48, 48));
			iconMingCuteIndexButton->setIcon(ImageSvgLoad(IconMingCuteFile, ThemeColor("AppSelectColor"), 48));
			iconMingCuteIndexButton->setStyleSheet("QPushButton {\n"
			                                       "    border-radius: 2px;\n"
			                                       "    padding: 0;\n"
			                                       "    min-width: 64px;\n"
			                                       "    min-height: 64px;\n"
			                                       "    max-width: 64px;\n"
			                                       "    max-height: 64px;\n"
			                                       "}");
			pIconQGridLayout->addWidget(iconMingCuteIndexButton, IconIndex / iconNum, IconIndex % iconNum, 1, 1);
			connect(iconMingCuteIndexButton, &QPushButton::released, this, [IconMingCuteFile, this]()
			        {
				        clipboard->setText(IconMingCuteFile);
				        VersaMessageShowFun(IconMingCuteFile);
			        }
			);
			IconIndex++;
			IconCount++;
		}
		iconMingCuteButton->setText(QString("MingCute:(%1)").arg(IconCount));
		iconMingCuteButton->setMinimumHeight(48);
		
		IconIndex = (IconIndex / iconNum + 1) * iconNum;
		
		// // 添加 iconIconParkButton
		// QPushButton *iconIconParkButton = new QPushButton(pIconQWidget);
		// pIconQGridLayout->addWidget(iconIconParkButton, IconIndex / iconNum, 0, 1, iconNum);
		//
		// IconCount = 0;
		// IconIndex = (IconIndex / iconNum + 1) * iconNum;
		// QDirIterator IconPark(":IconPark/", QDirIterator::Subdirectories);
		// while (IconPark.hasNext())
		// {
		// 	QString IconParkFile(IconPark.next());
		// 	if (!IconParkFile.contains(".svg", Qt::CaseInsensitive))
		// 	{
		// 		continue;
		// 	}
		//
		// 	QPushButton *iconParkIndexButton = new QPushButton(pIconQWidget);
		// 	iconParkIndexButton->setStyleSheet("QPushButton {\n"
		// 	                                   "    border-radius: 2px;\n"
		// 	                                   "    padding: 0;\n"
		// 	                                   "    min-width: 64px;\n"
		// 	                                   "    min-height: 64px;\n"
		// 	                                   "    max-width: 64px;\n"
		// 	                                   "    max-height: 64px;\n"
		// 	                                   "}");
		// 	iconParkIndexButton->setMinimumSize(64, 64);
		// 	iconParkIndexButton->setMaximumSize(64, 64);
		// 	iconParkIndexButton->setFixedSize(QSize(64, 64));
		// 	iconParkIndexButton->setIconSize(QSize(48, 48));
		// 	iconParkIndexButton->setIcon(QIcon(IconParkFile));
		// 	// iconParkIndexButton->setIcon(ImageSvgLoad(IconPark.next(),"#26c6da",48));
		// 	pIconQGridLayout->addWidget(iconParkIndexButton, IconIndex / iconNum, IconIndex % iconNum, 1, 1);
		// 	connect(iconParkIndexButton, &QPushButton::released, this, [IconParkFile, this]()
		// 	        {
		// 		        clipboard->setText(IconParkFile);
		// 		        VersaMessage(IconParkFile);
		// 	        }
		// 	);
		//
		// 	IconIndex++;
		// 	IconCount++;
		// }
		// iconIconParkButton->setText(QString("IconPark:(%1)").arg(IconCount));
	}
}


VersaQColorWidget::~VersaQColorWidget()
{
	if (clipboard)
	{
		clipboard->deleteLater();
		clipboard = nullptr;
	}
}