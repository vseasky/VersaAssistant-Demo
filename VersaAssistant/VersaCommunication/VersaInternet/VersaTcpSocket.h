/*
 ************************************** Copyright ******************************
 * @FileName     :  vqtcpsocket.h
 * @Author       :  Seasky.Liu
 * @Date         :  2021/8/30
 * @Time         :  19:28
 * @Email        :  xxx.
 ******************************************************************************
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
