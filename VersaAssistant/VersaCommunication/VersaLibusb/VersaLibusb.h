/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaLibusb\VersaLibusb.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSALIBUSB_H
#define VERSA_ASSISTANT_VERSALIBUSB_H

#include <QObject>
#include <libusb.h>
#include <QMap>
#include <QThread>
#include <QMutex>

struct VersaLibUsbInfo
{
	QString mName;
	uint16_t indexDevice;
	uint8_t indexInterface;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t MaxPacketSizeOut;
	uint8_t ENDPOINT_OUT;
	uint16_t MaxPacketSizeIn;
	uint8_t ENDPOINT_IN;
	uint16_t bIntervalOut;
	uint16_t bIntervalIn;
};

class VersaLibusb : public QThread
{
Q_OBJECT
public:
	VersaLibusb(QObject *parent = nullptr);
	~VersaLibusb();
	void refreshLibusbDevice();
	bool openLibusbHid();
	void closeLibusbHid();
	bool isOpen();
	bool writeBuffer(const QByteArray &data);
	QByteArray readAll();
	QStringList nameDevices;//设备名称
	QString portName;
	int portIndex;
signals:
	void readyRead();
protected:
	void run() override;
private:
	QMutex mutex;
	QVector<VersaLibUsbInfo> listUsbInfo;//设备信息
	QByteArray readQBytBuffer;
	bool readingBuffer = false;
	libusb_context *libusbContext = nullptr;
	libusb_device **libusbDevices = nullptr;
	libusb_device *libusbDevice = nullptr;
	libusb_device_handle *libusbHandle = nullptr;
	bool libusbOpenState = false;
};


#endif //VERSA_ASSISTANT_VERSALIBUSB_H
