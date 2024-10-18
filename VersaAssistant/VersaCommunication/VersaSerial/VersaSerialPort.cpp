/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaSerial\VersaSerialPort.cpp
 * @Description  : 
 */
#include "VersaSerialPort.h"
#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>
#include <QDebug>

VersaSerialPort::VersaSerialPort(QObject *parent) : QObject(parent), mSerialPort(this)
{

}

VersaSerialPort::~VersaSerialPort(void)
{
	mSerialPort.close();
	if (pSerialComInfo != nullptr)
	{
		delete[] pSerialComInfo;
		pSerialComInfo = nullptr;
	}
};

void VersaSerialPort::refreshListSerial(void)
{
	qint32 SerialComCnt = 0;
	this->mSerialPortCnt = 0;
	
	if (this->pSerialComInfo != nullptr)
	{
		delete[] this->pSerialComInfo;
	}
	
	this->mSerialPortCnt = QSerialPortInfo::availablePorts().length();
	this->pSerialComInfo = new VersaSerialInfoStruct[this->mSerialPortCnt];
	SerialComCnt = 0;
	nameDevices.clear();
	indexDevices.clear();
	for (int i = 0; i < mSerialPortCnt; ++i)
	{
		const QSerialPortInfo port = QSerialPortInfo::availablePorts().at(i);
		this->pSerialComInfo[SerialComCnt].SerialName = port.portName();
		this->pSerialComInfo[SerialComCnt].ProductCode = port.productIdentifier();
		this->pSerialComInfo[SerialComCnt].SyestemPosition = port.systemLocation();
		this->pSerialComInfo[SerialComCnt].SerialNumStr = port.serialNumber();
		this->pSerialComInfo[SerialComCnt].DescribeStr = port.description();
		this->pSerialComInfo[SerialComCnt].Manufacturer = port.manufacturer();
		this->pSerialComInfo[SerialComCnt].SupplierCode = port.vendorIdentifier();
		QString portInfo(QString("%1-%2[%3]")
				                 .arg(port.manufacturer())
				                 .arg(port.description())
				                 .arg(port.portName()));
		nameDevices.append(portInfo);
		indexDevices.append(port.portName());
		SerialComCnt++;
	}
}


void VersaSerialPort::debugInfo()
{
	qDebug() << "Serial Port Name:" << mSerialPortStruct.mSerialPortName;
	qDebug() << "Baud Rate:" << mSerialPortStruct.mSerialBaudRate;
	qDebug() << "Stop Bits:" << mSerialPortStruct.mSerialStopBits;
	qDebug() << "Data Bits:" << mSerialPortStruct.mSerialDataBits;
	qDebug() << "Parity:" << mSerialPortStruct.mSerialParrity;
	qDebug() << "Flow Control:" << mSerialPortStruct.mSerialFlowControl;
}

bool VersaSerialPort::openSerialPort(QSerialPort *port, QIODevice::OpenMode mode)
{
	bool result = false;
	port->close();
	//设置串口名称
	port->setPortName(mSerialPortStruct.mSerialPortName);
	//设置串口波特率
	port->setBaudRate(mSerialPortStruct.mSerialBaudRate);
	//设置停止位
	port->setStopBits(mSerialPortStruct.mSerialStopBits);
	//设置数据位
	port->setDataBits(mSerialPortStruct.mSerialDataBits);
	//设置奇偶校验
	port->setParity(mSerialPortStruct.mSerialParrity);
	//设置数据流控
	port->setFlowControl(mSerialPortStruct.mSerialFlowControl);
	
	port->setReadBufferSize(1024);
	
	result = port->open(mode);
	return result;
}

void VersaSerialPort::openserial(bool enable)
{
	if (enable)
	{
		open();
	}
	else
	{
		close();
	}
};

//打开选中的串口设备
bool VersaSerialPort::open()
{
	// debugInfo();
	return openSerialPort(&mSerialPort, QIODevice::ReadWrite);
}

bool VersaSerialPort::close(void)
{
	mSerialPort.close();
}



