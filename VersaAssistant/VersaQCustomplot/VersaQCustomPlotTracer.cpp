/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaQCustomplot\VersaQCustomPlotTracer.cpp
 * @Description  : 
 */
#include "VersaQCustomPlotTracer.h"


VersaQCustomPlotTracer::VersaQCustomPlotTracer(QCustomPlot *_plot) :
		plot(_plot),
		visible(false)
{
	if (plot)
	{
		// 跟踪点
		tracerPoint = new VersaTracer(plot);
		tracerPoint->setStyle(QCPItemTracer::tsCrosshair);//可以选择设置追踪光标的样式，这个是小十字，还有大十字，圆点等样式
		tracerPoint->setPen(QPen(Qt::blue));//设置tracer的颜色
		tracerPoint->setBrush(Qt::blue);
		QPen tracerPointPen(Qt::red);
		tracerPointPen.setStyle(Qt::CustomDashLine);
		// 实线长度为 4，虚线长度为 3，实线长度为 4，虚线长度为 3
		tracerPointPen.setDashPattern(QVector<qreal>() << 4 << 3 << 4 << 3);
		tracerPointPen.setWidth(1.5);
		tracerPoint->setPen(tracerPointPen);
		
		tracerLabel = new QCPItemText(plot);
		tracerLabel->setLayer("overlay");
		tracerLabel->setClipToAxisRect(false);
		tracerLabel->setPadding(QMargins(8, 8, 8, 8));
		
		// tracerLabel 位置锚定到 tracerPoint
		tracerLabel->setFont(QFont("宋体", 10));
		
		tracerArrow = new QCPItemLine(plot);
		tracerArrow->setLayer("overlay");
		tracerArrow->setPen(QPen(Qt::blue));//设置箭头的颜色
		tracerArrow->setClipToAxisRect(false);
		tracerArrow->setHead(QCPLineEnding::esSpikeArrow);
		
		tracerLine = new QCPItemLine(plot);
		tracerLine->setLayer("overlay");
		tracerLine->setPen(QPen(Qt::blue));//设置箭头的颜色
		tracerLine->setClipToAxisRect(false);
		tracerLine->setHead(QCPLineEnding::esNone);
		
		// 设置位置类型为使用绝对像素位置
		tracerPoint->position->setTypeX(QCPItemPosition::ptPlotCoords);
		tracerPoint->position->setTypeY(QCPItemPosition::ptPlotCoords);
		
		tracerLabel->setBrush(QBrush(QColor(255, 255, 255, 190)));
		tracerLabel->setPen(QPen(Qt::blue));
		tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		
		tracerArrow->end->setParentAnchor(tracerPoint->position);
		tracerArrow->start->setParentAnchor(tracerArrow->end);
		tracerArrow->start->setCoords(25, 25);
		
		tracerLine->end->setParentAnchor(tracerArrow->start);
		tracerLine->start->setParentAnchor(tracerLine->end);
		tracerLine->start->setCoords(25, 0);
		tracerLabel->position->setParentAnchor(tracerLine->start);
		setVisible(false);
	}
}

VersaQCustomPlotTracer::~VersaQCustomPlotTracer()
{
	if (plot->hasItem(tracerPoint))
	{
		plot->removeItem(tracerPoint);
	}
	if (plot->hasItem(tracerLabel))
	{
		plot->removeItem(tracerLabel);
	}
	if (plot->hasItem(tracerArrow))
	{
		plot->removeItem(tracerArrow);
	}
	if (plot->hasItem(tracerLine))
	{
		plot->removeItem(tracerLine);
	}
	
	plot = nullptr;
	
	if (!tracerPoint.isNull())
	{
		tracerPoint->deleteLater();
		tracerPoint = nullptr;
	}
	
	if (!tracerLabel.isNull())
	{
		tracerLabel->deleteLater();
		tracerLabel = nullptr;
	}
	
	if (!tracerArrow.isNull())
	{
		tracerArrow->deleteLater();
		tracerArrow = nullptr;
	}
	
	if (!tracerLine.isNull())
	{
		tracerLine->deleteLater();
		tracerLine = nullptr;
	}
}

void VersaQCustomPlotTracer::setLabelFont(QFont font)
{
	tracerLabel->setFont(font);
}

void VersaQCustomPlotTracer::setPen(const QPen &pen)
{
	tracerPoint->setPen(pen);
	tracerArrow->setPen(pen);
	tracerLine->setPen(pen);
}

void VersaQCustomPlotTracer::setBrush(const QBrush &brush)
{
	tracerPoint->setBrush(brush);
}

void VersaQCustomPlotTracer::setLabelPen(const QPen &pen)
{
	tracerLabel->setPen(pen);
}

void VersaQCustomPlotTracer::setText(const QString &text)
{
	tracerLabel->setText(text);
}

void VersaQCustomPlotTracer::setVisible(bool visible)
{
	tracerPoint->setVisible(visible);
	tracerLabel->setVisible(visible);
	tracerArrow->setVisible(visible);
	tracerLine->setVisible(visible);
}

void VersaQCustomPlotTracer::updatePosition(TracerShowType type, double xValue, double yValue)
{
	if (type != currentType)
	{
		tracerPoint->setVisible((type & showTracerPoint) != 0);
		tracerLabel->setVisible((type & showTracerLabel) != 0);
		tracerArrow->setVisible((type & showTracerArrow) != 0);
		tracerLine->setVisible((type & showTracerLine) != 0);
		if ((type & showTracerTop) != 0)
		{
			tracerArrow->start->setCoords(25, -25);
		}
		else if ((type & showTracerBottom) != 0)
		{
			tracerArrow->start->setCoords(25, 25);
		}
		currentType = type;
	}
	if (yValue > plot->yAxis->range().upper)
	{
		yValue = plot->yAxis->range().upper;
	}
	tracerPoint->position->setCoords(xValue, yValue);
}
