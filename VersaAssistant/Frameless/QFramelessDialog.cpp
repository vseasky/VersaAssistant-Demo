/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\Frameless\QFramelessDialog.cpp
 * @Description  : 
 */
#include "QFramelessDialog.h"

QFramelessDialog::QFramelessDialog(QWidget *parent, bool resizeEnable, bool shadowBorder, bool winNativeEvent)
		: QDialog(parent), m_FramelessHelper(0)
{
	m_FramelessHelper = new QFramelessHelper(this, resizeEnable, shadowBorder, winNativeEvent, this);
}

QPointer<QFramelessHelper> QFramelessDialog::framelessHelper()
{
	return m_FramelessHelper;
}

QFramelessDialog::~QFramelessDialog()
{
	if (!m_FramelessHelper.isNull())
	{
		m_FramelessHelper->deleteLater();
		m_FramelessHelper = nullptr;
	}
}

void QFramelessDialog::paintEvent(QPaintEvent *e)
{
	if (m_FramelessHelper == 0)
	{
		return;
	}
	m_FramelessHelper->paintEvent(e);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
bool QFramelessDialog::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else

bool QFramelessDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
	if (m_FramelessHelper == 0)
	{
		return false;
	}
	return m_FramelessHelper->nativeEvent(eventType, message, result);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
bool QFramelessDialog::winEvent(MSG *message, long *result)
{
	return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif

