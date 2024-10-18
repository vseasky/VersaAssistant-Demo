/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaSideWidget.cpp
 * @Description  : 
 */
#include "VersaSideWidget.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QComboBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QAbstractItemView>
#include <QListView>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <VersaPhysicalCom.h>
#include <QDebug>

void VersaSideWidget::refreshOpenState(bool state)
{
	pPhysicalSerialWidget->setEnabled(!state);
	pPhysicalInternetWidget->setEnabled(!state);
	pPhysicalLibusbWidget->setEnabled(!state);
}

void VersaSideWidget::setStackedLayout(int index)
{
	static int lastIndex = -1;
	static bool showEnable = false;
	if (index < pStackedLayout->count())
	{
		if (lastIndex != index)
		{
			pStackedLayout->setCurrentIndex(index);
			showSidebar();
			showEnable = true;
		}
		else
		{
			if (showEnable == true)
			{
				hideSidebar();
				showEnable = false;
			}
			else
			{
				showSidebar();
				showEnable = true;
			}
		}
		lastIndex = index;
	}
}

void VersaSideWidget::setPhysicaLibusbCombox(const QStringList &listString, int index)
{
	if ((pPhysicaLibusbCombox != nullptr) && (index >= 0))
	{
		pPhysicaLibusbCombox->clear();
		for (uint16_t iUsb = 0; iUsb < listString.size(); iUsb++)
		{
			pPhysicaLibusbCombox->addItem(listString[iUsb]);
		}
		pPhysicaLibusbCombox->setCurrentIndex(index);
	}
}

void VersaSideWidget::setPhysicaSerialCombox(const QStringList &listString, int index)
{
	if ((pPhysicaSerialCombox != nullptr) && (index >= 0))
	{
		pPhysicaSerialCombox->clear();
		for (uint16_t iSerial = 0; iSerial < listString.size(); iSerial++)
		{
			pPhysicaSerialCombox->addItem(listString[iSerial]);
		}
		pPhysicaSerialCombox->setCurrentIndex(index);
	}
}

void VersaSideWidget::setPhysicaInternetCombox(const QStringList &listString, int index)
{
	if ((pPhysicaInternetComboBox != nullptr) && (index >= 0))
	{
		pPhysicaInternetComboBox->clear();
		for (uint16_t iInternet = 0; iInternet < listString.size(); iInternet++)
		{
			pPhysicaInternetComboBox->addItem(listString[iInternet]);
		}
		pPhysicaInternetComboBox->setCurrentIndex(index);
		emit internetAddress(pPhysicaInternetComboBox->currentText());
	}
}

VersaSideWidget::VersaSideWidget(const QByteArray &buffer, QWidget *parent) :
		QWidget(parent),
		mSidebarWidth(300),
		mSidebarHidden(false)
{
	textEditBuffer.resize(8);
	restoreDataBuffer(buffer);
	
	pStackedLayout = new QStackedLayout(this);
	pStackedLayout->setSpacing(0);
	pStackedLayout->setContentsMargins(0, 0, 0, 0);
	
	this->setMinimumWidth(mSidebarWidth);
	this->setGeometry(-mSidebarWidth, 0, mSidebarWidth, parent->height());
	pParentWidget = parent;
	this->setAutoFillBackground(true);
	this->setMinimumWidth(mSidebarWidth);
	
	creatPhysicalSerialWidget();
	creatPhysicalInternetWidget();
	creatPhysicalUsbWidget();
	pStackedLayout->setCurrentIndex(1);
	hideSidebar();
	
	this->setLayout(pStackedLayout);
	
	hideSidebar();
}

VersaSideWidget::~VersaSideWidget()
{
	if (!pStackedLayout.isNull())
	{
		delete pStackedLayout;
		pStackedLayout = nullptr;
	}
	
	if (!pPhysicaSerialCombox.isNull())
	{
		delete pPhysicaSerialCombox;
		pPhysicaSerialCombox = nullptr;
	}
	
	if (!pPhysicaLibusbCombox.isNull())
	{
		delete pPhysicaLibusbCombox;
		pPhysicaLibusbCombox = nullptr;
	}
	
	if (!pPhysicaInternetComboBox.isNull())
	{
		delete pPhysicaInternetComboBox;
		pPhysicaInternetComboBox = nullptr;
	}
	
	if (!pPhysicalSerialWidget.isNull())
	{
		delete pPhysicalSerialWidget;
		pPhysicalSerialWidget = nullptr;
	}
	
	if (!pPhysicalInternetWidget.isNull())
	{
		delete pPhysicalInternetWidget;
		pPhysicalInternetWidget = nullptr;
	}
	
	if (!pPhysicalLibusbWidget.isNull())
	{
		delete pPhysicalLibusbWidget;
		pPhysicalLibusbWidget = nullptr;
	}
	
	pParentWidget = nullptr;
}

/*!
 * 创建串口设置菜单
 */
void VersaSideWidget::creatPhysicalSerialWidget()
{
	pPhysicalSerialWidget = new QWidget(this);
	pStackedLayout->addWidget(pPhysicalSerialWidget);
	QVBoxLayout *pSerialVBoxLayout = new QVBoxLayout(pPhysicalSerialWidget);
	pSerialVBoxLayout->setSpacing(mSpacing);
	pSerialVBoxLayout->setContentsMargins(mSpacing, mSpacing, mSpacing, mSpacing);
	pPhysicalSerialWidget->setLayout(pSerialVBoxLayout);
	
	QPushButton *pSerialButton = new QPushButton(pPhysicalSerialWidget);
	pSerialButton->setMinimumWidth(mSidebarWidth - mSpacing * 2);
	pSerialButton->setEnabled(false);
	pSerialButton->setText(tr("串口配置"));
	pSerialVBoxLayout->addWidget(pSerialButton);
	for (uint8_t iInfoIndex = 0; iInfoIndex < 6; iInfoIndex++)
	{
		QWidget *mSerialInfo = new QWidget(this);
		QHBoxLayout *pSerialHBoxLayout = new QHBoxLayout(mSerialInfo);
		pSerialHBoxLayout->setSpacing(8);
		pSerialHBoxLayout->setContentsMargins(0, 0, 0, 0);
		
		QPushButton *pSerialButtonName = new QPushButton(pPhysicalSerialWidget);
		pSerialButtonName->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pSerialButtonName->setEnabled(false);
		QComboBox *pSerialComboBox = new QComboBox(pPhysicalSerialWidget);
		pSerialComboBox->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		
		pSerialHBoxLayout->addWidget(pSerialButtonName);
		pSerialHBoxLayout->addWidget(pSerialComboBox);
		
		
		mSerialInfo->setLayout(pSerialHBoxLayout);
		pSerialVBoxLayout->addWidget(mSerialInfo);
		// 需要此设置，否则css不生效
		pSerialComboBox->setView(new QListView(pSerialComboBox));
		
		switch (iInfoIndex)
		{
			case 0:
			{
				pSerialButtonName->setText(tr("端口选择"));
				connect(pSerialComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this,
				        [=](const QString &str)
				        {
					        emit serialIndexChanged(pSerialComboBox->currentIndex());
					        textEditBuffer[iInfoIndex] = pSerialComboBox->currentText().toUtf8();
				        });
				pSerialComboBox->setCurrentIndex(0);
				pSerialComboBox->setStyleSheet("QComboBox QAbstractItemView {\n"
				                               "    min-width: 768px;\n"
				                               "}");
				pPhysicaSerialCombox = pSerialComboBox;
				
				VersaPhysicalPort->setSerialIndex(pSerialComboBox->currentIndex());
			}
				break;
			case 1:
			{
				pSerialButtonName->setText(tr("波特率"));
				// 添加选项到字符串列表中
				QStringList serialOptions;
				serialOptions << "2000000"
				              << "1382400"
				              << "921600"
				              << "460800"
				              << "256000"
				              << "230400"
				              << "128000"
				              << "115200"
				              << "76800"
				              << "57600"
				              << "43000"
				              << "38400"
				              << "19200"
				              << "14400"
				              << "9600"
				              << "4800"
				              << "2400"
				              << "1200";
				
				// 将数据模型设置为 QComboBox 的模型
				// pSerialComboBox->addItems(serialOptions);
				pSerialComboBox->setEditable(true);
				pSerialComboBox->addItems(serialOptions);
				QCompleter *pCompleter = new QCompleter(serialOptions, pSerialComboBox);
				pCompleter->setFilterMode(Qt::MatchContains);
				pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
				pCompleter->setCompletionMode(QCompleter::PopupCompletion);
				pSerialComboBox->setCompleter(pCompleter);
				
				QRegExp regExp("^[0-9]*[1-9][0-9]*$");
				pSerialComboBox->setValidator(new QRegExpValidator(regExp, pSerialComboBox));
				connect(pSerialComboBox, &QComboBox::currentTextChanged, this, [=](const QString &string)
				{
					bool ok = false;
					int BaudRate = pSerialComboBox->currentText().toInt(&ok);
					if ((BaudRate > 0) && ok)
					{
						emit serialBaudRateChanged(pSerialComboBox->currentText().toInt());
						textEditBuffer[iInfoIndex] = pSerialComboBox->currentText().toUtf8();
					}
				});
				
				pSerialComboBox->setCurrentIndex(0);
				for (uint32_t iCount = 0; iCount < pSerialComboBox->count(); iCount++)
				{
					if (textEditBuffer[iInfoIndex] == pSerialComboBox->itemText(iCount).toUtf8())
					{
						pSerialComboBox->setCurrentIndex(iCount);
					}
				}
				
				VersaPhysicalPort->setSerialBaudRate(pSerialComboBox->currentText().toInt());
			}
				break;
			case 2:
			{
				pSerialButtonName->setText(tr("停止位"));
				pSerialComboBox->addItem("1", QSerialPort::StopBits(QSerialPort::OneStop));
				pSerialComboBox->addItem("1.5", QSerialPort::StopBits(QSerialPort::OneAndHalfStop));
				pSerialComboBox->addItem("2", QSerialPort::StopBits(QSerialPort::TwoStop));
				connect(pSerialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
				{
					emit serialStopBitsChanged(QSerialPort::StopBits(pSerialComboBox->currentData().toInt()));
					textEditBuffer[iInfoIndex] = pSerialComboBox->currentText().toUtf8();
				});
				pSerialComboBox->setCurrentIndex(0);
				for (uint32_t iCount = 0; iCount < pSerialComboBox->count(); iCount++)
				{
					if (textEditBuffer[iInfoIndex] == pSerialComboBox->itemText(iCount).toUtf8())
					{
						pSerialComboBox->setCurrentIndex(iCount);
					}
				}
				
				VersaPhysicalPort->setSerialStopBits(
						QSerialPort::StopBits(pSerialComboBox->currentData().toInt()));
			}
				break;
			case 3:
			{
				pSerialButtonName->setText(tr("数据位"));
				pSerialComboBox->addItem("8", QSerialPort::DataBits(QSerialPort::Data8));
				pSerialComboBox->addItem("7", QSerialPort::DataBits(QSerialPort::Data7));
				pSerialComboBox->addItem("6", QSerialPort::DataBits(QSerialPort::Data6));
				pSerialComboBox->addItem("5", QSerialPort::DataBits(QSerialPort::Data5));
				connect(pSerialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
				{
					emit serialDataBitsChanged(QSerialPort::DataBits(pSerialComboBox->currentData().toInt()));
					textEditBuffer[iInfoIndex] = pSerialComboBox->currentText().toUtf8();
				});
				pSerialComboBox->setCurrentIndex(0);
				for (uint32_t iCount = 0; iCount < pSerialComboBox->count(); iCount++)
				{
					if (textEditBuffer[iInfoIndex] == pSerialComboBox->itemText(iCount).toUtf8())
					{
						pSerialComboBox->setCurrentIndex(iCount);
					}
				}
				
				VersaPhysicalPort->setSerialDataBits(
						QSerialPort::DataBits(pSerialComboBox->currentData().toInt()));
			}
				break;
			case 4:
			{
				QStringList listParity =
						{
								tr("无"),
								tr("偶校验"),
								tr("无奇校验")
							
						};
				pSerialButtonName->setText(tr("奇偶校验"));
				pSerialComboBox->addItem(tr("无"), QSerialPort::Parity(QSerialPort::NoParity));
				pSerialComboBox->addItem(tr("偶校验"),
				                         QSerialPort::Parity(QSerialPort::EvenParity));
				pSerialComboBox->addItem(tr("无奇校验"),
				                         QSerialPort::Parity(QSerialPort::OddParity));
				connect(pSerialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
				{
					emit serialParrityChanged(QSerialPort::Parity(pSerialComboBox->currentData().toInt()));
					textEditBuffer[iInfoIndex] = pSerialComboBox->currentText().toUtf8();
				});
				pSerialComboBox->setCurrentIndex(0);
				for (uint32_t iCount = 0; iCount < pSerialComboBox->count(); iCount++)
				{
					if (textEditBuffer[iInfoIndex] == pSerialComboBox->itemText(iCount).toUtf8())
					{
						pSerialComboBox->setCurrentIndex(iCount);
					}
				}
				
				VersaPhysicalPort->setSerialParrity(
						QSerialPort::Parity(pSerialComboBox->currentData().toInt()));
			}
				break;
			case 5:
			{
				pSerialButtonName->setText(tr("数据流控"));
				pSerialComboBox->addItem("NoFlowControl", QSerialPort::Parity(QSerialPort::NoFlowControl));
				pSerialComboBox->addItem("RTS/CTS", QSerialPort::Parity(QSerialPort::HardwareControl));
				pSerialComboBox->addItem("XON/XOFF", QSerialPort::Parity(QSerialPort::SoftwareControl));
				connect(pSerialComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
				{
					emit serialFlowControlChanged(QSerialPort::FlowControl(pSerialComboBox->currentData().toInt()));
					textEditBuffer[iInfoIndex] = pSerialComboBox->currentText().toUtf8();
				});
				pSerialComboBox->setCurrentIndex(0);
				for (uint32_t iCount = 0; iCount < pSerialComboBox->count(); iCount++)
				{
					if (textEditBuffer[iInfoIndex] == pSerialComboBox->itemText(iCount).toUtf8())
					{
						pSerialComboBox->setCurrentIndex(iCount);
					}
				}
				
				VersaPhysicalPort->setSerialFlowControl(
						QSerialPort::FlowControl(pSerialComboBox->currentData().toInt()));
			}
				break;
		}
	}
	pSerialVBoxLayout->addStretch(1);
}

void VersaSideWidget::creatPhysicalInternetWidget()
{
	pPhysicalInternetWidget = new QWidget(this);
	pStackedLayout->addWidget(pPhysicalInternetWidget);
	QVBoxLayout *pInternetQVBoxLayout = new QVBoxLayout(pPhysicalInternetWidget);
	pInternetQVBoxLayout->setSpacing(mSpacing);
	pInternetQVBoxLayout->setContentsMargins(mSpacing, mSpacing, mSpacing, mSpacing);
	pPhysicalInternetWidget->setLayout(pInternetQVBoxLayout);
	
	QPushButton *pInternetButton = new QPushButton(pPhysicalInternetWidget);
	pInternetButton->setMinimumWidth(mSidebarWidth - mSpacing * 2);
	pInternetButton->setEnabled(false);
	pInternetButton->setText(tr("网络配置"));
	pInternetQVBoxLayout->addWidget(pInternetButton);
	{
		QWidget *mInternetInfo = new QWidget(this);
		QHBoxLayout *pInternetModeHBoxLayout = new QHBoxLayout(mInternetInfo);
		pInternetModeHBoxLayout->setSpacing(mSpacing);
		pInternetModeHBoxLayout->setContentsMargins(0, 0, 0, 0);
		
		QPushButton *pInternetModeButton = new QPushButton(pPhysicalInternetWidget);
		pInternetModeButton->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pInternetModeButton->setEnabled(false);
		QComboBox *pInternetModeComboBox = new QComboBox(pPhysicalInternetWidget);
		pInternetModeComboBox->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		
		pInternetModeHBoxLayout->addWidget(pInternetModeButton);
		pInternetModeHBoxLayout->addWidget(pInternetModeComboBox);
		
		
		mInternetInfo->setLayout(pInternetModeHBoxLayout);
		pInternetQVBoxLayout->addWidget(mInternetInfo);
		// 需要此设置，否则css不生效
		pInternetModeComboBox->setView(new QListView(pInternetModeComboBox));
		
		pInternetModeButton->setText(tr("协议类型"));
		pInternetModeComboBox->addItem("Tcp Server", int(0));
		pInternetModeComboBox->addItem("Tcp Client", int(1));
		pInternetModeComboBox->addItem("UDP", int(2));
		
		QWidget *mIpInfo = new QWidget(this);
		QHBoxLayout *pIpHBoxLayout = new QHBoxLayout(mIpInfo);
		pIpHBoxLayout->setSpacing(8);
		pIpHBoxLayout->setContentsMargins(0, 0, 0, 0);
		
		QPushButton *pIpButtonName = new QPushButton(pPhysicalInternetWidget);
		pIpButtonName->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pIpButtonName->setEnabled(false);
		pPhysicaInternetComboBox = new QComboBox(pPhysicalInternetWidget);
		pPhysicaInternetComboBox->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		// 需要此设置，否则css不生效
		pPhysicaInternetComboBox->setView(new QListView(pInternetModeComboBox));
		pIpHBoxLayout->addWidget(pIpButtonName);
		pIpHBoxLayout->addWidget(pPhysicaInternetComboBox);
		
		mIpInfo->setLayout(pIpHBoxLayout);
		pInternetQVBoxLayout->addWidget(mIpInfo);
		
		pIpButtonName->setText(tr("服务器IP地址"));
		QRegExpValidator *validatorIpv4 = new QRegExpValidator(
				QRegExp("^(?:(?:\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.){3}(?:\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])$"));
		pPhysicaInternetComboBox->setValidator(validatorIpv4);
		
		connect(pPhysicaInternetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]()
		{
			emit internetAddress(pPhysicaInternetComboBox->currentText());
		});
		
		
		QWidget *mPortInfo = new QWidget(this);
		QHBoxLayout *pPortHBoxLayout = new QHBoxLayout(mPortInfo);
		pPortHBoxLayout->setSpacing(mSpacing);
		pPortHBoxLayout->setContentsMargins(0, 0, 0, 0);
		
		QPushButton *pPortButtonName = new QPushButton(pPhysicalInternetWidget);
		pPortButtonName->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pPortButtonName->setEnabled(false);
		QLineEdit *pPortQLineEdit = new QLineEdit(pPhysicalInternetWidget);
		pPortQLineEdit->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		
		pPortHBoxLayout->addWidget(pPortButtonName);
		pPortHBoxLayout->addWidget(pPortQLineEdit);
		
		mPortInfo->setLayout(pPortHBoxLayout);
		
		pInternetQVBoxLayout->addWidget(mPortInfo);
		
		pPortButtonName->setText(tr("服务器端口"));
		QRegExpValidator *validatorPort = new QRegExpValidator(
				QRegExp("^(?:[0-9]|[1-9][0-9]{1,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$"));
		pPortQLineEdit->setValidator(validatorPort);
		if (textEditBuffer[7] != "")
		{
			pPortQLineEdit->setText(QString::fromUtf8(textEditBuffer[7]));
		}
		else
		{
			pPortQLineEdit->setText("32401");
		}
		connect(pPortQLineEdit, &QLineEdit::textChanged, [=]()
		{
			emit internetPort(pPortQLineEdit->text().toInt());
			textEditBuffer[7] = pPortQLineEdit->text().toUtf8();
		});
		emit internetPort(pPortQLineEdit->text().toInt());
		
		connect(pInternetModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
		        this, [this, pIpButtonName, pPortButtonName](int index)
		        {
			        // 在这里处理选择变化
			        switch (index)
			        {
				        case 0:
				        {
					        pIpButtonName->setText(tr("本地IP地址"));
					        pPortButtonName->setText(tr("本地端口"));
				        }
					        break;
				        case 1:
				        {
					        pIpButtonName->setText(tr("服务器IP地址"));
					        pPortButtonName->setText(tr("服务器端口"));
				        }
					        break;
				        case 2:
				        {
					        pIpButtonName->setText(tr("本地IP地址"));
					        pPortButtonName->setText(tr("本地端口"));
				        }
					        break;
			        }
			        emit internetType(index);
		        });
	}
	pInternetQVBoxLayout->addStretch(1);
}

void VersaSideWidget::creatPhysicalUsbWidget()
{
	pPhysicalLibusbWidget = new QWidget(this);
	pStackedLayout->addWidget(pPhysicalLibusbWidget);
	QVBoxLayout *pPhysicalUsbLayout = new QVBoxLayout(pPhysicalLibusbWidget);
	pPhysicalUsbLayout->setSpacing(mSpacing);
	pPhysicalUsbLayout->setContentsMargins(mSpacing, mSpacing, mSpacing, mSpacing);
	pPhysicalLibusbWidget->setLayout(pPhysicalUsbLayout);
	
	QPushButton *pPhysicalUsbButton = new QPushButton(pPhysicalLibusbWidget);
	pPhysicalUsbButton->setMinimumWidth(mSidebarWidth - mSpacing * 2);
	pPhysicalUsbButton->setEnabled(false);
	pPhysicalUsbButton->setText(tr("USB配置"));
	pPhysicalUsbLayout->addWidget(pPhysicalUsbButton);
	{
		QWidget *mPhysicalUsbInfo = new QWidget(this);
		QHBoxLayout *pPhysicalUsbModeHBoxLayout = new QHBoxLayout(mPhysicalUsbInfo);
		pPhysicalUsbModeHBoxLayout->setSpacing(8);
		pPhysicalUsbModeHBoxLayout->setContentsMargins(0, 0, 0, 0);
		
		QPushButton *pPhysicalUsbModeButton = new QPushButton(pPhysicalLibusbWidget);
		pPhysicalUsbModeButton->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pPhysicalUsbModeButton->setEnabled(false);
		QComboBox *pInternetModeComboBox = new QComboBox(pPhysicalLibusbWidget);
		pInternetModeComboBox->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		
		pPhysicalUsbModeHBoxLayout->addWidget(pPhysicalUsbModeButton);
		pPhysicalUsbModeHBoxLayout->addWidget(pInternetModeComboBox);
		
		mPhysicalUsbInfo->setLayout(pPhysicalUsbModeHBoxLayout);
		pPhysicalUsbLayout->addWidget(mPhysicalUsbInfo);
		// 需要此设置，否则css不生效
		pInternetModeComboBox->setView(new QListView(pInternetModeComboBox));
		
		pPhysicalUsbModeButton->setText(tr("协议类型"));
		pInternetModeComboBox->addItem("HID", int(0));
		pInternetModeComboBox->addItem("WinUsb", int(1));
		pInternetModeComboBox->setEnabled(false);
		
		QWidget *mUsbInfo = new QWidget(this);
		QHBoxLayout *pUsbIndexHBoxLayout = new QHBoxLayout(mUsbInfo);
		pUsbIndexHBoxLayout->setSpacing(8);
		pUsbIndexHBoxLayout->setContentsMargins(0, 0, 0, 0);
		
		QPushButton *pUsbIndexButtonName = new QPushButton(pPhysicalLibusbWidget);
		pUsbIndexButtonName->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pUsbIndexButtonName->setEnabled(false);
		QComboBox *pUsbComBox = new QComboBox(pPhysicalLibusbWidget);
		pUsbComBox->setMinimumWidth(mSidebarWidth / 2 - mSpacing * 1.5);
		pUsbComBox->setView(new QListView(pUsbComBox));
		pUsbComBox->setStyleSheet("QComboBox QAbstractItemView {\n"
		                          "    min-width: 768px;\n"
		                          "}");
		
		connect(pUsbComBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this,
		        [=](const QString &str)
		        {
			        emit usbIndexChanged(pUsbComBox->currentIndex());
		        });
		
		pUsbIndexHBoxLayout->addWidget(pUsbIndexButtonName);
		pUsbIndexHBoxLayout->addWidget(pUsbComBox);
		
		mUsbInfo->setLayout(pUsbIndexHBoxLayout);
		pPhysicalUsbLayout->addWidget(mUsbInfo);
		
		pUsbIndexButtonName->setText(tr("USB设备"));
		pPhysicaLibusbCombox = pUsbComBox;
	}
	pPhysicalUsbLayout->addStretch(1);
}

void VersaSideWidget::resizeSidebar()
{
	if (mSidebarHidden == true)
	{
		this->setGeometry(0, 0, mSidebarWidth, pParentWidget->height());
	}
	else
	{
		this->setGeometry(-mSidebarWidth, 0, mSidebarWidth, pParentWidget->height());
	}
}

void VersaSideWidget::toggleSidebar()
{
	if (mSidebarHidden == false)
	{
		showSidebar();
	}
	else
	{
		hideSidebar();
	}
}

void VersaSideWidget::showSidebar()
{
	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(mSidebarWidth);
	QRect startRect(-mSidebarWidth, -pParentWidget->height(), mSidebarWidth, pParentWidget->height());
	QRect endRect(0, 0, mSidebarWidth, pParentWidget->height());
	animation->setStartValue(startRect);
	animation->setEndValue(endRect);
	animation->start();
	emit showSidebarStart();
	connect(animation, &QPropertyAnimation::finished, this, [=]()
	{
		emit showSidebarEnd();
	});
	mSidebarHidden = true;
}

void VersaSideWidget::hideSidebar()
{
	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(mSidebarWidth);
	QRect startRect(0, 0, mSidebarWidth, pParentWidget->height());
	QRect endRect(-mSidebarWidth, 0, mSidebarWidth, pParentWidget->height());
	animation->setStartValue(startRect);
	animation->setEndValue(endRect);
	animation->start();
	emit hideSidebarStart();
	connect(animation, &QPropertyAnimation::finished, this, [=]()
	{
		emit hideSidebarEnd();
	});
	mSidebarHidden = false;
}

QByteArray VersaSideWidget::getSaveDataBuffer()
{
	QByteArray mDataBuffer;
	QDataStream stream(&mDataBuffer, QIODevice::WriteOnly);
	
	QVector<int> dataLength;
	dataLength.resize(8);
	
	stream << dataLength;
	
	for (uint8_t iTextIndex = 0; iTextIndex < 8; iTextIndex++)
	{
		dataLength[iTextIndex] = textEditBuffer[iTextIndex].size();
		if (dataLength[iTextIndex] > 0)
		{
			stream << textEditBuffer[iTextIndex];
		}
	}
	
	return mDataBuffer;
}

void VersaSideWidget::restoreDataBuffer(const QByteArray &buffer)
{
	QDataStream stream(buffer);
	int bufferSize = buffer.size();
	int totalSize = 0;
	totalSize = sizeof(int) * 8;
	if (bufferSize >= totalSize)
	{
		QVector<int> dataLength;
		dataLength.resize(8);
		stream >> dataLength;
		
		for (uint8_t iTextIndex = 0; iTextIndex < 8; iTextIndex++)
		{
			totalSize += dataLength[iTextIndex];
			if ((dataLength[iTextIndex] > 0) && (bufferSize >= totalSize))
			{
				textEditBuffer[iTextIndex].resize(
						qMin(dataLength[iTextIndex], dataLength[iTextIndex] + bufferSize - totalSize));
				stream >> textEditBuffer[iTextIndex];
			}
		}
	}
}
