/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaLicenseWidget.cpp
 * @Description  : 
 */

#include "VersaLicenseWidget.h"
#include "VersaConfig.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QThread>
#include <QProcess>
#include <QPushButton>
#include <QDebug>


const QString VersaLicenseUrl = "https://www.liuwei.vin/wp?";
const QString secret_key = "846E097D0FEF0ACDFF4515A11B68F82A50CAF76AE079233AF6DF6BE07A0B5B3F";
const QString wmic_key = "C:\\Windows\\System32\\wbem\\WMIC.exe";

//获取cpu名称：wmic cpu get Name
//获取cpu核心数：wmic cpu get NumberOfCores
//获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
//查询cpu序列号：wmic cpu get processorid
//查询主板序列号：wmic baseboard get serialnumber
//查询BIOS序列号：wmic bios get serialnumber
//查看硬盘：wmic diskdrive get serialnumber
QString VersaLicenseWidget::getInfo(const QString &cmd)
{
	QProcess process;        //启动外部程序
	process.start(cmd);      //一体式启动，不分离，主程序退出则启动程序退出,使用close关闭
	//p.startDetached(cmd)  //分离式启动，主程序退出后，外部程序继续运行
	process.waitForFinished(-1);  //超时等待，设置为-1，直到执行完成
	QString result = QString::fromLocal8Bit(process.readAllStandardOutput());//读取运行结果
	QStringList list = cmd.split(" ");  //将cmd按空格拆分
	result = result.remove(list.last(), Qt::CaseInsensitive);//删除cmd序列中最后一个元素
	result = result.replace("\r", "");  //删除\r
	result = result.replace("\n", "");  //删除\n
	result = result.simplified();  //移除字符串两端的空白字符
	
	process.terminate(); // 强制终止进程
	process.waitForFinished(); // 等待进程终止
	process.close();
	return result;
}

//查询CPU型号
QString VersaLicenseWidget::getCpuName()
{
	return getInfo(wmic_key + " cpu get Name");
}

//查询CPU核心数
QString VersaLicenseWidget::getCpuCore()
{
	return getInfo(wmic_key + " cpu get NumberOfCores");
}

//查询CPU线程数
QString VersaLicenseWidget::getCpuProcessors()
{
	return getInfo(wmic_key + " cpu get NumberOfLogicalProcessors");
}

//查询CPU处理器标识符
QString VersaLicenseWidget::getCpuProcessorid()
{
	return getInfo(wmic_key + " cpu get processorid");
}

//查询主板序列号
QString VersaLicenseWidget::getBaseboardSerialnumber()
{
	return getInfo(wmic_key + " baseboard get serialnumber");
}

//查询BIOS序列号
QString VersaLicenseWidget::getBiosSerialnumber()
{
	return getInfo(wmic_key + " bios get serialnumber");
}

//查询主板唯一标识符
QString VersaLicenseWidget::getBaseboardUuid()
{
	return getInfo(wmic_key + " csproduct get uuid");
}

//查询硬盘标识符
QString VersaLicenseWidget::getDiskSerialnumber()
{
	return getInfo(wmic_key + " diskdrive get serialnumber");
}

VersaLicenseWidget::~VersaLicenseWidget()
{
	mTimerCheck.stop();
	// 判断指针是否为空，并释放内存
	if (!license_key_label.isNull())
	{
		delete license_key_label;
		license_key_label = nullptr;
	}
	
	if (!status_label.isNull())
	{
		delete status_label;
		status_label = nullptr;
	}
	
	if (!max_allowed_domains_label.isNull())
	{
		delete max_allowed_domains_label;
		max_allowed_domains_label = nullptr;
	}
	
	if (!email_label.isNull())
	{
		delete email_label;
		email_label = nullptr;
	}
	
	if (!date_created_label.isNull())
	{
		delete date_created_label;
		date_created_label = nullptr;
	}
	
	if (!date_renewed_label.isNull())
	{
		delete date_renewed_label;
		date_renewed_label = nullptr;
	}
	
	if (!date_expiry_label.isNull())
	{
		delete date_expiry_label;
		date_expiry_label = nullptr;
	}
	
	if (!product_ref_label.isNull())
	{
		delete product_ref_label;
		product_ref_label = nullptr;
	}
	
	if (!first_name_label.isNull())
	{
		delete first_name_label;
		first_name_label = nullptr;
	}
	
	if (!last_name_label.isNull())
	{
		delete last_name_label;
		last_name_label = nullptr;
	}
	
	if (!company_name_label.isNull())
	{
		delete company_name_label;
		company_name_label = nullptr;
	}
	
	if (!txn_id_label.isNull())
	{
		delete txn_id_label;
		txn_id_label = nullptr;
	}
	
	if (!cpuName.isNull())
	{
		delete cpuName;
		cpuName = nullptr;
	}
	
	if (!cpuSerialNumber.isNull())
	{
		delete cpuSerialNumber;
		cpuSerialNumber = nullptr;
	}
	
	if (!pLicenseKeyEdit.isNull())
	{
		delete pLicenseKeyEdit;
		pLicenseKeyEdit = nullptr;
	}
	
	if (!authorizeState.isNull())
	{
		delete authorizeState;
		authorizeState = nullptr;
	}
	
	if (!authorize.isNull())
	{
		delete authorize;
		authorize = nullptr;
	}
}

VersaLicenseWidget::VersaLicenseWidget(QWidget *parent, const QString license) :
		QWidget(parent, Qt::Dialog),
		LicenseKey(license),
		tableWidget(this)
{
	setMinimumSize(800, 1024);
	QVBoxLayout *layout = new QVBoxLayout(this);
	tableWidget.setColumnCount(2);
	tableWidget.setRowCount(16);
	// 隐藏水平表头
	tableWidget.horizontalHeader()->setVisible(false);
	// 隐藏垂直表头
	tableWidget.verticalHeader()->setVisible(false);
	layout->addWidget(&tableWidget);
	setLayout(layout);
	{
		tableWidget.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		// 固定行高
		tableWidget.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		tableWidget.verticalHeader()->setDefaultSectionSize(48);
		tableWidget.horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
		tableWidget.setColumnWidth(0, 200);
		int labelCount = 0;
		{
			tableWidget.setSpan(labelCount, 0, 1, 2);
			authorizeState = new QPushButton(&tableWidget);
			authorizeState->setEnabled(false);
			authorizeState->setCheckable(true);
			tableWidget.setCellWidget(labelCount, 0, authorizeState);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("产品名称"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			product_ref_label = new QLabel(&tableWidget);
			product_ref_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, product_ref_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("注册码"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			license_key_label = new QLabel(&tableWidget);
			license_key_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, license_key_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("订单号"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			txn_id_label = new QLabel(&tableWidget);
			txn_id_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, txn_id_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("邮箱"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			email_label = new QLabel(&tableWidget);
			email_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, email_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("状态"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			status_label = new QLabel(&tableWidget);
			status_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, status_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("创建日期"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			date_created_label = new QLabel(&tableWidget);
			date_created_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, date_created_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("激活日期"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			date_renewed_label = new QLabel(&tableWidget);
			date_renewed_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, date_renewed_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("到期日期"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			date_expiry_label = new QLabel(&tableWidget);
			date_expiry_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, date_expiry_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("公司名称"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			company_name_label = new QLabel(&tableWidget);
			company_name_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, company_name_label);
		}
		labelCount++;
		
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("用户名1"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			first_name_label = new QLabel(&tableWidget);
			first_name_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, first_name_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("用户名2"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			last_name_label = new QLabel(&tableWidget);
			last_name_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, last_name_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("授权数量"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			max_allowed_domains_label = new QLabel(&tableWidget);
			max_allowed_domains_label->setText("");
			tableWidget.setCellWidget(labelCount, 1, max_allowed_domains_label);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("计算机名称"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			cpuName = new QLabel(&tableWidget);
			cpuName->setText(getCpuName());
			tableWidget.setCellWidget(labelCount, 1, cpuName);
		}
		labelCount++;
		
		{
			QLabel *label = new QLabel(&tableWidget);
			label->setText(tr("CPU序列号"));
			label->setAlignment(Qt::AlignCenter);
			label->setFont(ThemeQFont("FontAppAbout"));
			tableWidget.setCellWidget(labelCount, 0, label);
			cpuSerialNumber = new QLabel(&tableWidget);
			cpuSerialNumber->setText(getCpuProcessorid());
			tableWidget.setCellWidget(labelCount, 1, cpuSerialNumber);
		}
		labelCount++;
		
		{
			authorize = new QPushButton(&tableWidget);
			authorize->setCheckable(true);
			toggleAuthorize(true);
			tableWidget.setCellWidget(labelCount, 0, authorize);
			authorize->setFont(ThemeQFont("FontAppAbout"));
			pLicenseKeyEdit = new QLineEdit(&tableWidget);
			pLicenseKeyEdit->setFont(ThemeQFont("FontAppAbout"));
			pLicenseKeyEdit->setText(LicenseKey);
			tableWidget.setCellWidget(labelCount, 1, pLicenseKeyEdit);
		}
		labelCount++;
		
		license_key_label->setContentsMargins(12, 0, 12, 0);
		license_key_label->setFont(ThemeQFont("FontAppAbout"));
		
		status_label->setContentsMargins(12, 0, 12, 0);
		status_label->setFont(ThemeQFont("FontAppAbout"));
		
		max_allowed_domains_label->setContentsMargins(12, 0, 12, 0);
		max_allowed_domains_label->setFont(ThemeQFont("FontAppAbout"));
		
		email_label->setContentsMargins(12, 0, 12, 0);
		email_label->setFont(ThemeQFont("FontAppAbout"));
		
		date_created_label->setContentsMargins(12, 0, 12, 0);
		date_created_label->setFont(ThemeQFont("FontAppAbout"));
		
		date_renewed_label->setContentsMargins(12, 0, 12, 0);
		date_renewed_label->setFont(ThemeQFont("FontAppAbout"));
		
		date_expiry_label->setContentsMargins(12, 0, 12, 0);
		date_expiry_label->setFont(ThemeQFont("FontAppAbout"));
		
		product_ref_label->setContentsMargins(12, 0, 12, 0);
		product_ref_label->setFont(ThemeQFont("FontAppAbout"));
		
		first_name_label->setContentsMargins(12, 0, 12, 0);
		first_name_label->setFont(ThemeQFont("FontAppAbout"));
		
		last_name_label->setContentsMargins(12, 0, 12, 0);
		last_name_label->setFont(ThemeQFont("FontAppAbout"));
		
		company_name_label->setContentsMargins(12, 0, 12, 0);
		company_name_label->setFont(ThemeQFont("FontAppAbout"));
		
		txn_id_label->setContentsMargins(12, 0, 12, 0);
		txn_id_label->setFont(ThemeQFont("FontAppAbout"));
		
		cpuName->setContentsMargins(12, 0, 12, 0);
		cpuName->setFont(ThemeQFont("FontAppAbout"));
		
		cpuSerialNumber->setContentsMargins(12, 0, 12, 0);
		cpuSerialNumber->setFont(ThemeQFont("FontAppAbout"));
	}
	
	connect(authorize, &QPushButton::released, this, [=]()
	{
		authorize->setChecked(!authorize->isChecked());
		if (isActivate)
		{
			if (pLicenseKeyEdit->text() != "")
			{
				activateLicense(pLicenseKeyEdit->text(),
				                cpuSerialNumber->text(),
				                cpuName->text());
			}
		}
		else
		{
			if (license_key_label->text() != "")
			{
				deactivateLicense(license_key_label->text(),
				                  cpuSerialNumber->text(),
				                  cpuName->text());
			}
		}
	});
	
	
	QString version = QSslSocket::sslLibraryBuildVersionString();//"OpenSSL 1.1.1g  21 Apr 2020"
	bool support = QSslSocket::supportsSsl();
	config = request.sslConfiguration();
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);
	
	checkLicense(LicenseKey);
	
	mTimerCheck.setTimerType(Qt::PreciseTimer);
	mTimerCheck.setInterval(1000);
	
	connect(&mTimerCheck, &QTimer::timeout, this, [=]()
	{
		
		switch (requestState)
		{
			case requestCheckLicenseResult:
			{
				requestState = requestNull;
				break;
			}
			case requestActivateLicenseResult:
			{
				if (pLicenseKeyEdit != nullptr)
				{
					if (pLicenseKeyEdit->text() != "")
					{
						requestState = requestNull;
						checkLicense(pLicenseKeyEdit->text());
					}
				}
				break;
			}
			case requestDeactivateLicenseResult:
			{
				if (pLicenseKeyEdit != nullptr)
				{
					if (pLicenseKeyEdit->text() != "")
					{
						requestState = requestNull;
						checkLicense(pLicenseKeyEdit->text());
					}
				}
				break;
			}
		}
	});
	mTimerCheck.start();
	hide();
}

void VersaLicenseWidget::checkLicense(const QString &license)
{
	if (requestState != requestNull)
	{
		return;
	}
	requestState = requestCheckLicense;
	QString checkURL =
			VersaLicenseUrl +
			"secret_key=" + secret_key + "&" +
			"slm_action=slm_check&" +
			"license_key=" + license;
	onSendHttpsRequest(checkURL);
}

void VersaLicenseWidget::activateLicense(const QString &license, const QString &registered_domain,
                                         const QString &item_reference)
{
	if (requestState != requestNull)
	{
		return;
	}
	requestState = requestActivateLicense;
	QString checkURL =
			VersaLicenseUrl +
			"secret_key=" + secret_key + "&" +
			"slm_action=slm_activate&" +
			"license_key=" + license + "&" +
			"registered_domain=" + registered_domain + "&" +
			"item_reference" + item_reference;
	onSendHttpsRequest(checkURL);
}

void VersaLicenseWidget::deactivateLicense(const QString &license, const QString &registered_domain,
                                           const QString &item_reference)
{
	if (requestState != requestNull)
	{
		return;
	}
	requestState = requestDeactivateLicense;
	QString checkURL =
			VersaLicenseUrl +
			"secret_key=" + secret_key + "&" +
			"slm_action=slm_deactivate&" +
			"license_key=" + license + "&" +
			"registered_domain=" + registered_domain + "&" +
			"item_reference" + item_reference;
	onSendHttpsRequest(checkURL);
}

void VersaLicenseWidget::onSendHttpsRequest(QString url)
{
	// 发送https请求前准备工作;
	request.setUrl(QUrl(url));
	QNetworkReply *pReply = manager.get(request);
	connect(pReply, &QNetworkReply::finished,
	        this, [=]()
	        {
		        QNetworkReply *pReplay = qobject_cast<QNetworkReply *>(sender());
		        // 保存接受的数据;
		        QByteArray replyContent = pReplay->readAll();
		        QJsonObject jsonDoc = QJsonDocument::fromJson(replyContent).object();
		        
		        if (jsonDoc.isEmpty() || replyContent.isEmpty())
		        {
			        requestState = requestNull;
			        pReply->deleteLater();
		        }
		        switch (requestState)
		        {
			        case requestCheckLicense:
			        {
				        QString result_string = jsonDoc.value("result").toString();
				        int isActivatedLicense = false;
				        if (result_string == "success")
				        {
					        license_key_label->setText(jsonDoc.value("license_key").toString());
					        pLicenseKeyEdit->setText(jsonDoc.value("license_key").toString());
					        status_label->setText(jsonDoc.value("status").toString());
					        max_allowed_domains_label->setText(jsonDoc.value("max_allowed_domains").toString());
					        email_label->setText(jsonDoc.value("email").toString());
					        date_created_label->setText(jsonDoc.value("date_created").toString());
					        date_renewed_label->setText(jsonDoc.value("date_renewed").toString());
					        date_expiry_label->setText(jsonDoc.value("date_expiry").toString());
					        product_ref_label->setText(jsonDoc.value("product_ref").toString());
					        first_name_label->setText(jsonDoc.value("first_name").toString());
					        last_name_label->setText(jsonDoc.value("last_name").toString());
					        company_name_label->setText(jsonDoc.value("company_name").toString());
					        txn_id_label->setText(jsonDoc.value("txn_id").toString());
					        // 获取 "registered_domains" 字段
					        QJsonValue registeredDomainsValue = jsonDoc.value("registered_domains");
					        if (registeredDomainsValue.isArray())
					        {
						        QJsonArray registeredDomainsArray = registeredDomainsValue.toArray();
						        // 获取第一个元素
						        if (!registeredDomainsArray.isEmpty())
						        {
							        for (int iArray = 0; iArray < registeredDomainsArray.size(); iArray++)
							        {
								        QJsonObject registeredDomainObject =
										        registeredDomainsArray.at(iArray).toObject();
								        if (!registeredDomainObject.isEmpty())
								        {
									        if (registeredDomainObject.value("registered_domain") ==
									            cpuSerialNumber->text())
									        {
										        // 已经激活
										        isActivatedLicense = true;
										        LicenseKey = jsonDoc.value("license_key").toString();
										        break;
									        }
								        }
							        }
						        }
					        }
					        toggleAuthorize(!isActivatedLicense);
				        }
				        requestState = requestCheckLicenseResult;
				        break;
			        }
			        case requestActivateLicense:
			        {
				        QString result_string = jsonDoc["result"].toString();
				        // 激活成功
				        if (result_string == "success")
				        {
					        toggleAuthorize(false);
				        }
				        requestState = requestActivateLicenseResult;
				        break;
			        }
			        case requestDeactivateLicense:
			        {
				        QString result_string = jsonDoc["result"].toString();
				        // 激活成功
				        if (result_string == "success")
				        {
					        toggleAuthorize(true);
				        }
				        requestState = requestDeactivateLicenseResult;
				        break;
			        }
		        }
		        pReply->deleteLater();
	        });
	connect(pReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
	        this, [=](QNetworkReply::NetworkError errorCode)
	        {
		        pReply->deleteLater();
	        });
}

void VersaLicenseWidget::toggleAuthorize(bool isauth)
{
	if (authorize == nullptr)
	{
		return;
	}
	isActivate = isauth;
	if (isActivate)
	{
		authorizeState->setText(tr("未授权"));
		authorize->setText(tr("激活授权"));
	}
	else
	{
		authorizeState->setText(tr("已授权"));
		authorize->setText(tr("取消授权"));
	}
	authorizeState->setChecked(isActivate);
	authorize->setChecked(!isActivate);
}