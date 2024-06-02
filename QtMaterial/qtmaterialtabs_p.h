#ifndef QTMATERIALTABS_P_H
#define QTMATERIALTABS_P_H

#include <QtGlobal>
#include <QBoxLayout>
#include "lib/qtmaterialtheme.h"
#include <lib/qtmaterialglobal.h>

class QBoxLayout;

class QtMaterialTabs;

class QtMaterialTabsInkBar;

class MATERIAL_EXPORT  QtMaterialTabsPrivate
{
	Q_DISABLE_COPY(QtMaterialTabsPrivate)
	
	Q_DECLARE_PUBLIC(QtMaterialTabs)

public:
	QtMaterialTabsPrivate(QtMaterialTabs *q);
	
	~QtMaterialTabsPrivate();
	
	void init(QBoxLayout::Direction dir);
	
	QtMaterialTabs *const q_ptr;
	QtMaterialTabsInkBar *inkBar;
	QBoxLayout::Direction tabDirection = QBoxLayout::Direction::LeftToRight;
	QBoxLayout *tabLayout;
	// QHBoxLayout          *tabHLayout;
	Material::RippleStyle rippleStyle;
	QColor inkColor;
	QColor backgroundColor;
	QColor textColor;
	int tab;
	bool showHalo;
	bool useThemeColors;
};

#endif // QTMATERIALTABS_P_H
