#ifndef QTMATERIALCIRCULARPROGRESS_P_H
#define QTMATERIALCIRCULARPROGRESS_P_H

#include <QtGlobal>
#include "lib/qtmaterialtheme.h"
#include <lib/qtmaterialglobal.h>

class QtMaterialCircularProgress;

class QtMaterialCircularProgressDelegate;

class MATERIAL_EXPORT QtMaterialCircularProgressPrivate
{
	Q_DISABLE_COPY(QtMaterialCircularProgressPrivate)
	
	Q_DECLARE_PUBLIC(QtMaterialCircularProgress)

public:
	QtMaterialCircularProgressPrivate(QtMaterialCircularProgress *q);
	
	~QtMaterialCircularProgressPrivate();
	
	void init();
	
	QtMaterialCircularProgress *const q_ptr;
	QtMaterialCircularProgressDelegate *delegate;
	Material::ProgressType progressType;
	QColor color;
	qreal penWidth;
	int size;
	bool useThemeColors;
};

#endif // QTMATERIALCIRCULARPROGRESS_P_H
