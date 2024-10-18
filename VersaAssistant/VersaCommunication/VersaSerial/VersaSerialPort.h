/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaSerial\VersaSerialPort.h
 * @Description  : 
 */
#ifndef VSERIALPORT_H
#define VSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "VersaSerialPortStruct.h"
#include <QThread>

class VersaSerialPort : public QObject
{
Q_OBJECT
public:
	explicit VersaSerialPort(QObject *parent = nullptr);
	~VersaSerialPort(void);
	VersaSerialInfoStruct *pSerialComInfo = nullptr;
	VersaSerialPortStruct mSerialPortStruct;
	qint32 mSerialPortCnt;
	QSerialPort mSerialPort;
	QStringList nameDevices;//设备名称
	QStringList indexDevices;//设备索引
	QString portName;
	void write(QByteArray &byteArray);
public slots:
	void refreshListSerial(void);
	void openserial(bool enable);
	bool open(void);
	bool close(void);
signals:

private:
	void debugInfo();
	bool openSerialPort(QSerialPort *port, QIODevice::OpenMode mode);
};

#endif // VSERIALPORT
