/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\ImageSvgLoad.h
 * @Description  : 
 */
#ifndef IMAGESVGLOAD_H
#define IMAGESVGLOAD_H

#include <QIcon>
#include <QString>

QIcon ImageSvgLoad(const QString &File, QColor color);
QIcon ImageSvgLoad(const QString &File, QColor color, int size);

#endif // IMAGESVGLOAD_H
