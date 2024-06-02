#ifndef QTMATERIALTABS_H
#define QTMATERIALTABS_H

#include <QtWidgets/QWidget>
#include <QIcon>
#include "lib/qtmaterialtheme.h"
#include <QBoxLayout>
#include <lib/qtmaterialglobal.h>

class QtMaterialTabsPrivate;

class QtMaterialTab;

class  MATERIAL_EXPORT QtMaterialTabs :public QWidget
{
Q_OBJECT

public:
	enum TabsInkBarDirection
	{
		Left, Right, Top, Bottom
	};
	
	explicit QtMaterialTabs(QBoxLayout::Direction dir, QWidget *parent = 0);
	
	~QtMaterialTabs();
	
	void setTabsInkBarRect(TabsInkBarDirection dir, int width);
	
	void setUseThemeColors(bool value);
	
	bool useThemeColors() const;
	
	void setHaloVisible(bool value);
	
	bool isHaloVisible() const;
	
	void setRippleStyle(Material::RippleStyle style);
	
	Material::RippleStyle rippleStyle() const;
	
	void setInkColor(const QColor &color);
	
	QColor inkColor() const;
	
	void setBackgroundColor(const QColor &color);
	
	QColor backgroundColor() const;
	
	void setTextColor(const QColor &color);
	
	QColor textColor() const;
	
	void addTab(const QString &text, const QIcon &icon = QIcon());
	
	void setCurrentTab(QtMaterialTab *tab);
	
	void setCurrentTab(int index);
	
	int currentIndex() const;

Q_SIGNALS:
	
	void currentChanged(int index);

protected:
	void setTabActive(int index, bool active = true);
	
	void updateTabs();
	
	const QScopedPointer<QtMaterialTabsPrivate> d_ptr;


private:
	QSize TabIconSize = QSize(64, 64);
	QSize TabSize = QSize(80, 80);
	Q_DISABLE_COPY(QtMaterialTabs)
	
	Q_DECLARE_PRIVATE(QtMaterialTabs)
};

#endif // QTMATERIALTABS_H
