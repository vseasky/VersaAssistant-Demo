/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaReceiveWidget.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSATEXTWIDGET_H
#define VERSA_ASSISTANT_VERSATEXTWIDGET_H

#include <QPointer>
#include <QTextEdit>
#include <QTimer>
#include <QDateTime>
#include <QScrollBar>

class VersaReceiveWidget : public QWidget
{
Q_OBJECT
public:
	VersaReceiveWidget(QWidget *parent = nullptr);
	~VersaReceiveWidget();
	void setTextBuffer(QByteArray *buffer);
	void setHexEnable(bool enable);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

public slots:
	void setTextCodec(const QByteArray &codeText);
	void actionTriggered(int action);
	void refreshBuffer();
	void timerout();

private:
	void refreshText();
	void refreshScroll();
	QTextEdit textEdit;
	QByteArray textCode = "UTF-8";
	QScrollBar verticalScroll;
	QTimer timerBase;
	QByteArray* pTextBuffer = nullptr;
	int indexStart = 0;
	int indexLength = 8192;//显示数据宽度
	int indexMaxLength = 8192;
	int bytesPerLine;
	QTextCursor::MoveOperation refreshMove;
	bool enableRefresh = false;
	bool enableHex = false;
	bool enableRefreshScroll = false;
};

#endif //VERSA_ASSISTANT_VERSATEXTWIDGET_H
