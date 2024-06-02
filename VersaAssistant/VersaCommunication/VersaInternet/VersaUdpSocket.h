//
// Created by vSeasky on 2023/9/16.
//

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
