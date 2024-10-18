/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaInternet\VersaTcpSocket.cpp
 * @Description  : 
 */

#include "VersaTcpSocket.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QThread>
#include <QDebug>

VersaTcpSocket::VersaTcpSocket(QObject *parent) : QTcpSocket(parent)
{

}

VersaTcpSocket::~VersaTcpSocket()
{
	closeSocket();
}

bool VersaTcpSocket::openSocket(void)
{
	closeSocket();
	this->connectToHost(mAddress, mPort);
	return (this->waitForConnected(10 * 1000));
}

void VersaTcpSocket::closeSocket(void)
{
	this->close();
}

void VersaTcpSocket::setAddress(QString Address)
{
	mAddress = QHostAddress(Address);
}

void VersaTcpSocket::setPort(quint16 port)
{
	mPort = port;
}

QStringList VersaTcpSocket::refreshListTcpSocket(void)
{
	QStringList stringList;
	QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
			foreach (QNetworkInterface interface, interfaces)
		{
			if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
			    !interface.flags().testFlag(QNetworkInterface::IsLoopBack))
			{
				QList<QNetworkAddressEntry> entries = interface.addressEntries();
						foreach (QNetworkAddressEntry entry, entries)
					{
						if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
						{
							stringList.append(entry.ip().toString());
						}
					}
			}
		}
	stringList.append("127.0.0.1");
	return stringList;
}