//
// Created by vSeasky on 2023/9/13.
//

#ifndef VERSA_ASSISTANT_VERSAMESSAGEBOX_H
#define VERSA_ASSISTANT_VERSAMESSAGEBOX_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFramelessDialog.h>
#include <QTimer>
#include <QPointer>
#include <QLabel>


class VersaMessageBox : public QFramelessDialog
{
Q_OBJECT
public:
	QTimer mTimer;
	QPointer<QVBoxLayout> pQVBoxLayout;
	QPointer<QVBoxLayout> pQLabelLayout;
	
	VersaMessageBox(QWidget *parent = nullptr);
	~VersaMessageBox();
	void VersaMessagePrintf(const QString &message, int showTime);
	void VersaTimeout();
private:
	QLabel messageBoxInfo;
};

void VersaMessageInitFun(void);
void VersaMessageFreeFun(void);
void VersaMessageShowFun(const QString &str, int time = 1500);


#define VersaMessageInit VersaMessageInitFun()
#define VersaMessageFree VersaMessageFreeFun()

#endif //VERSA_ASSISTANT_VERSAMESSAGEBOX_H
