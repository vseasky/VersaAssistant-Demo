//
// Created by vSeasky on 2023/9/13.
//

#include "VersaMessageBox.h"
#include "VersaConfig.h"
#include <QVBoxLayout>
#include <QCursor>
#include <QLabel>
#include <QCoreApplication>
#include <QDebug>

VersaMessageBox *pVersaMessageBox = nullptr;

void VersaMessageInitFun(void)
{
	if (pVersaMessageBox == nullptr)
	{
		pVersaMessageBox = new VersaMessageBox(nullptr);
	}
}

void VersaMessageShowFun(const QString &str, int time)
{
	if (pVersaMessageBox != nullptr)
	{
		pVersaMessageBox->VersaMessagePrintf(str, time);
	}
}

void VersaMessageFreeFun(void)
{
	if (pVersaMessageBox != nullptr)
	{
		pVersaMessageBox->deleteLater();
		pVersaMessageBox = nullptr;
	}
}

VersaMessageBox::VersaMessageBox(QWidget *parent) :
		QFramelessDialog(parent),
		messageBoxInfo(this)
{
	// 无边框,透明窗口
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setContentsMargins(0, 0, 0, 0);
	
	pQVBoxLayout = new QVBoxLayout();
	pQVBoxLayout->setContentsMargins(0, 0, 0, 0);
	pQVBoxLayout->setSpacing(0);
	QWidget *pQWidget = new QWidget(this);
	pQWidget->setContentsMargins(0, 0, 0, 0);
	
	pQVBoxLayout->addWidget(pQWidget);
	
	pQWidget->setAttribute(Qt::WA_TranslucentBackground, false);
	
	pQLabelLayout = new QVBoxLayout();
	pQLabelLayout->setContentsMargins(0, 0, 0, 0);
	pQLabelLayout->setSpacing(0);
	
	pQLabelLayout->addWidget(&messageBoxInfo);
	
	pQWidget->setLayout(pQLabelLayout);
	
	this->setLayout(pQVBoxLayout);
	
	messageBoxInfo.setFont(ThemeQFont("FontAppName"));
	QObject::connect(&mTimer, &QTimer::timeout, this, &VersaMessageBox::VersaTimeout);
}

VersaMessageBox::~VersaMessageBox()
{
	mTimer.stop();
	if (pQVBoxLayout.isNull())
	{
		pQVBoxLayout->deleteLater();
		pQVBoxLayout = nullptr;
	}
	if (pQLabelLayout.isNull())
	{
		pQLabelLayout->deleteLater();
		pQLabelLayout = nullptr;
	}
}

void VersaMessageBox::VersaMessagePrintf(const QString &message, int showTime)
{
	// 获取鼠标相对于屏幕的位置
	QPoint globalPos = QCursor::pos();
	messageBoxInfo.setText(message);
	this->show();
	this->hide();
	this->move(globalPos - this->rect().bottomRight());
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	if (!mTimer.isActive())
	{
		mTimer.start(showTime);
	}
	else
	{
		mTimer.stop();
		mTimer.start(showTime);
	}
	this->show();
}

void VersaMessageBox::VersaTimeout()
{
	this->hide();
	mTimer.stop();
}