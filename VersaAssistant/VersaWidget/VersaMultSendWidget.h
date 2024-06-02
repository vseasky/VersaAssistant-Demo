//
// Created by vSeasky on 2023/9/22.
//

#ifndef VERSA_ASSISTANT_VERSAMULTTEXT_H
#define VERSA_ASSISTANT_VERSAMULTTEXT_H

#include <QTableWidget>
#include <QByteArray>
#include <QContextMenuEvent>
#include <QMenu>
#include <QVBoxLayout>
#include <QPointer>


#define MAX_MULT_NUM 128

class VersaMultSendWidget : public QWidget
{
Q_OBJECT
public:
	VersaMultSendWidget(QWidget *parent = nullptr);
	~VersaMultSendWidget();
	void loadDataBuffer(const QByteArray &buffer, int count = 128);
	QPointer<QTableWidget> creatMultTextWidget();
	void defaultConfigDataBuffer(int count);
	void resizeMultSendWidget(const int count);
	void restoreDataBuffer(const QByteArray &buffer, int count);
	QByteArray getSaveDataBuffer();
	void writeIndex(int index);

signals:
	void readyWriteSignals(const QString &address, quint16 port, const QByteArray &data);
public slots:
	void setTextCodec(const QByteArray &codeText);
	void readyWrite();
	void setHexEnable(bool enValue);
	void setNewLineEnable(bool enValue);
	void setEnterEnable(bool enValue);
protected:
	void insertMultRow(QTableWidget *tableWidget, int row);
	void keyPressEvent(QKeyEvent *event) override;
private:
	QPointer<QVBoxLayout> pMultQTableLayout = nullptr;
	QPointer<QTableWidget> pMultQTableWidget = nullptr;
	QVector<bool> mVectorTimerEnable;
	QVector<QString> mVectorAddress;
	QVector<quint16> mVectorPort;
	QVector<QString> mVectorData;
	QByteArray textCode = "UTF-8";
	int maxRowCount;
	int currentIndex = 0;
	bool registerEnableNewline = false;
	bool registerEnableHex = false;
	bool registerEnterEnable = false;
};

#endif //VERSA_ASSISTANT_VERSAMULTTEXT_H
