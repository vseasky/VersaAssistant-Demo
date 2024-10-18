/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaWidget.h
 * @Description  : 
 */
#ifndef VERSA_WIDGET_H
#define VERSA_WIDGET_H

#include <QFramelessWidget.h>
#include <QByteArray>
#include "VersaDockWidget.h"
#include "VersaAppBar.h"
#include "VersaTabBar.h"
#include "VersaStateBar.h"
#include "VersaSideWidget.h"
#include "VersaLicenseWidget.h"
#include "VersaAboutApp.h"
#include <QStackedWidget>
#include <QPointer>

class VersaWidget : public QFramelessWidget
{
Q_OBJECT
public:
	VersaWidget(QWidget *parent = nullptr,
	            bool resizeEnable = true,
	            bool shadowBorder = true,
	            bool winNativeEvent = true);
	
	~VersaWidget();
	void VersaFree();
	void saveVersaInfo();
	void restoreVersaInfo();
	void restoreVersaWidgetUi();
protected:
	void resizeEvent(QResizeEvent *event) override;
	
signals:
	void textCodecGroupChanged(const QByteArray &codeText);
public slots:
	void on_btnMin_clicked();
	void on_btnMax_clicked();
	void on_btnExit_clicked();
private:
	void timeout();
	void configMenuBar();
	QPointer<QWidget> pMainVersaWidget = nullptr;
	QPointer<QWidget> pVersaWidget = nullptr;
	QPointer<VersaAppBar> pVersaAppBar = nullptr;
	QPointer<VersaTabBar> pVersaTabBar = nullptr;
	// QPointer<VersaStateBar> pVersaStateBar = nullptr;
	QPointer<VersaDockWidget> pVersaDockWidget = nullptr;
	QPointer<VersaSideWidget> pVersaSideWidget = nullptr;
	QPointer<VersaLicenseWidget> pVersaLicenseWidget = nullptr;
	QPointer<VersaAboutApp> pVersaAboutAppWidget = nullptr;
	QTimer refreshTimer;
	bool isInitReady = false;
};

#endif // WIDGET_H
