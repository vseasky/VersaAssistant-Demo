/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaCommunication\VersaInternet\VersaUdpSocket.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSAUDPSOCKET_H
#define VERSA_ASSISTANT_VERSAUDPSOCKET_H

#include <QUdpSocket>

class VersaUdpSocket : public QUdpSocket
{
Q_OBJECT
public:
	explicit VersaUdpSocket(QObject *parent = nullptr);
	~VersaUdpSocket();
	bool openSocket(void); //打开链接
	void closeSocket(void);//关闭链接
	void setAddress(QString Address);
	void setPort(quint16 port);
	QStringList refreshListUdpSocket(void);
signals:
private:
	QHostAddress mAddress;
	quint16 mPort;
};


#endif //VERSA_ASSISTANT_VERSAUDPSOCKET_H
