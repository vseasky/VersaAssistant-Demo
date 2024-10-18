/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\Frameless\QFramelessMainWindow.h
 * @Description  : 
 */
#ifndef QFRAMELESSMAINWINDOW_H
#define QFRAMELESSMAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include "QFramelessHelper.h"

class QFramelessHelper;

class QFramelessMainWindow : public QMainWindow
{
Q_OBJECT
public:
	explicit QFramelessMainWindow(QWidget *parent = 0, bool resizeEnable = true, bool shadowBorder = true,
	                              bool winNativeEvent = true);
	~QFramelessMainWindow();
	QPointer<QFramelessHelper> framelessHelper();

protected:
	//拦截系统事件用于修复系统休眠后唤醒程序的BUG
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
	bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif
	
	//Qt4的写法
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
	bool winEvent(MSG *message, long *result);
#endif
#endif
	
	virtual void paintEvent(QPaintEvent *e);

private:
	QPointer<QFramelessHelper> m_FramelessHelper;
};

#endif // QFRAMELESSMAINWINDOW_H
