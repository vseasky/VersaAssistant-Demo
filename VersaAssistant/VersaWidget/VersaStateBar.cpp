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