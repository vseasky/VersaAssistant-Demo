#include "VersaTcpServer.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDateTime>
#include <QList>
#include <QThread>

// 构造函数
VersaTcpServer::VersaTcpServer(QObject *parent) : QTcpServer(parent)
{
	// 监视Tcp服务器状态
	connect(this, &QTcpServer::newConnection,
	        this, &VersaTcpServer::newConnect);
}

VersaTcpServer::~VersaTcpServer()
{
	closeServer();
}

bool VersaTcpServer::openServer(void)
{
	this->closeServer();
	openState = this->listen(mAddress, mPort);
	return (openState);
}

void VersaTcpServer::closeServer(void)
{
	openState = false;
	closeAllSocket();
	return;
}


void VersaTcpServer::setAddress(QString Address)
{
	mAddress = QHostAddress(Address);
}

void VersaTcpServer::setPort(quint16 port)
{
	mPort = port;
}

QStringList VersaTcpServer::refreshListTcpServer()
{
	QStringList stringList;
	
	if (openState == true)
	{
		return stringList;
	}
	
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
	return stringList;
}

QTcpSocket *VersaTcpServer::findServerSocket(const QHostAddress &address, quint16 port)
{
	if (listSocket.size() == 0)
	{
		return nullptr;
	}
	for (int iSocket = 0; iSocket < listSocket.size(); iSocket++)
	{
		if ((listSocket[iSocket]->peerAddress() == address) &&
		    listSocket[iSocket]->peerPort() == port)
		{
			return listSocket[iSocket];
		}
	}
	return nullptr;
}

// 新的链接接入
void VersaTcpServer::newConnect()
{
	// 挂起的连接
	QTcpSocket *newSocket = nextPendingConnection();
	// 保存客户端连接
	listSocket.append(newSocket);
	// 发送欢迎消息给客户端
	
	// 断开连接
	QMetaObject::Connection disconnectSocket = connect(newSocket, &QTcpSocket::disconnected, [=]()
	{
		if (newSocket != nullptr)
		{
			listSocket.removeOne(newSocket);
			newSocket->deleteLater(); // 释放资源
			if (listSocket.size() == 0)
			{
				listSocket.clear();
			}
		}
	});
	
	// 接收数据
	QMetaObject::Connection readyReadSocket = connect(newSocket, &QTcpSocket::readyRead, [=]()
	{
		// 读取数据
		if (openState == true)
		{
			emit readyRead(newSocket);
		}
	});
	
	if (listSocket.size() > 0)
	{
		emit newConnectSignals(newSocket);
	}
}

// 关闭所有，同时清空所有内存
void VersaTcpServer::closeAllSocket()
{
	openState = false;
	// 遍历 listSocket 容器中的所有元素
	while (listSocket.size() > 0)
	{
		if (listSocket[0] != nullptr)
		{
			QTcpSocket *pSocket = listSocket[0];
			// 中止当前套接字连接
			listSocket.removeOne(pSocket);
			pSocket->deleteLater();
			QThread::msleep(10);
		}
	}
	// 清空 listSocket 容器
	listSocket.clear();
	// 关闭服务器
	close();
}