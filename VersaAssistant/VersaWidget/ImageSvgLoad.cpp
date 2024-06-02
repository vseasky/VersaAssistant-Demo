#include "ImageSvgLoad.h"
#include <QPainter>
#include <QIcon>
#include <QFile>
#include <QDebug>


QIcon ImageSvgLoad(const QString &File, QColor color)
{
	QIcon imgIcon(File);
	QPixmap pixmap(imgIcon.pixmap(24));
	QPainter painter(&pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), color);
	painter.end();
	return QIcon(pixmap);
}

QIcon ImageSvgLoad(const QString &File, QColor color, int size)
{
	QIcon imgIcon(File);
	QPixmap pixmap(imgIcon.pixmap(size));
	QPainter painter(&pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), color);
	painter.end();
	return QIcon(pixmap);
}
