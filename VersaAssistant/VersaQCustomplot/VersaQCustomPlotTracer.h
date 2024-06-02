#ifndef VERSA_QCUSTOMPLOT_TRACER_H
#define VERSA_QCUSTOMPLOT_TRACER_H

#include <qcustomplot.h>
#include <QObject>
#include <QPointer>

/*
 **********************************************
 ************VseraQCustomPlot的移动光标************
 **********************************************
 */

class VersaTracer : public QCPItemTracer
{
public:
	VersaTracer(QCustomPlot *parentPlot) : QCPItemTracer(parentPlot)
	{}

protected:
	bool event(QEvent *event) override
	{
		if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease ||
		    event->type() == QEvent::MouseMove)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			QApplication::sendEvent(mParentPlot, mouseEvent);
			return true;
		}
		return QCPItemTracer::event(event);
	}
};

class VersaQCustomPlotTracer : public QObject
{
Q_OBJECT
public:
	
	enum TracerShowType
	{
		showTracerPoint = 0X01, showTracerLabel = 0X02, showTracerArrow = 0X04, showTracerLine =
		0X08, showTracerTop = 0X10, showTracerBottom = 0X20, showTracerAll =
		showTracerPoint | showTracerLabel | showTracerArrow | showTracerLine
	};
	explicit VersaQCustomPlotTracer(QCustomPlot *_plot);
	~VersaQCustomPlotTracer();
	void setLabelFont(QFont font);
	void setPen(const QPen &pen);
	void setBrush(const QBrush &brush);
	void setText(const QString &text);
	void setLabelPen(const QPen &pen);
	void updatePosition(TracerShowType type, double xValue, double yValue);
	void setVisible(bool visible);
protected:
	QCustomPlot* plot;          // 传入实例化的QcustomPlot
	QPointer<VersaTracer> tracerPoint;      // 跟踪的点
	QPointer<QCPItemText> tracerLabel;        // 显示的数值
	QPointer<QCPItemLine> tracerArrow;        // 箭头
	QPointer<QCPItemLine> tracerLine;        // 线段
	TracerShowType currentType;
	bool visible;
signals:
public slots:

};

#endif // MYTRACER_H
