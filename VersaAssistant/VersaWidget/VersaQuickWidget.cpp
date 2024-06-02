#include "VersaQuickWidget.h"
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QDebug>

VersaQuickWidget::VersaQuickWidget(QWidget *parent) : QWidget(parent), sidebar_hidden(true), sidebar_width(150)
{
	initUI();
}

VersaQuickWidget::~VersaQuickWidget()
{
	if (!sidebarButton.isNull())
	{
		sidebarButton->deleteLater();
		sidebarButton = nullptr;
	}
	
	if (!sidebar.isNull())
	{
		sidebar->deleteLater();
		sidebar = nullptr;
	}
}


void VersaQuickWidget::initUI()
{
	setGeometry(100, 100, 800, 600);
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	sidebarButton = new QPushButton("Toggle Sidebar", this);
	connect(sidebarButton, &QPushButton::clicked, this, &VersaQuickWidget::toggleSidebar);
	
	layout->addWidget(sidebarButton);
	
	sidebar = new QWidget(this);
	sidebar->setGeometry(-sidebar_width, 0, sidebar_width, height());
	
	QPalette pal = sidebar->palette();
	pal.setColor(QPalette::Background, Qt::darkGray);
	sidebar->setAutoFillBackground(true);
	sidebar->setPalette(pal);
	
	layout->addWidget(sidebar);
	setLayout(layout);
}

// public slots:
void VersaQuickWidget::toggleSidebar()
{
	if (sidebar_hidden)
	{
		showSidebar();
	}
	else
	{
		hideSidebar();
	}
}

void VersaQuickWidget::showSidebar()
{
	QPropertyAnimation *animation = new QPropertyAnimation(sidebar, "geometry");
	animation->setDuration(300);
	QRect endRect(0, 0, sidebar_width, height());
	animation->setEndValue(endRect);
	animation->start();
	sidebar_hidden = false;
}

void VersaQuickWidget::hideSidebar()
{
	QPropertyAnimation *animation = new QPropertyAnimation(sidebar, "geometry");
	animation->setDuration(300);
	QRect endRect(-sidebar_width, 0, sidebar_width, height());
	animation->setEndValue(endRect);
	animation->start();
	sidebar_hidden = true;
}

