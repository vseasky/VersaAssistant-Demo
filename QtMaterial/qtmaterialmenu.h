#ifndef QTMATERIALMENU_H
#define QTMATERIALMENU_H

#include <QtWidgets/QWidget>
#include <lib/qtmaterialglobal.h>

class MATERIAL_EXPORT QtMaterialMenu : public QWidget
{
Q_OBJECT

public:
	explicit QtMaterialMenu(QWidget *parent = 0);
	
	~QtMaterialMenu();
};

#endif // QTMATERIALMENU_H
