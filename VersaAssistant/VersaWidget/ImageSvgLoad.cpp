/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\ImageSvgLoad.cpp
 * @Description  : 
 */
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
