//
// Created by vSeasky on 2023/9/16.
//

#include "VersaUdpSocket.h"
#include <QNetworkInterface>
#include <QDebug>

VersaUdpSocket::VersaUdpSocket(QObject *parent) : QUdpSocket(parent)
{

}

VersaUdpSocket::~VersaUdpSocket()
{
	closeSocket();
}

bool VersaUdpSocket::openSocket()
{
	if (this->isOpen())
	{
		closeSocket();
	}
	// 绑定本地端口
	bool bindResult = bind(mAddress, mPort);
	if (bindResult)
	{
		setOpenMode(QIODevice::ReadWrite); // 设置设备为可读写模式
		bool openResult = open(QIODevice::ReadWrite); // 打开设备
		return openResult;
	}
	return false;
}

void VersaUdpSocket::closeSocket()
{
	close();
}

void VersaUdpSocket::setAddress(QString Address)
{
	mAddress = QHostAddress(Address);
}

void VersaUdpSocket::setPort(quint16 port)
{
	mPort = port;
}

QStringList VersaUdpSocket::refreshListUdpSocket(void)
{
	QStringList stringList;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
			foreach (QHostAddress address, ipAddressesList)
		{
			if (address.protocol() == QAbstractSocket::IPv4Protocol &&
			    !address.isLoopback() &&
			    address.toIPv4Address())
			{
				QUdpSocket socket;
				socket.bind(address);
				if (socket.state() == QUdpSocket::BoundState)
				{
					stringList.append(address.toString());
				}
			}
		}
	return stringList;
}
