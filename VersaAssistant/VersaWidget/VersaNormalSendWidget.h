//
// Created by vSeasky on 2023/9/20.
//

#ifndef VERSA_ASSISTANT_VERSATEXTEDIT_H
#define VERSA_ASSISTANT_VERSATEXTEDIT_H

#include <QTextEdit>
#include <QTimer>
#include <QDateTime>
#include <QScrollBar>

class VersaNormalSendWidget : public QTextEdit
{
Q_OBJECT
public:
	VersaNormalSendWidget(QWidget *parent = nullptr);
	~VersaNormalSendWidget();
	
	void loadDataBuffer(const QByteArray &buffer);
	void restoreDataBuffer(const QByteArray &buffer);
	QByteArray getSaveDataBuffer();
signals:
	void readyWriteSignals(const QString &address, quint16 port, const QByteArray &data);
public slots:
	
	void setTextCodec(const QByteArray &codeText);
	void setHexEnable(bool enValue);
	void setNewLineEnable(bool enValue);
	void setEnterEnable(bool enValue);
	
	void currentTextChanged();
	void changeTcp(const QString &address, quint16 port);
	void readyWrite(const QString &address, quint16 port);
	void readyWriteFileText(const QString &address, quint16 port);
protected:
	void insertFromMimeData(const QMimeData *source) override;
	void keyPressEvent(QKeyEvent *event) override;
private:
	QByteArray textBuffer;
	QByteArray mSendBuffer;
	QByteArray textCode = "UTF-8";
	QString sendAddr = "";
	quint16 sendPort = 32401;
	bool registerNewLineEnable = false;
	bool registerEnableHex = false;
	bool registerEnterEnable = false;
};


#endif //VERSA_ASSISTANT_VERSATEXTEDIT_H