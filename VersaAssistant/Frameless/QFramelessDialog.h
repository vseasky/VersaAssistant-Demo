/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\Frameless\QFramelessDialog.h
 * @Description  : 
 */
#ifndef QFRAMELESSDIALOG_H
#define QFRAMELESSDIALOG_H

#include <QPointer>
#include <QDialog>
#include "QFramelessHelper.h"

class QFramelessHelper;

class QFramelessDialog : public QDialog
{
Q_OBJECT
public:
	explicit QFramelessDialog(QWidget *parent = 0, bool resizeEnable = false, bool shadowBorder = true,
	                          bool winNativeEvent = true);
	~QFramelessDialog();
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

#endif // FRAMELESSDIALOG_H
