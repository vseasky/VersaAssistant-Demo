#ifndef VERSA_ASSISTANT_VERSAPHYSICALCOM_H
#define VERSA_ASSISTANT_VERSAPHYSICALCOM_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QElapsedTimer>
#include <QTimer>
#include <QDateTime>
#include <QPointer>
#include "VersaSerialPort.h"
#include "VersaTcpServer.h"
#include "VersaTcpSocket.h"
#include "VersaUdpSocket.h"
#include "VersaLibusb.h"
#include "bsp_protocol.h"

class VersaPhysicalCom : public QObject
{
Q_OBJECT

public:
	VersaPhysicalCom(QObject *parent = nullptr);
	~VersaPhysicalCom();
	void enableReady(bool ready);
	enum
	{
		ePhysicalSerial = 0, ePhysicalInternet = 1, ePhysicalLibusb = 2
	};
	enum
	{
		eInternetTcpServer = 0, eInternetTcpSocket = 1, eInternetUdpSocket = 2
	};
	enum
	{
		eProtocolNone = 0, eProtocolVersa = 1
	};
	
	QByteArray mVersaReadData;//接收的原始数据
	
	double sendCnt = 0;
	double readCnt = 0;
	double sendLastCnt = 0;
	double readLastCnt = 0;
	double sendSpeed = 0;
	double readSpeed = 0;
	double sendMaxSpeed = 1;
	double readMaxSpeed = 1;
	double sendUtilization = 0;
	double readUtilization = 0;

protected:

signals:
	
	void newQMessageBox(const QString &tittle, const QString &message);
	void refreshOpenState(bool open);//更新打开的状态
	
	void refreshSendBuffer();//更新发送数据
	void refreshReadBuffer();//更新接收数据
	void refreshProtocolRx();//更新协议数据
	
	void physicaLibusbChanged(const QStringList &listString, int index);//libusb更改
	void physicaSerialChanged(const QStringList &listString, int index);//串口更改
	void physicaInternetChanged(const QStringList &listString, int index);//网络接口更改
	
	
	void newConnectPeerAddress(QString peerAddress);
	void newConnectPeerPort(quint16 port);

public slots:
	void saveReceiveData(void);
	void readReceiveFileData(void);
	void startThread();
	void toggleOpenReleased();
	void disconnectAll();
	
	// 设置模式
	void setPhysicaIndex(int index);
	
	// 设置串口配置
	void setSerialIndex(int index);
	void setSerialBaudRate(int baudRate);
	void setSerialStopBits(QSerialPort::StopBits stopBits);
	void setSerialDataBits(QSerialPort::DataBits dataBits);
	void setSerialParrity(QSerialPort::Parity Parrity);
	void setSerialFlowControl(QSerialPort::FlowControl flowControl);
	
	void setInternetIndex(int index);
	void setInternetAddress(QString Address);
	void setInternetPort(quint16 Port);
	
	void setLibusbIndex(int index);
	
	// 设置协议数据
	void setProtocolTxStruct(protocol_struct *protocol);
	void setProtocolRxStruct(protocol_struct *protocol);
	
	// 扫描设备信息
	void physicaDeviceCheck();
	
	// 使能协议
	void protocolEnableChanged(bool enable);
	// 使能定时
	void setTimestampEnabel(bool setValue);
	// 使能发送显示
	void setShowSendTextEnabel(bool setValue);
	// 使能显示IP地址
	void setShowIpAddrEnaabl(bool setValue);
	
	// 原始数据发送接口
	void protocolNoneWrite(const QString &address, quint16 port, const QByteArray &data);
	// 协议数据发送接口
	void protocolVersaWrite(const QString &address, quint16 port);
	// 清空数据
	void clearAllBuffer();
	// 清空统计
	void clearStatistics();
private:
	// 切换打开状态
	void toggleOpenPhysicalCom();
	// 数据发送接口
	void startWrite(const QString &address, quint16 port, const QByteArray &data);
	// 发送数据
	bool writeData(const QString &address, quint16 port, const QByteArray &data);
	// 读取数据
	void readData(QByteArray &data);
	// 解析协议
	void parseProtocol(QByteArray &data);
	
	bool toggleOpenPhysicalSerial();
	bool writePhysicalSerial(const QByteArray &data);
	
	bool toggleOpenPhysicalTcpServer();
	bool writePhysicalTcpServer(const QHostAddress &address, quint16 port, const QByteArray &data);
	
	bool toggleOpenPhysicalTcpSocket();
	bool writePhysicalTcpSocket(const QByteArray &data);
	
	bool toggleOpenPhysicalUdpSocket();
	bool writePhysicalUdpSocket(const QHostAddress &address, quint16 port, const QByteArray &data);
	
	bool toggleOpenPhysicalLibusbHid();
	bool writePhysicalLibusbHid(const QByteArray &data);
	
	QMetaObject::Connection mVersaSerialRead;
	QMetaObject::Connection mVersaTcpServerRead;
	QMetaObject::Connection mVersaTcpSocketRead;
	QMetaObject::Connection mVersaUdpSocketRead;
	QMetaObject::Connection mVersaLibUsbRead;
	QMetaObject::Connection mVersaTcpServerNew;
	
	QPointer<VersaSerialPort> pVersaSerialPort;
	QPointer<VersaTcpServer> pVersaTcpServer;
	QPointer<VersaTcpSocket> pVersaTcpSocket;
	QPointer<VersaUdpSocket> pVersaUdpSocket;
	QPointer<VersaLibusb> pVersaLibusb;
	QPointer<QTimer> pVersaTimerCheck;
	
	bool mVersaPhysicalReady = false;
	
	QString mAddress;
	quint16 mPort = 32401;
	
	QDateTime sendCurrentTime;
	QDateTime readCurrentTime;
	QElapsedTimer sendElapsedTimer;
	QElapsedTimer readElapsedTimer;
	
	bool enableTimestamp = false;
	bool enableShowSendText = false;
	bool enableShowIpAddr = false;
	
	QStringList lastSerialListName;
	QStringList lastUsbHidListName;
	QStringList lastInternetListName;
	QStringList SerialListName;
	QStringList UsbHidListName;
	QStringList InternetListName;
	
	int mVersaInternetIndex = 0;
	int mVersaPhysicaIndex = 0;
	int mVersaProtocolIndex = 0;
	
	protocol_struct *pProtocolVersaTx = nullptr;
	protocol_struct *pProtocolVersaRx = nullptr;
	
	int mVersaReadySetPhysicaIndex = 0;
	bool toggleOpenState = false;
	
	int lastProtocolStartIndex = 0;
	int startProtocolIndex = 0;
};

VersaPhysicalCom *VersaPhysicalComGetFun();//获取通信接口
VersaPhysicalCom *VersaPhysicalComFreeFun();//释放通信接口


#define VersaPhysicalPort  VersaPhysicalComGetFun()
#define VersaPhysicalPortFree VersaPhysicalComFreeFun()

#endif //VERSA_ASSISTANT_VERSAPHYSICALCOM_H
