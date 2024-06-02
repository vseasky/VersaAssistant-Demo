#ifndef QTMATERIALAPPBAR_H
#define QTMATERIALAPPBAR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <lib/qtmaterialtheme.h>
#include <lib/qtmaterialglobal.h>

class QtMaterialAppBarPrivate;

class QtMaterialFlatButton;


class MATERIAL_EXPORT QtMaterialAppBar : public QWidget
{
Q_OBJECT
	
	Q_PROPERTY(QColor foregroundColor WRITE setForegroundColor READ foregroundColor)
	Q_PROPERTY(QColor backgroundColor WRITE setBackgroundColor READ backgroundColor)

public:
	QtMaterialFlatButton *minToolButton;
	QtMaterialFlatButton *maxToolButton;
	QtMaterialFlatButton *closeToolButton;
	QtMaterialFlatButton *logoButton;
	QLabel *textLabel;
	QMenuBar *menuBar;
	
	explicit QtMaterialAppBar(int height, int icon_size, QWidget *parent = nullptr);
	
	~QtMaterialAppBar();
	
	QSize sizeHint() const Q_DECL_OVERRIDE;
	
	void setUseThemeColors(bool value);
	
	bool useThemeColors() const;
	
	void setForegroundColor(const QColor &color);
	
	QColor foregroundColor() const;
	
	void setBackgroundColor(const QColor &color);
	
	QColor backgroundColor() const;
	
	inline QHBoxLayout *appBarLayout() const;

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	
	const QScopedPointer<QtMaterialAppBarPrivate> d_ptr;
	
	void mousePressEvent(QMouseEvent *event);   // 鼠标点击
	void mouseMoveEvent(QMouseEvent *event);    // 鼠标移动
	void mouseReleaseEvent(QMouseEvent *event); // 鼠标释放
	void mouseDoubleClickEvent(QMouseEvent *event);// 鼠标双击

private:
	Q_DISABLE_COPY(QtMaterialAppBar)
	
	Q_DECLARE_PRIVATE(QtMaterialAppBar)
	
	QPoint topLeftPoint;
	QPoint mousePoint;
	QWidget *moveQWidget;
	bool mousePressed = false;
	
	int iconSize = 32;
	int titleHeight = 48;
	
	void toggleMaxState();
};

inline QHBoxLayout *QtMaterialAppBar::appBarLayout() const
{
	return static_cast<QHBoxLayout *>(layout());
}

#endif // QTMATERIALAPPBAR_H
