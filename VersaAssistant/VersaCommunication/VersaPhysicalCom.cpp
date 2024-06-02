//
// Created by vSeasky on 2023/9/15.
//

#include "VersaPhysicalCom.h"
#include "VersaMessageBox.h"
#include <QTimer>
#include <QPushButton>
#include <QSerialPort>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

VersaPhysicalCom *pVersaPhysicalComAddr = nullptr;
QThread *pVersaPhysicalComThreadAddr = nullptr;

const char *ipAddrString = "\n[%1:%2]";
const char *timeStampStringTx = "\n[hh:mm:ss.zzz.zzz]Tx:";
const char *timeStampStringRx = "\n[hh:mm:ss.zzz.zzz]Rx:";

VersaPhysicalCom *VersaPhysicalComGetFun()
{
	if (pVersaPhysicalComAddr == nullptr)
	{
		pVersaPhysicalComThreadAddr = new QThread();
		pVersaPhysicalComAddr = new VersaPhysicalCom;
		pVersaPhysicalComAddr->moveToThread(pVersaPhysicalComThreadAddr);
		pVersaPhysicalComThreadAddr->start();
		// 初始化子通信组件，跨线程初始化，以让整个通信模块在同一个线程
		QMetaObject::invokeMethod(VersaPhysicalPort, "startThread", Qt::QueuedConnection);
		return pVersaPhysicalComAddr;
	}
	else
	{
		return pVersaPhysicalComAddr;
	}
}

VersaPhysicalCom *VersaPhysicalComFreeFun()
{
	if (pVersaPhysicalComAddr != nullptr)
	{
		pVersaPhysicalComThreadAddr->quit();
		pVersaPhysicalComThreadAddr->wait();
		pVersaPhysicalComAddr->deleteLater();
		pVersaPhysicalComThreadAddr->deleteLater();
		pVersaPhysicalComAddr = nullptr;
		pVersaPhysicalComThreadAddr = nullptr;
		return nullptr;
	}
}

VersaPhysicalCom::VersaPhysicalCom(QObject *parent) :
		QObject(parent)
{
	// 注册 QByteArray 类型
	qRegisterMetaType<QByteArray>("QByteArray");
	qRegisterMetaType<QByteArray>("QByteArray&");
	qRegisterMetaType<QString>("QString&");
}

VersaPhysicalCom::~VersaPhysicalCom()
{
	mVersaPhysicalReady = false;
	if (!pVersaTimerCheck.isNull())
	{
		pVersaTimerCheck->stop();
	}
	
	if (!pVersaSerialPort.isNull())
	{
		pVersaSerialPort->deleteLater();
		pVersaSerialPort = nullptr;
	}
	
	if (!pVersaTcpServer.isNull())
	{
		pVersaTcpServer->deleteLater();
		pVersaTcpServer = nullptr;
	}
	
	if (!pVersaTcpSocket.isNull())
	{
		pVersaTcpSocket->deleteLater();
		pVersaTcpSocket = nullptr;
	}
	
	if (!pVersaUdpSocket.isNull())
	{
		pVersaUdpSocket->deleteLater();
		pVersaUdpSocket = nullptr;
	}
	
	if (!pVersaLibusb.isNull())
	{
		pVersaLibusb->deleteLater();
		pVersaLibusb = nullptr;
	}
	
	if (!pVersaTimerCheck.isNull())
	{
		pVersaTimerCheck->deleteLater();
		pVersaTimerCheck = nullptr;
	}
	
	
	pProtocolVersaTx = nullptr;
	pProtocolVersaRx = nullptr;
}

void VersaPhysicalCom::enableReady(bool ready)
{
	mVersaPhysicalReady = ready;
}

void VersaPhysicalCom::saveReceiveData(void)
{
	// 未就绪
	if (mVersaPhysicalReady == false)
	{
		return;
	}
	
	if (toggleOpenState == false)
	{
		QString dirpath = QFileDialog::getSaveFileName(nullptr, QStringLiteral("保存原始数据"),
		                                               qApp->applicationDirPath()+"/receivedata.dat",
		                                               QString(tr("*.dat")));
		if (dirpath != NULL)
		{
			QFile file(dirpath);
			//方式：Append为追加，WriteOnly，ReadOnly
			if (!file.open(QIODevice::WriteOnly))
			{
				emit newQMessageBox("提示", "无法创建文件！");
				return;
			}
			else
			{
				QTextStream saveStream(&file);
				saveStream << mVersaReadData;
				file.close();
			}
		}
	}
	else
	{
		emit newQMessageBox("提示", "请先关闭通信接口！");
		return;
	}
}

void VersaPhysicalCom::readReceiveFileData(void)
{
	// 未就绪
	if (mVersaPhysicalReady == false)
	{
		return;
	}
	
	if (toggleOpenState == false)
	{
		QString dirpath = QFileDialog::getOpenFileName(nullptr, QStringLiteral("导入原始数据"),
		                                               qApp->applicationDirPath()+"/receivedata.dat",
		                                               QString(tr("*.dat")));
		if (dirpath != NULL)
		{
			QFile file(dirpath);
			if (!file.open(QIODevice::ReadOnly))
			{
				emit newQMessageBox("提示", "无法打开该文件！");
				return;
			}
			else
			{
				QTextStream readStream(&file);
				QByteArray byteArray; // 创建 QByteArray 对象
				int index = mVersaProtocolIndex;
				toggleOpenState = true;
				mVersaProtocolIndex = eProtocolNone;
				while (!readStream.device()->atEnd())
				{
					byteArray.clear();
					byteArray = readStream.device()->read(1024);
					readData(byteArray);
				}
				toggleOpenState = false;
				mVersaProtocolIndex = index;
				file.close();
			}
		}
	}
	else
	{
		emit newQMessageBox("提示", "请先关闭通信接口！");
		return;
	}
}

void VersaPhysicalCom::startThread()
{
	pVersaSerialPort = new VersaSerialPort;
	pVersaTcpServer = new VersaTcpServer;
	pVersaTcpSocket = new VersaTcpSocket;
	pVersaUdpSocket = new VersaUdpSocket;
	pVersaLibusb = new VersaLibusb;
	pVersaTimerCheck = new QTimer;
	
	pVersaLibusb->start();
	
	pVersaSerialPort->moveToThread(this->thread());
	pVersaTcpServer->moveToThread(this->thread());
	pVersaTcpSocket->moveToThread(this->thread());
	pVersaUdpSocket->moveToThread(this->thread());
	pVersaLibusb->moveToThread(this->thread());
	
	pVersaTimerCheck->setInterval(250);
	pVersaTimerCheck->setTimerType(Qt::PreciseTimer);
	pVersaTimerCheck->stop();
	
	connect(pVersaTimerCheck,
	        &QTimer::timeout,
	        this,
	        &VersaPhysicalCom::physicaDeviceCheck,
	        Qt::QueuedConnection);
	
	pVersaTimerCheck->start();
}

void VersaPhysicalCom::toggleOpenReleased()
{
	// 未就绪
	if (mVersaPhysicalReady == false)
	{
		return;
	}
	// 打开的过程才设置
	if (toggleOpenState == false)
	{
		mVersaPhysicaIndex = mVersaReadySetPhysicaIndex;
	}
	toggleOpenPhysicalCom();
}

void VersaPhysicalCom::protocolEnableChanged(bool enable)
{
	if (enable)
	{
		mVersaProtocolIndex = eProtocolVersa;
	}
	else
	{
		mVersaProtocolIndex = eProtocolNone;
	}
}

void VersaPhysicalCom::setPhysicaIndex(int index)
{
	mVersaReadySetPhysicaIndex = index;
}

void VersaPhysicalCom::setSerialIndex(int index)
{
	if ((index < pVersaSerialPort->indexDevices.size()) && (index >= 0))
	{
		pVersaSerialPort->portName = pVersaSerialPort->indexDevices[index];
		pVersaSerialPort->mSerialPortStruct.setSerialPortName(pVersaSerialPort->portName);
	}
}

void VersaPhysicalCom::setSerialBaudRate(int baudRate)
{
	pVersaSerialPort->mSerialPortStruct.setSerialBaudRate(baudRate);
}

void VersaPhysicalCom::setSerialStopBits(QSerialPort::StopBits stopBits)
{
	pVersaSerialPort->mSerialPortStruct.setSerialStopBits(stopBits);
}

void VersaPhysicalCom::setSerialDataBits(QSerialPort::DataBits dataBits)
{
	pVersaSerialPort->mSerialPortStruct.setSerialDataBits(dataBits);
}

void VersaPhysicalCom::setSerialParrity(QSerialPort::Parity Parrity)
{
	pVersaSerialPort->mSerialPortStruct.setSerialParrity(Parrity);
}

void VersaPhysicalCom::setSerialFlowControl(QSerialPort::FlowControl flowControl)
{
	pVersaSerialPort->mSerialPortStruct.setSerialFlowControl(flowControl);
}

void VersaPhysicalCom::setProtocolTxStruct(protocol_struct *protocol)
{
	pProtocolVersaTx = protocol;
}

void VersaPhysicalCom::setProtocolRxStruct(protocol_struct *protocol)
{
	pProtocolVersaRx = protocol;
}


void VersaPhysicalCom::setInternetIndex(int index)
{
	mVersaInternetIndex = index;
}

void VersaPhysicalCom::setInternetAddress(QString Address)
{
	mAddress = Address;
}

void VersaPhysicalCom::setInternetPort(quint16 Port)
{
	mPort = Port;
}

void VersaPhysicalCom::setLibusbIndex(int index)
{
	if ((index < pVersaLibusb->nameDevices.size()) && (index >= 0))
	{
		pVersaLibusb->portName = pVersaLibusb->nameDevices[index];
		pVersaLibusb->portIndex = index;
	}
}

void VersaPhysicalCom::disconnectAll()
{
	QObject::disconnect(mVersaSerialRead);
	QObject::disconnect(mVersaTcpServerRead);
	QObject::disconnect(mVersaTcpSocketRead);
	QObject::disconnect(mVersaUdpSocketRead);
	QObject::disconnect(mVersaLibUsbRead);
	QObject::disconnect(mVersaTcpServerNew);
}

void VersaPhysicalCom::physicaDeviceCheck()
{
	// 未就绪
	if (mVersaPhysicalReady == false)
	{
		return;
	}
	
	if (toggleOpenState == false)
	{
		// 获取串口设备信息
		pVersaSerialPort->refreshListSerial();
		{
			if (pVersaSerialPort->nameDevices != lastSerialListName)
			{
				// 如果上一次使用的设备未拔出，则继续使用上一次的设备
				int index = 0;
				for (uint16_t iSerial = 0; iSerial < pVersaSerialPort->indexDevices.size(); iSerial++)
				{
					if (pVersaSerialPort->portName == pVersaSerialPort->indexDevices[iSerial])
					{
						index = iSerial;
					}
				}
				SerialListName = pVersaSerialPort->nameDevices;
				emit physicaSerialChanged(SerialListName, index);
			}
			lastSerialListName = pVersaSerialPort->nameDevices;
		}
		
		switch (mVersaInternetIndex)
		{
			case 0:
			{
				InternetListName = pVersaTcpServer->refreshListTcpServer();
				break;
			}
			case 1:
			{
				InternetListName = pVersaTcpSocket->refreshListTcpSocket();
				break;
			}
			case 2:
			{
				InternetListName = pVersaUdpSocket->refreshListUdpSocket();
				break;
			}
		}
		
		if (InternetListName.size() > 0)
		{
			if (lastInternetListName != InternetListName)
			{
				int index = 0;
				for (uint16_t iInternet = 0; iInternet < InternetListName.size(); iInternet++)
				{
					if (mAddress == InternetListName[iInternet])
					{
						index = iInternet;
					}
					emit physicaInternetChanged(InternetListName, index);
				}
				lastInternetListName = InternetListName;
			}
		}
		
		// 获取USB设备信息
		pVersaLibusb->refreshLibusbDevice();
		{
			if (pVersaLibusb->nameDevices != lastUsbHidListName)
			{
				// 如果上一次使用的设备未拔出，则继续使用上一次的设备
				UsbHidListName = pVersaLibusb->nameDevices;
				emit physicaLibusbChanged(UsbHidListName, 0);
			}
			lastUsbHidListName = pVersaLibusb->nameDevices;
		}
	}
	else
	{
		// 计算接收速率
		{
			double timeElapsed = ((double) (readElapsedTimer.elapsed())) / 1000.0;
			if (timeElapsed > 3.0)
			{
				readCnt += (0) * (1.0f / 1024.0f);
				readSpeed = ((readCnt - readLastCnt) / timeElapsed);
				readLastCnt = readCnt;
				if ((readMaxSpeed > 0) && (mVersaPhysicaIndex == 0))
				{
					readUtilization = readSpeed / readMaxSpeed;
				}
				readElapsedTimer.restart();
			}
		}
		// 计算发送速度
		{
			double timeElapsed = ((double) (sendElapsedTimer.elapsed())) / 1000.0;
			if (timeElapsed > 3.0)
			{
				sendCnt += (0) * (1.0f / 1024.0f);
				sendSpeed = ((sendCnt - sendLastCnt) / timeElapsed);
				sendLastCnt = sendCnt;
				if ((sendMaxSpeed > 0) && (mVersaPhysicaIndex == 0))
				{
					sendUtilization = sendSpeed / sendMaxSpeed;
				}
				sendElapsedTimer.restart();
			}
		}
	}
}

void VersaPhysicalCom::setTimestampEnabel(bool setValue)
{
	enableTimestamp = setValue;
}

void VersaPhysicalCom::setShowSendTextEnabel(bool setValue)
{
	enableShowSendText = setValue;
}

void VersaPhysicalCom::setShowIpAddrEnaabl(bool setValue)
{
	enableShowIpAddr = setValue;
}

/*!
 * 准备发送数据
 * @param address
 * @param port
 * @param data
 */
void VersaPhysicalCom::protocolNoneWrite(const QString &address, quint16 port, const QByteArray &data)
{
	if ((mVersaProtocolIndex == eProtocolNone) && (data.size() != 0))
	{
		startWrite(address, port, data);
	}
}

void VersaPhysicalCom::protocolVersaWrite(const QString &address, quint16 port)
{
	if (pProtocolVersaTx != nullptr)
	{
		make_protocol(pProtocolVersaTx);
		QByteArray writeData(reinterpret_cast<char *>(&pProtocolVersaTx->message.pData[0]),
		                     pProtocolVersaTx->message.data_len);
		// 124 字节只能收到 62字节，感觉是WIN的bug
		if (pProtocolVersaTx->message.data_len == 124)
		{
			writeData.append('\0');
		}
		if ((mVersaProtocolIndex == eProtocolVersa) &&
		    (writeData.size() > 0))
		{
			startWrite(address, port, writeData);
		}
	}
}

void VersaPhysicalCom::startWrite(const QString &address, quint16 port, const QByteArray &data)
{
	// 当前发送数据长度
	double currentSendCnt = data.size();
	if (currentSendCnt == 0)
	{
		return;
	}
	// 发送数据
	if (writeData(address, port, data))
	{
		// 计算发送速度
		{
			double timeSendElapsed = ((double) (sendElapsedTimer.elapsed())) / 1000.0;
			if (timeSendElapsed > 0)
			{
				sendCnt += (currentSendCnt) * (1.0f / 1024.0f);
				sendSpeed = ((sendCnt - sendLastCnt) / timeSendElapsed);
				sendLastCnt = sendCnt;
				if ((sendMaxSpeed > 0) && (mVersaPhysicaIndex == 0))
				{
					sendUtilization = sendSpeed / sendMaxSpeed;
				}
				sendElapsedTimer.restart();
			}
		}
		// 如果需要显示发送的数据
		if (enableShowSendText && (mVersaProtocolIndex == eProtocolNone))
		{
			QByteArray appendBuffer = data;
			if (enableShowIpAddr && (mVersaPhysicaIndex == ePhysicalInternet))
			{
				// 目标地址
				switch (mVersaInternetIndex)
				{
					case eInternetTcpServer:
					{
						appendBuffer.insert(0, QString(ipAddrString).arg(address).arg(port));
						break;
					}
					case eInternetTcpSocket:
					{
						appendBuffer.insert(0, QString(ipAddrString).arg(mAddress).arg(mPort));
						break;
					}
					case eInternetUdpSocket:
					{
						appendBuffer.insert(0, QString(ipAddrString).arg(address).arg(port));
						break;
					}
				}
			}
			if (enableTimestamp == true)
			{
				sendCurrentTime = QDateTime::currentDateTime();
				appendBuffer.replace("\n", sendCurrentTime.toString(timeStampStringTx).toUtf8());
			}
			mVersaReadData.append(appendBuffer);
			emit refreshReadBuffer();
		}
	}
}

void VersaPhysicalCom::clearAllBuffer()
{
	lastProtocolStartIndex = 0;
	startProtocolIndex = 0;
	mVersaReadData.clear();
	emit refreshReadBuffer();
}

void VersaPhysicalCom::clearStatistics()
{
	sendCnt = 0;
	readCnt = 0;
	sendLastCnt = 0;
	readLastCnt = 0;
	sendSpeed = 0;
	readSpeed = 0;
	sendMaxSpeed = 1;
	readMaxSpeed = 1;
	sendUtilization = 0;
	readUtilization = 0;
}

void VersaPhysicalCom::parseProtocol(QByteArray &data)
{
	int result;
	if (pProtocolVersaRx != nullptr)
	{
		if (lastProtocolStartIndex > 0)
		{
			data.remove(0, lastProtocolStartIndex);
			lastProtocolStartIndex = 0;
		}
		// 获取帧头索引
		startProtocolIndex = data.indexOf(PROTOCOL_HEAD_ID, lastProtocolStartIndex);
		if (startProtocolIndex != -1)
		{
			const char *pParseData = data.constData() + startProtocolIndex;
			int dataSize = data.size();
			
			int currentParseLength = qMin((int) (dataSize - startProtocolIndex),
			                              (int) (pProtocolVersaRx->message.max_data_len));
			
			if ((lastProtocolStartIndex <= startProtocolIndex) &&
			    (currentParseLength >= DATAS_MIN_SIZE))
			{
				memcpy(&pProtocolVersaRx->message.pData[0], pParseData + startProtocolIndex, currentParseLength);
				result = parse_protocol(pProtocolVersaRx, currentParseLength);
				switch (result)
				{
					// 解析成功
					case PROTOCOL_RESULT_OK:
					{
						// 记录成功的位置，下一次的位置在此之后
						lastProtocolStartIndex = startProtocolIndex + pProtocolVersaRx->message.data_len;
						emit refreshProtocolRx();
						// 还可以继续解析
						if (lastProtocolStartIndex + DATAS_MIN_SIZE < dataSize)
						{
							parseProtocol(data);
						}
						break;
					}
					case PROTOCOL_RESULT_ERR:
					{
						// 转到下一个帧头检测
						lastProtocolStartIndex = startProtocolIndex + 2;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_CHECK_HEAD_ERR:
					{
						// 转到下一个帧头检测
						lastProtocolStartIndex = startProtocolIndex + 2;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_CHECK_FRAME_ERR:
					{
						// 数据帧头检查成功，但是数据损坏，移动到帧头之后检查新数据包，损坏数据包丢弃
						lastProtocolStartIndex = startProtocolIndex + DATAS_OFFSET_ADDR;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_OUT_OF_MSG_LEN:
					{
						// 超出解析长度，数据包出错，解析下一个数据包
						lastProtocolStartIndex = startProtocolIndex + DATAS_OFFSET_ADDR;
						parseProtocol(data);
						break;
					}
					case PROTOCOL_RESULT_OUT_OF_DATA_LEN:
					{
						// 通过包头计算，还未收到完整的数据包，等待下一个数据包来即可。
						break;
					}
					default:
					{
						// 转到下一个帧头检测
						lastProtocolStartIndex = startProtocolIndex + 2;
						parseProtocol(data);
					}
				}
			}
		}
	}
}

void VersaPhysicalCom::toggleOpenPhysicalCom()
{
	switch (mVersaPhysicaIndex)
	{
		case ePhysicalSerial:
		{
			toggleOpenPhysicalSerial();
			break;
		}
		case ePhysicalInternet:
		{
			switch (mVersaInternetIndex)
			{
				case eInternetTcpServer:
				{
					toggleOpenPhysicalTcpServer();
					break;
				}
				case eInternetTcpSocket:
				{
					toggleOpenPhysicalTcpSocket();
					
					break;
				}
				case eInternetUdpSocket:
				{
					toggleOpenPhysicalUdpSocket();
					break;
				}
			}
			break;
		}
		case ePhysicalLibusb:
		{
			toggleOpenPhysicalLibusbHid();
			break;
		}
	}
	emit refreshOpenState(toggleOpenState);
}

/*!
 * 发送数据接口
 * @param address
 * @param port
 * @param data
 * @return
 */
bool VersaPhysicalCom::writeData(const QString &address, quint16 port, const QByteArray &data)
{
	if (toggleOpenState == false)
	{
		return false;
	}
	
	switch (mVersaPhysicaIndex)
	{
		case ePhysicalSerial:
		{
			return writePhysicalSerial(data);
			break;
		}
		case ePhysicalInternet:
		{
			switch (mVersaInternetIndex)
			{
				case eInternetTcpServer:
				{
					return writePhysicalTcpServer(QHostAddress(address), port, data);
					break;
				}
				case eInternetTcpSocket:
				{
					return writePhysicalTcpSocket(data);
					break;
				}
				case eInternetUdpSocket:
				{
					return writePhysicalUdpSocket(QHostAddress(address), port, data);
					break;
				}
			}
			break;
		}
		case ePhysicalLibusb:
		{
			return writePhysicalLibusbHid(data);
			break;
		}
	}
}

/*!
 * 读取数据接口
 * @param data
 */
void VersaPhysicalCom::readData(QByteArray &data)
{
	double currentReadCnt = data.size();
	if ((currentReadCnt == 0) || (toggleOpenState == false))
	{
		return;
	}
	
	if (enableTimestamp == true)
	{
		readCurrentTime = QDateTime::currentDateTime();
		data.replace("\n", readCurrentTime.toString(timeStampStringRx).toUtf8());
	}
	mVersaReadData.append(data);
	// 计算接收速率
	{
		double timeReadElapsed = ((double) (readElapsedTimer.elapsed())) / 1000.0;
		if (timeReadElapsed > 0)
		{
			readCnt += (currentReadCnt) * (1.0f / 1024.0f);//  kbyte
			readSpeed = ((readCnt - readLastCnt) / timeReadElapsed);//kbyte/s
			readLastCnt = readCnt;
			if ((readMaxSpeed > 0) && (mVersaPhysicaIndex == 0))
			{
				readUtilization = readSpeed / readMaxSpeed;
			}
			readElapsedTimer.restart();
		}
	}
	
	switch (mVersaProtocolIndex)
	{
		case eProtocolNone:
		{
			emit refreshReadBuffer();
			break;
		}
		case eProtocolVersa:
		{
			parseProtocol(mVersaReadData);
			break;
		}
	}
}

bool VersaPhysicalCom::toggleOpenPhysicalSerial()
{
	if (toggleOpenState == false)
	{
		// 1.断开所有连接
		disconnectAll();
		toggleOpenState = pVersaSerialPort->open();
		if (toggleOpenState == true)
		{
			// 2.设置信号与槽
			mVersaSerialRead =
					connect(&pVersaSerialPort->mSerialPort, &QSerialPort::readyRead, [=]()
					{
						QByteArray mReadByteArray = pVersaSerialPort->mSerialPort.readAll();
						readData(mReadByteArray);
					});
			
			// 3.计算最高速率
			{
				sendMaxSpeed = (((double) pVersaSerialPort->mSerialPortStruct.mSerialBaudRate) *
				                (pVersaSerialPort->mSerialPortStruct.mSerialDataBits /
				                 (1 + pVersaSerialPort->mSerialPortStruct.mSerialDataBits +
				                  (1 + 0.5 * pVersaSerialPort->mSerialPortStruct.mSerialStopBits) +
				                  bool(pVersaSerialPort->mSerialPortStruct.mSerialParrity)))) / (8.0 * 1024);
				readMaxSpeed = sendMaxSpeed;
			}
			
			readElapsedTimer.start();
		}
	}
	else
	{
		pVersaSerialPort->close();
		toggleOpenState = false;
		disconnectAll();
	}
}

bool VersaPhysicalCom::writePhysicalSerial(const QByteArray &data)
{
	qint64 result = -1;
	result = pVersaSerialPort->mSerialPort.write(data);
	if (result != -1)
	{
		return true;
	}
	return false;
}

bool VersaPhysicalCom::toggleOpenPhysicalTcpServer()
{
	if (toggleOpenState == false)
	{
		// 1.断开所有连接
		disconnectAll();
		
		pVersaTcpServer->setAddress(mAddress);
		pVersaTcpServer->setPort(mPort);
		toggleOpenState = pVersaTcpServer->openServer();
		if (toggleOpenState == true)
		{
			mVersaTcpServerRead =
					connect(pVersaTcpServer, &VersaTcpServer::readyRead, this,
					        [=](QTcpSocket *socket)
					        {
						        // 使能了ip地址显示
						        QByteArray mReadByteArray = socket->readAll();
						        if (enableShowIpAddr && (mVersaPhysicaIndex == ePhysicalInternet))
						        {
							        mReadByteArray.insert(0, QString(ipAddrString)
									        .arg(socket->peerAddress().toString())
									        .arg(socket->peerPort()).toUtf8());
						        }
						        readData(mReadByteArray);
					        });
			
			mVersaTcpServerNew =
					connect(pVersaTcpServer, &VersaTcpServer::newConnectSignals, [=](QTcpSocket *socket)
					{
						if (socket != nullptr)
						{
							emit newConnectPeerAddress(socket->peerAddress().toString());
							emit newConnectPeerPort(socket->peerPort());
						}
					});
			readElapsedTimer.start();
		}
	}
	else
	{
		pVersaTcpServer->closeServer();
		toggleOpenState = false;
		disconnectAll();
	}
}

bool VersaPhysicalCom::writePhysicalTcpServer(const QHostAddress &address, quint16 port, const QByteArray &data)
{
	QTcpSocket *sendSocket = pVersaTcpServer->findServerSocket(address, port);
	if (sendSocket != nullptr)
	{
		qint64 result = -1;
		result = sendSocket->write(data);
		if (result != -1)
		{
			return true;
		}
	}
	// 发送失败
	return false;
}

bool VersaPhysicalCom::toggleOpenPhysicalTcpSocket()
{
	if (toggleOpenState == false)
	{
		// 1.断开所有连接
		disconnectAll();
		pVersaTcpSocket->setAddress(mAddress);
		pVersaTcpSocket->setPort(mPort);
		toggleOpenState = pVersaTcpSocket->openSocket();
		if (toggleOpenState == true)
		{
			// 2.设置信号与槽
			mVersaTcpSocketRead =
					connect(pVersaTcpSocket, &QTcpSocket::readyRead, [=]()
					{
						QByteArray mReadByteArray = pVersaTcpSocket->readAll();
						if (enableShowIpAddr && (mVersaPhysicaIndex == ePhysicalInternet))
						{
							mReadByteArray.insert(0, QString(ipAddrString)
									.arg(pVersaTcpSocket->peerAddress().toString())
									.arg(pVersaTcpSocket->peerPort()).toUtf8());
						}
						readData(mReadByteArray);
					});
			readElapsedTimer.start();
		}
	}
	else
	{
		pVersaTcpSocket->closeSocket();
		toggleOpenState = false;
		disconnectAll();
	}
}

bool VersaPhysicalCom::writePhysicalTcpSocket(const QByteArray &data)
{
	if (pVersaTcpSocket->isOpen())
	{
		qint64 result = -1;
		result = pVersaTcpSocket->write(data);
		if (result != -1)
		{
			return true;
		}
	}
	return false;
}

bool VersaPhysicalCom::toggleOpenPhysicalUdpSocket()
{
	if (toggleOpenState == false)
	{
		// 1.断开所有连接
		disconnectAll();
		pVersaUdpSocket->setAddress(mAddress);
		pVersaUdpSocket->setPort(mPort);
		toggleOpenState = pVersaUdpSocket->openSocket();
		if (toggleOpenState == true)
		{
			// 2.设置信号与槽
			mVersaUdpSocketRead =
					connect(pVersaUdpSocket, &QUdpSocket::readyRead, [=]()
					{
						QByteArray mReadByteArray;
						QHostAddress senderAddress;
						quint16 senderPort;
						// 读取数据报
						mReadByteArray.resize(pVersaUdpSocket->pendingDatagramSize());
						pVersaUdpSocket->readDatagram(mReadByteArray.data(),
						                              mReadByteArray.size(),
						                              &senderAddress, &senderPort);
						if (enableShowIpAddr && (mVersaPhysicaIndex == ePhysicalInternet))
						{
							mReadByteArray
									.insert(0, QString(ipAddrString)
											.arg(senderAddress.toString())
											.arg(senderPort).toUtf8());
						}
						readData(mReadByteArray);
					});
			readElapsedTimer.start();
		}
	}
	else
	{
		pVersaUdpSocket->closeSocket();
		toggleOpenState = false;
		disconnectAll();
	}
}

bool VersaPhysicalCom::writePhysicalUdpSocket(const QHostAddress &address, quint16 port, const QByteArray &data)
{
	if (pVersaUdpSocket->isOpen())
	{
		qint64 result = -1;
		result = pVersaUdpSocket->writeDatagram(data, address, port);
		if (result != -1)
		{
			return true;
		}
	}
	return false;
}

/*!
 * usb hid打开函数
 * @return
 */
bool VersaPhysicalCom::toggleOpenPhysicalLibusbHid()
{
	if (toggleOpenState == false)
	{
		// 断开所有连接
		disconnectAll();
		readElapsedTimer.start();
		toggleOpenState = pVersaLibusb->openLibusbHid();
		if (toggleOpenState == true)
		{
			mVersaLibUsbRead =
					connect(pVersaLibusb, &VersaLibusb::readyRead, this, [=]()
					        {
						        QByteArray readBuffer = pVersaLibusb->readAll();
						        readData(readBuffer);
					        },
					        Qt::QueuedConnection);
		}
	}
	else
	{
		pVersaLibusb->closeLibusbHid();
		toggleOpenState = false;
		disconnectAll();
	}
}

/*!
 * usb hid 写入函数
 * @param data
 * @return
 */
bool VersaPhysicalCom::writePhysicalLibusbHid(const QByteArray &data)
{
	if (pVersaLibusb->isOpen())
	{
		return pVersaLibusb->writeBuffer(data);;
	}
	else
	{
		return false;
	}
}
