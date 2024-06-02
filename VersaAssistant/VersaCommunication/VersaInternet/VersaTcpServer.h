/*
 ************************************** Copyright ******************************
 * @FileName     :  vqtcpserver.h
 * @Author       :  Seasky.Liu
 * @Date         :  2021/8/30
 * @Time         :  19:28
 * @Email        :  xxx.
 ******************************************************************************
 */

#ifndef VQTCPSERVER_H
#define VQTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>


class VersaTcpServer : public QTcpServer
{
Q_OBJECT
public:
	explicit VersaTcpServer(QObject *parent = nullptr);
	~VersaTcpServer();
	bool openServer(void);
	void closeServer(void);
	void setAddress(QString Address);
	void setPort(quint16 port);
	QTcpSocket *findServerSocket(const QHostAddress &address, quint16 port);
	QStringList refreshListTcpServer(void);
signals:
	void readyRead(QTcpSocket *socket);
	void newConnectSignals(QTcpSocket *socket);
private:
	void closeAllSocket(void);
	void newConnect(void);
	QList<QTcpSocket *> listSocket; // 用于保存所有客户端连接
	QVector<QMetaObject::Connection *> pConnection;
	QHostAddress mAddress;
	quint16 mPort;
	bool openState = false;
signals:

};

#endif // VQTCPSERVER_H
