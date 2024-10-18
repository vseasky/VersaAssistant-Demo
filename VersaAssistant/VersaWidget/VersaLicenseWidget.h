/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaLicenseWidget.h
 * @Description  : 
 */

#ifndef VERSADEBUGASSISTANTPROJECT_VERSALICENSEWIDGET_H
#define VERSADEBUGASSISTANTPROJECT_VERSALICENSEWIDGET_H

#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QNetworkReply>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCloseEvent>
#include <QPointer>

class VersaLicenseWidget : public QWidget
{
Q_OBJECT
public:
	QString LicenseKey = "";
	enum requestLicenseState
	{
		requestNull
		, requestCheckLicense
		, requestCheckLicenseResult
		, requestActivateLicense
		, requestActivateLicenseResult
		, requestDeactivateLicense
		, requestDeactivateLicenseResult
	};
	VersaLicenseWidget(QWidget *parent = nullptr, const QString license = "");
	~VersaLicenseWidget();
	QString getCpuName();
	QString getCpuCore();
	QString getCpuProcessors();
	QString getCpuProcessorid();
	QString getBaseboardSerialnumber();
	QString getBiosSerialnumber();
	QString getBaseboardUuid();
	QString getDiskSerialnumber();
	QString getInfo(const QString &cmd);
public slots:
	void checkLicense(const QString &license);
	void activateLicense(const QString &license, const QString &registered_domain, const QString &item_reference);
	void deactivateLicense(const QString &license, const QString &registered_domain, const QString &item_reference);
	void onSendHttpsRequest(QString url);
	void toggleAuthorize(bool isauth);
private:
	QTableWidget tableWidget;
	QNetworkAccessManager manager;
	QNetworkRequest request;
	QSslConfiguration config;
	
	QPointer<QLabel> license_key_label = nullptr;
	QPointer<QLabel> status_label = nullptr;
	QPointer<QLabel> max_allowed_domains_label = nullptr;
	QPointer<QLabel> email_label = nullptr;
	QPointer<QLabel> date_created_label = nullptr;
	QPointer<QLabel> date_renewed_label = nullptr;
	QPointer<QLabel> date_expiry_label = nullptr;
	QPointer<QLabel> product_ref_label = nullptr;
	QPointer<QLabel> first_name_label = nullptr;
	QPointer<QLabel> last_name_label = nullptr;
	QPointer<QLabel> company_name_label = nullptr;
	QPointer<QLabel> txn_id_label = nullptr;
	QPointer<QLabel> cpuName = nullptr;
	QPointer<QLabel> cpuSerialNumber = nullptr;
	QPointer<QLineEdit> pLicenseKeyEdit = nullptr;
	QPointer<QPushButton> authorizeState = nullptr;
	QPointer<QPushButton> authorize = nullptr;
	
	bool isActivate = false;
	requestLicenseState requestState = requestNull;
	QTimer mTimerCheck;
protected:
	void closeEvent(QCloseEvent *event) override
	{
		event->ignore(); // 忽略关闭事件
		Q_UNUSED(event);
		hide(); // 隐藏窗口
	}
};


#endif //VERSADEBUGASSISTANTPROJECT_VERSALICENSEWIDGET_H
