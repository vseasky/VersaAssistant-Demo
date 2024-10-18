/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaStateBar.cpp
 * @Description  : 
 */
#include "VersaStateBar.h"
#include <QHBoxLayout>

VersaStateBar::VersaStateBar(QWidget *parent) :
		QWidget(parent)
{
	QHBoxLayout *pVersaStateBarLayout = new QHBoxLayout(this);
	QWidget *pVersaStateWidget = new QWidget(this);
	pVersaStateWidget->setContentsMargins(0, 0, 0, 0);
	pVersaStateBarLayout->setContentsMargins(0, 0, 0, 0);
	pVersaStateBarLayout->addWidget(pVersaStateWidget);
	pVersaStateWidget->setMinimumHeight(48);
	
}

VersaStateBar::~VersaStateBar()
{

}