/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaSerial\VersaSerialPortStruct.h
 * @Description  : 
 */
#ifndef VERSA_ASSISTANT_VERSASERIALPORTSTRUCT_H
#define VERSA_ASSISTANT_VERSASERIALPORTSTRUCT_H

#include <QString>
#include <QSerialPort>

typedef struct
{                           //描述:        eg
	QString SerialName;     //名称:        "COMx"
	qint16 ProductCode;     //产品编号:     "29987"
	QString SyestemPosition;//系统位置:     "\\\\.\\COM10"
	QString SerialNumStr;   //序列号字符串: ""
	QString DescribeStr;    //描述字符串:   "USB-SERIAL CH340"
	QString Manufacturer;   //制造商:      "wch.cn"
	QString SupplierCode;   //供应商编码:   "6790"
} VersaSerialInfoStruct;

class VersaSerialPortStruct : public QObject
{
Q_OBJECT
public:
	QString mSerialPortName;                    //串口名称
	int mSerialBaudRate;                        //串口波特率
	QSerialPort::StopBits mSerialStopBits;      //串口停止位
	QSerialPort::DataBits mSerialDataBits;      //串口数据位
	QSerialPort::Parity mSerialParrity;         //设置奇偶校验
	QSerialPort::FlowControl mSerialFlowControl;//数据流控
	
	void setSerialPortName(QString portName);
	void setSerialBaudRate(int baudRate);
	void setSerialStopBits(QSerialPort::StopBits stopBits);
	void setSerialDataBits(QSerialPort::DataBits dataBits);
	void setSerialParrity(QSerialPort::Parity Parrity);
	void setSerialFlowControl(QSerialPort::FlowControl flowControl);
signals:
	void serialPortNameChanged(QString portName);
	void serialBaudRateChanged(int baudRate);
	void serialStopBitsChanged(QSerialPort::StopBits stopBits);
	void serialDataBitsChanged(QSerialPort::DataBits dataBits);
	void serialParrityChanged(QSerialPort::Parity Parrity);
	void serialFlowControlChanged(QSerialPort::FlowControl flowControl);
	
};


#endif //VERSA_ASSISTANT_VERSASERIALPORTSTRUCT_H
