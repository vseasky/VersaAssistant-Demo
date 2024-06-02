#include "QFramelessWidget.h"

QFramelessWidget::QFramelessWidget(QWidget *parent, bool resizeEnable, bool shadowBorder, bool winNativeEvent)
		: QWidget(parent), m_FramelessHelper(0)
{
	m_FramelessHelper = new QFramelessHelper(this, resizeEnable, shadowBorder, winNativeEvent, this);
}

QFramelessWidget::~QFramelessWidget()
{
	if (!m_FramelessHelper.isNull())
	{
		m_FramelessHelper->deleteLater();
		m_FramelessHelper = nullptr;
	}
	
}

QPointer<QFramelessHelper> QFramelessWidget::framelessHelper()
{
	return m_FramelessHelper;
}

void QFramelessWidget::paintEvent(QPaintEvent *e)
{
	if (m_FramelessHelper == 0)
	{
		return;
	}
	m_FramelessHelper->paintEvent(e);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
bool QFramelessWidget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else

bool QFramelessWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
	bool resultFrameless = false;
	if (m_FramelessHelper == 0)
	{
		return false;
	}
	
	resultFrameless = m_FramelessHelper->nativeEvent(eventType, message, result);
	
	return resultFrameless;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_WIN
bool QFramelessWidget::winEvent(MSG *message, long *result)
{
	return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif

