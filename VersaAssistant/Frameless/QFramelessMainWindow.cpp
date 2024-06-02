#include "QFramelessMainWindow.h"

QFramelessMainWindow::QFramelessMainWindow(QWidget *parent, bool resizeEnable, bool shadowBorder, bool winNativeEvent)
		: QMainWindow(parent), m_FramelessHelper(0)
{
	m_FramelessHelper = new QFramelessHelper(this, resizeEnable, shadowBorder, winNativeEvent, this);
}

QPointer<QFramelessHelper> QFramelessMainWindow::framelessHelper()
{
	return m_FramelessHelper;
}

QFramelessMainWindow::~QFramelessMainWindow()
{
	if (!m_FramelessHelper.isNull())
	{
		m_FramelessHelper->deleteLater();
		m_FramelessHelper = nullptr;
	}
}

void QFramelessMainWindow::paintEvent(QPaintEvent *e)
{
	if (m_FramelessHelper == 0)
	{
		return;
	}
	m_FramelessHelper->paintEvent(e);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
bool QFramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else

bool QFramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
bool QFramelessMainWindow::winEvent(MSG *message, long *result)
{
	return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif

