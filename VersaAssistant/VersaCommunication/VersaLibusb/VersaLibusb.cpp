/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaLibusb\VersaLibusb.cpp
 * @Description  : 
 */

#include "VersaLibusb.h"
#include <QByteArray>
#include <QtGlobal>
#include <QString>
#include <QDebug>

VersaLibusb::VersaLibusb(QObject *parent) : QThread(parent)
{

}

VersaLibusb::~VersaLibusb()
{
	closeLibusbHid();
	quit();
	wait();
}

// 打开usb设备
bool VersaLibusb::openLibusbHid()
{
	int result = -1;
	libusbOpenState = false;
	
	closeLibusbHid();
	
	// libusb初始化
	result = libusb_init(&libusbContext);
	
	if (result < 0)
	{
		closeLibusbHid();
		return false;
	}
	
	// 获取设备列表
	libusb_get_device_list(libusbContext, &libusbDevices);
	
	// 获取所选设备
	libusbDevice = libusbDevices[listUsbInfo[portIndex].indexDevice];
	
	// 打开设备
	result = libusb_open(libusbDevice, &libusbHandle);
	
	if (result < 0)
	{
		closeLibusbHid();
		return false;
	}
	
	// 选择接口
	result = libusb_claim_interface(libusbHandle, listUsbInfo[portIndex].indexInterface);
	
	if (result < 0)
	{
		closeLibusbHid();
		return false;
	}
	libusbOpenState = true;
	return true;
}

void VersaLibusb::closeLibusbHid()
{
	libusbOpenState = false;
	if (libusbHandle != nullptr)
	{
		libusb_close(libusbHandle);
		libusbHandle = nullptr;
		libusbDevice = nullptr;
	}
	libusb_free_device_list(libusbDevices, 1);
	libusb_exit(libusbContext);
}

bool VersaLibusb::isOpen()
{
	return libusbOpenState;
}

void VersaLibusb::run()
{
	int timeout = 1000; // 超时时间（毫秒）
	int bytesRead;
	int result = -1;
	// 数据读取循环
	while (!isInterruptionRequested())
	{
		if (libusbOpenState == true)
		{
			result = -1;
			bytesRead = -1;
			unsigned char *buffer = new unsigned char[listUsbInfo[portIndex].MaxPacketSizeIn];
			memset(buffer, 0, listUsbInfo[portIndex].MaxPacketSizeIn);
			result = libusb_interrupt_transfer(libusbHandle,
			                                   listUsbInfo[portIndex].ENDPOINT_IN,
			                                   buffer,
			                                   listUsbInfo[portIndex].MaxPacketSizeIn,
			                                   &bytesRead, timeout);
			if ((result == 0) && (bytesRead > 0))
			{
				QByteArray readQByteArray(reinterpret_cast<char *>(buffer), listUsbInfo[portIndex].MaxPacketSizeIn);
				if (readingBuffer == true)
				{
					readQBytBuffer.clear();
					mutex.lock();
					readQBytBuffer.append(readQByteArray);
					mutex.unlock();
				}
				else
				{
					mutex.lock();
					readQBytBuffer.append(readQByteArray);
					mutex.unlock();
				}
				emit readyRead();
			}
			delete[] buffer; // 释放分配的内存
		}
	}
}

QByteArray VersaLibusb::readAll()
{
	mutex.lock();
	readingBuffer = true;
	mutex.unlock();
	return readQBytBuffer;
}

bool VersaLibusb::writeBuffer(const QByteArray &data)
{
	int timeout = 1000; // 超时时间（毫秒）
	int bytesSend;
	int result = -1;
	unsigned char *buffer = new unsigned char[listUsbInfo[portIndex].MaxPacketSizeOut];
	int offset = 0;     // 初始化偏移量
	while (offset < data.size())
	{
		int length = qMin(data.size() - offset, static_cast<int>(listUsbInfo[portIndex].MaxPacketSizeOut));
		const char *databuffer = data.constData(); // 获取 QByteArray 的数据指针
		memset(buffer, 0, listUsbInfo[portIndex].MaxPacketSizeOut);
		memcpy(buffer, databuffer + offset, length); // 复制数据到 buffer
		// 在这里使用 buffer 来进行读取或者发送操作
		result = libusb_interrupt_transfer(libusbHandle,
		                                   listUsbInfo[portIndex].ENDPOINT_OUT,
		                                   buffer,
		                                   listUsbInfo[portIndex].MaxPacketSizeOut,
		                                   &bytesSend, timeout);
		if (result != 0)
		{
			delete[] buffer; // 释放分配的内存
			closeLibusbHid();
			return false;
		}
		offset += length; // 更新偏移量
	}
	delete[] buffer; // 释放分配的内存
	return true;
}

// 扫描usb设备信息
void VersaLibusb::refreshLibusbDevice()
{
	if (libusbOpenState == true)
	{
		return;
	}
	
	int result = -1;
	libusb_context *context = nullptr;
	
	// 初始化 libusb
	result = libusb_init(&context);
	
	if (result < 0)
	{
		qDebug() << "libusb initialization failed";
		return;
	}
	
	// 获取设备列表
	libusb_device **devices;
	// 1.获取设备描述符和设备数量
	ssize_t deviceCount = libusb_get_device_list(context, &devices);
	
	if (deviceCount < 0)
	{
		libusb_exit(context);
		context = nullptr;
		return;
	}
	
	// 更新设备信息
	listUsbInfo.clear();
	nameDevices.clear();
	// 遍历设备
	for (ssize_t device_i = 0; device_i < deviceCount; ++device_i)
	{
		libusb_device *device = devices[device_i];
		struct libusb_device_descriptor device_descriptor;
		// 1.1.获取设备描述符
		{
			result = libusb_get_device_descriptor(device, &device_descriptor);
			if (result < 0)
			{
				continue;
			}
		}
		
		QString serialString;
		QString manufacturerString;
		QString productString;
		libusb_device_handle *handle = nullptr;
		
		// 1.2.打开设备
		{
			result = libusb_open(device, &handle);
			if (result < 0)
			{
				continue;
			}
		}
		
		// 1.3.获取字符串描述符
		{
			unsigned char serialNumber[256];
			unsigned char manufacturer[256];
			unsigned char product[256];
			result = libusb_get_string_descriptor_ascii(handle,
			                                            device_descriptor.iManufacturer,
			                                            manufacturer,
			                                            sizeof(manufacturer));
			if (result >= 0)
			{
				manufacturerString = QString::fromUtf8(reinterpret_cast<const char *>(manufacturer));
			}
			
			result = libusb_get_string_descriptor_ascii(handle,
			                                            device_descriptor.iProduct,
			                                            product,
			                                            sizeof(product));
			if (result >= 0)
			{
				productString = QString::fromUtf8(reinterpret_cast<const char *>(product));
			}
			
			// 获取设备的字符串描述符
			result = libusb_get_string_descriptor_ascii(handle,
			                                            device_descriptor.iSerialNumber,
			                                            serialNumber,
			                                            sizeof(serialNumber));
			if (result >= 0)
			{
				serialString = QString::fromUtf8(reinterpret_cast<const char *>(serialNumber));
			}
		}
		
		// 1.4.获取配置描述符
		{
			libusb_config_descriptor *config_descriptor;
			result = libusb_get_active_config_descriptor(device, &config_descriptor);
			if (result < 0)
			{
				libusb_close(handle);
				continue;
			}
			// 遍历接口和端点
			for (int i = 0; i < config_descriptor->bNumInterfaces; ++i)
			{
				VersaLibUsbInfo usbInfo;
				// 接口
				const libusb_interface *interface = &config_descriptor->interface[i];
				for (int j = 0; j < interface->num_altsetting; ++j)
				{
					// 接口描述符
					const libusb_interface_descriptor *interfaceDesc = &interface->altsetting[j];
					// 如果接口为 hid 设备
					if (interfaceDesc->bInterfaceClass == LIBUSB_CLASS_HID)
					{
						for (int k = 0; k < interfaceDesc->bNumEndpoints; ++k)
						{
							const libusb_endpoint_descriptor *endpointDesc = &interfaceDesc->endpoint[k];
							if (endpointDesc->bEndpointAddress & 0X80)
							{
								usbInfo.ENDPOINT_IN = endpointDesc->bEndpointAddress;
								usbInfo.MaxPacketSizeIn = endpointDesc->wMaxPacketSize;
								usbInfo.bIntervalIn = endpointDesc->bInterval;
							}
							else
							{
								usbInfo.ENDPOINT_OUT = endpointDesc->bEndpointAddress;
								usbInfo.MaxPacketSizeOut = endpointDesc->wMaxPacketSize;
								usbInfo.bIntervalOut = endpointDesc->bInterval;
							}
						}
						
						QString usbQString = QString("%1 0x%2-0x%3-%4 [0x%5][0x%6]")
								.arg(productString)
								.arg(QString("%1").arg(device_descriptor.idVendor, 4, 16, QChar('0')).toUpper())
								.arg(QString("%1").arg(device_descriptor.idProduct, 4, 16, QChar('0')).toUpper())
								.arg(interfaceDesc->bInterfaceNumber)
								.arg(QString("%1").arg(usbInfo.ENDPOINT_IN, 2, 16, QChar('0')).toUpper())
								.arg(QString("%1").arg(usbInfo.ENDPOINT_OUT, 2, 16, QChar('0')).toUpper());
						
						usbInfo.mName = usbQString;
						usbInfo.idProduct = device_descriptor.idProduct;
						usbInfo.idVendor = device_descriptor.idVendor;
						usbInfo.indexDevice = device_i;
						usbInfo.indexInterface = interfaceDesc->bInterfaceNumber;
						
						listUsbInfo.append(usbInfo);
						nameDevices.append(usbQString);
					}
				}
			}
			libusb_free_config_descriptor(config_descriptor);
		}
		libusb_close(handle);
	}
	libusb_free_device_list(devices, 1);
	libusb_exit(context);
	return;
}
