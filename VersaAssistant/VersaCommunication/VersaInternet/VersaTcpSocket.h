/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaInternet\VersaTcpSocket.h
 * @Description  : 
 */

#ifndef VQTCPSOCKET_H
#define VQTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class VersaTcpSocket : public QTcpSocket
{
Q_OBJECT
public:
	explicit VersaTcpSocket(QObject *parent = nullptr);
	~VersaTcpSocket();
	bool openSocket(void); //打开链接
	void closeSocket(void);//关闭链接
	void setAddress(QString Address);
	void setPort(quint16 port);
	QStringList refreshListTcpSocket(void);
signals:
private:
	QHostAddress mAddress;
	quint16 mPort;
};

#endif // VQTCPSOCKET_H
