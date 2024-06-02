//
// Created by vSeasky on 2023/10/24.
//

#include "VersaAboutApp.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextStream>
#include <QTextCodec>
#include <VersaConfig.h>
#include "VersaAppVersionConfig.h"

VersaAboutApp::VersaAboutApp(QWidget *parent) :
		QWidget(parent,Qt::Dialog),
		tableWidget(this)
{
	setMinimumSize(800, 1024);
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	tableWidget.setColumnCount(2);
	tableWidget.setRowCount(5);
	tableWidget.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// 固定行高
	tableWidget.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	tableWidget.verticalHeader()->setDefaultSectionSize(48);
	tableWidget.horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	tableWidget.verticalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
	tableWidget.setColumnWidth(0, 200);
	// 隐藏水平表头
	tableWidget.horizontalHeader()->setVisible(false);
	// 隐藏垂直表头
	tableWidget.verticalHeader()->setVisible(false);
	layout->addWidget(&tableWidget);
	setLayout(layout);
	{
		{
			QLabel *label1 = new QLabel(&tableWidget);
			label1->setText(tr("软件名称"));
			label1->setAlignment(Qt::AlignCenter);
			tableWidget.setCellWidget(0, 0, label1);
			QLabel *label2 = new QLabel(&tableWidget);
			label2->setText(VERSA_ASSISTANT_APP);
			label2->setContentsMargins(12, 0, 12, 0);
			tableWidget.setCellWidget(0, 1, label2);
			
			label1->setFont(ThemeQFont("FontAppAbout"));
			label2->setFont(ThemeQFont("FontAppAbout"));
		}
		{
			QLabel *label1 = new QLabel(&tableWidget);
			label1->setText(tr("开发工具"));
			label1->setAlignment(Qt::AlignCenter);
			tableWidget.setCellWidget(1, 0, label1);
			QLabel *label2 = new QLabel(&tableWidget);
			label2->setText(VERSA_BUILD_TOOL);
			label2->setContentsMargins(12, 0, 12, 0);
			tableWidget.setCellWidget(1, 1, label2);
			
			label1->setFont(ThemeQFont("FontAppAbout"));
			label2->setFont(ThemeQFont("FontAppAbout"));
		}
		{
			QLabel *label1 = new QLabel(&tableWidget);
			label1->setText(tr("构建平台"));
			label1->setAlignment(Qt::AlignCenter);
			tableWidget.setCellWidget(2, 0, label1);
			QLabel *label2 = new QLabel(&tableWidget);
			label2->setText(VERSA_BUILD_PLATFOR);
			label2->setContentsMargins(12, 0, 12, 0);
			tableWidget.setCellWidget(2, 1, label2);
			
			label1->setFont(ThemeQFont("FontAppAbout"));
			label2->setFont(ThemeQFont("FontAppAbout"));
		}
		{
			QLabel *label1 = new QLabel(&tableWidget);
			label1->setText(tr("构建日期"));
			label1->setAlignment(Qt::AlignCenter);
			tableWidget.setCellWidget(3, 0, label1);
			QLabel *label2 = new QLabel(&tableWidget);
			label2->setText(VERSA_BUILD_DATE);
			label2->setContentsMargins(12, 0, 12, 0);
			tableWidget.setCellWidget(3, 1, label2);
			
			label1->setFont(ThemeQFont("FontAppAbout"));
			label2->setFont(ThemeQFont("FontAppAbout"));
		}
	}
	QTextEdit *pAboutInfo = new QTextEdit(&tableWidget);
	pAboutInfo->setContentsMargins(48, 24, 48, 24);
	pAboutInfo->setReadOnly(true);
	pAboutInfo->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
	
	QString resourceFilePath = ":/statement.txt";
	// 获取目标目录的路径
	// 读取文本文件内容
	QFile file(resourceFilePath);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		// 设置文件编码方式为UTF-8
		QTextCodec *codec = QTextCodec::codecForName("UTF-8");
		QTextStream in(&file);
		in.setCodec(codec);
		
		QString text = in.readAll();
		file.close();
		pAboutInfo->setFont(ThemeQFont("FontAppAbout"));
		// 设置文本内容
		pAboutInfo->setText(text);
	}
	
	tableWidget.setSpan(4, 0, 1, 2);
	tableWidget.setCellWidget(4, 0, pAboutInfo);
	hide();
}

VersaAboutApp::~VersaAboutApp()
{


}
