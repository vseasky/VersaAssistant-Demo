/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\Frameless\QDrawShadowHelper.h
 * @Description  : 
 */
#ifndef QSHADOWHELPER_H
#define QSHADOWHELPER_H

#include <QWidget>
#include <QPointer>

class QPainter;

class QDrawShadowHelper : public QObject
{
Q_OBJECT

public:
	explicit QDrawShadowHelper(QWidget *w, int shadowSize = 10, QObject *parent = 0);
	~QDrawShadowHelper();
	void setShadowSize(int shadowSize);
	void paint(QPainter *p);
	int shadowSize();
	
	QColor getClientBorderColor() const;
	void setClientBorderColor(const QColor &value);

public slots:
	void hide();
	void show();
protected:
	QImage makeShadowImage(int shadowSize, bool activated);
	bool splitRect(const QRect &rcSrc, int shadowSize, QRect *parrayRect, int nArrayCount);
private:
	int m_shadowSize;
	QImage m_img;
	QRect m_arrayImageGrid[9];
	QColor m_clientBorderColor;
	QColor m_clientBgColor;
	QPointer<QWidget> m_widget;
	bool m_show;
};

#endif // QSHADOWHELPER_H
