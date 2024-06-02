//
// Created by vSeasky on 2023/9/23.
//

#ifndef VERSA_ASSISTANT_VERSAPROTOCOLWIDGET_H
#define VERSA_ASSISTANT_VERSAPROTOCOLWIDGET_H

#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QElapsedTimer>
#include <QDateTime>
#include <QTimer>
#include <QPointer>
#include "VersaDataControl.h"
#include "bsp_protocol.h"

#define MAX_PROTOCOL_NUM 28

typedef union
{
	char data_char[4];
	uint8_t data_u8[4];
	int16_t data_int16[2];
	uint16_t data_u16[2];
	int32_t data_int32;
	uint32_t data_u32;
	float data_float;
} VersaDataUnion;

class VersaProtocolWidget : public QWidget
{
Q_OBJECT
public:
	enum ProtocolWidgetMode
	{
		ReadProtocolWidget = 0, SendProtocolWidget
	};
	protocol_struct protocolStruct;
	
	VersaProtocolWidget(const QByteArray &buffer,
	                    ProtocolWidgetMode mode,
	                    QWidget *parent = nullptr);
	~VersaProtocolWidget();
	
	void resizeProtocolTable(const int size);
	void restoreDataBuffer(const QByteArray &buffer, int size);
	void defautlDataBuffer(int size);
	QByteArray getSaveDataBuffer();
signals:
	void protocolVersa(const QString &address, quint16 port);
	void addGraphsData(const QVector<double> &keys, const QVector<double> &values);
	void updataEditHeaderSignals();
	void updataSendDataSignals();
public slots:
	void updataReadSlots();
	void refreshEditText();
	void timeout();
	void addPeerAddress(QString peerAddress);
	void addPeerPort(quint16 port);
private:
	void clearVector();
	void resizeVector(int size);
	
	void limitInputDouble(QLineEdit *lineEdit);
	void limitInputFloat32(QLineEdit *lineEdit);
	void limitInputInt32(QLineEdit *lineEdit);
	void limitInputUInt32(QLineEdit *lineEdit);
	
	void updataEditHeader();
	void updataSendData(int row);
	void updataReadData(int row);
	
	void refreshEditHeader();
	void refreshSendData(int row);
	void refreshReadData(int row);
	
	QPointer<QTableWidget> creatProtocolInfo();
	QPointer<QTableWidget> creatProtocolTable();
	
	void deleteProtocolTable();
	void insertProtocolRow(QTableWidget *dataTable, int start, int row);
	
	QString mAddress;
	quint16 mPort;
	
	QVector<QComboBox *> mVectorUnionType;
	QVector<QLineEdit *> mVectorLineEdit1;
	QVector<QLineEdit *> mVectorLineEdit2;
	QVector<QLineEdit *> mVectorLineEdit3;
	QVector<VersaDataControl *> mVectorDataControl;
	
	QVector<int32_t> mVectorUnionValue;
	QVector<VersaDataUnion> mVectorLineEdit1Value;
	QVector<VersaDataUnion> mVectorLineEdit2Value;
	QVector<VersaDataUnion> mVectorLineEdit3Value;
	QVector<int32_t> mVectorDataControlValue;
	
	QVector<QString> infoControl;
	
	QPointer<QLineEdit> editHeaderCmd = nullptr;
	QPointer<QLineEdit> editHeaderDeviceType = nullptr;
	QPointer<QLineEdit> editHeaderDeviceId = nullptr;
	QPointer<QLineEdit> editHeaderDataId = nullptr;
	QPointer<QSpinBox> editHeaderDataLen = nullptr;
	
	QPointer<QVBoxLayout> pProtocolWidgetLayout = nullptr;
	QPointer<QTableWidget> currentProtocolTable = nullptr;
	
	QPointer<QComboBox> pInternetAddrCombox = nullptr;
	QPointer<QComboBox> pInternetPortCombox = nullptr;
	
	
	QVector<double> readKeys;
	QVector<double> readValues;
	
	QDateTime startDateTime;
	
	const ProtocolWidgetMode protocolMode;
	bool ProtocolTableEnable = false;
	int currentMaxRowCount = 0;
	
	int currentIndex = 0;
	
	bool refreshReadHeaderEnable = false;
	QVector<bool> refreshReadDataEnable;
	bool refreshSendHeaderEnable = false;
	QVector<bool> updataSendDataEnable;
	
	
	int refreshSpeed = 75;
	QTimer mRefreshTimer;
	QTimer mTimer;
};

#endif //VERSA_ASSISTANT_VERSAPROTOCOLWIDGET_H
