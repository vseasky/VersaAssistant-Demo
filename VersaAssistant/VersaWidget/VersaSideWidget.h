//
// Created by vSeasky on 2023/9/15.
//

#ifndef VERSA_ASSISTANT_VERSASIDEWIDGET_H
#define VERSA_ASSISTANT_VERSASIDEWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QStackedLayout>
#include <QComboBox>
#include <QPointer>


class VersaSideWidget : public QWidget
{
Q_OBJECT
public:
	VersaSideWidget(const QByteArray &buffer, QWidget *parent = nullptr);
	~VersaSideWidget();
	
	void creatPhysicalSerialWidget();
	void creatPhysicalInternetWidget();
	void creatPhysicalUsbWidget();
	
	void resizeSidebar();
	void toggleSidebar();
	void showSidebar();
	void hideSidebar();
	
	QByteArray getSaveDataBuffer();
	void restoreDataBuffer(const QByteArray &buffer);
signals:
	
	void serialIndexChanged(int index);
	void serialBaudRateChanged(int baudRate);
	void serialStopBitsChanged(QSerialPort::StopBits stopBits);
	void serialDataBitsChanged(QSerialPort::DataBits dataBits);
	void serialParrityChanged(QSerialPort::Parity Parrity);
	void serialFlowControlChanged(QSerialPort::FlowControl flowControl);
	
	void internetType(int type);
	void internetAddress(QString addr);
	void internetPort(quint16 port);
	
	void usbIndexChanged(int index);
	
	void showSidebarStart();
	void showSidebarEnd();
	void hideSidebarStart();
	void hideSidebarEnd();

public slots:
	void refreshOpenState(bool state);
	void setStackedLayout(int index);
	void setPhysicaLibusbCombox(const QStringList &listString, int index);
	void setPhysicaSerialCombox(const QStringList &listString, int index);
	void setPhysicaInternetCombox(const QStringList &listString, int index);
private:
	int mSpacing = 8;
	const int mSidebarWidth = 450;
	bool mSidebarHidden = false;
	
	QWidget *pParentWidget;
	QPointer<QStackedLayout> pStackedLayout;
	QPointer<QComboBox> pPhysicaSerialCombox;
	QPointer<QComboBox> pPhysicaLibusbCombox;
	QPointer<QComboBox> pPhysicaInternetComboBox;
	QPointer<QWidget> pPhysicalSerialWidget;
	QPointer<QWidget> pPhysicalInternetWidget;
	QPointer<QWidget> pPhysicalLibusbWidget;
	QVector<QByteArray> textEditBuffer;
};


#endif //VERSA_ASSISTANT_VERSASIDEWIDGET_H
