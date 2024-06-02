/*!
 * 数据滑块控制，一个简单的左右零点滑块
 */
#include "VersaDataControl.h"
#include <QDebug>


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