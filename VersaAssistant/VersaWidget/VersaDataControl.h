/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaDataControl.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSADATACONTROL_H
#define VERSA_ASSISTANT_VERSADATACONTROL_H

#include <QWidget>
#include <QPointer>
#include <QStackedLayout>
#include <QSlider>
#include <QPainter>
#include <QWheelEvent>
#include <QStyleOptionSlider>

class VersaSlider : public QSlider
{
public:
	VersaSlider(Qt::Orientation dir, QWidget *parent = nullptr) : QSlider(dir, parent)
	{		
		setTickPosition(QSlider::NoTicks);
	}

protected:
	void paintEvent(QPaintEvent *event) override
	{
		QSlider::paintEvent(event);
		QPainter painter(this);
		QStyleOptionSlider opt;
		initStyleOption(&opt);
		
		QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt,
		                                           QStyle::SC_SliderGroove, this);
		QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt,
		                                           QStyle::SC_SliderHandle, this);
		
		int grooveCenter;
		int handleCenter;
		
		grooveCenter = grooveRect.center().x();
		handleCenter = handleRect.center().x();
		
		if (handleCenter < grooveCenter)
		{
			painter.fillRect(handleRect.center().x(),               // 绘制区域的左上角 x 坐标
			                 handleRect.y(),                        // 绘制区域的左上角 y 坐标
			                 grooveCenter - handleCenter,           // 绘制区域的宽度
			                 handleRect.height(),                   // 绘制区域的高度
			                 QColor(38, 198, 218));
		}
		else if (handleCenter > grooveCenter)
		{
			painter.fillRect(handleRect.center().x(),               // 绘制区域的左上角 x 坐标
			                 handleRect.y(),                        // 绘制区域的左上角 y 坐标
			                 grooveCenter - handleCenter,           // 绘制区域的宽度
			                 handleRect.height(),                   // 绘制区域的高度
			                 QColor(255, 0, 0));
		}
	}
	
	void wheelEvent(QWheelEvent *event) override
	{
		// 忽略鼠标滚轮事件
		event->ignore();
	}
};

class VersaDataControl : public QWidget
{
Q_OBJECT
public:
	VersaDataControl(QWidget *parent = nullptr);
	~VersaDataControl();
	QPointer<VersaSlider> pVersaScrollBar;
private:
};


#endif //VERSA_ASSISTANT_VERSADATACONTROL_H
