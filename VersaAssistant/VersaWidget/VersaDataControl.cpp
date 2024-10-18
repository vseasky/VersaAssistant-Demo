/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaDataControl.cpp
 * @Description  : 
 */

#include "VersaDataControl.h"
#include <QDebug>
/*!
 * 数据滑块控制，一个简单的左右零点滑块
 */

VersaDataControl::VersaDataControl(QWidget *parent) : QWidget(parent)
{
	pVersaScrollBar = new VersaSlider(Qt::Horizontal, this);
	pVersaScrollBar->setRange(-1000*1000, 1000*1000); // 设置取值范围
	pVersaScrollBar->setValue(0);      // 设置初始值
	pVersaScrollBar->setSingleStep(1);
	pVersaScrollBar->setFixedHeight(40);
	pVersaScrollBar->setMinimumHeight(40);
	QVBoxLayout *pLayout = new QVBoxLayout();
	pLayout->addWidget(pVersaScrollBar);
	this->setLayout(pLayout);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
}

VersaDataControl::~VersaDataControl()
{
	if (!pVersaScrollBar.isNull())
	{
		pVersaScrollBar->deleteLater();
		pVersaScrollBar = nullptr;
	}
}