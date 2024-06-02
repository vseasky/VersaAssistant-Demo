//
// Created by vSeasky on 2023/9/13.
//

#ifndef VERSA_ASSISTANT_VERSATABBAR_H
#define VERSA_ASSISTANT_VERSATABBAR_H

#include <QWidget>
#include <QPushButton>
#include <QPointer>

class VersaTabBar : public QWidget
{
Q_OBJECT
public:
	VersaTabBar(QWidget *parent = nullptr);
	~VersaTabBar();
signals:
	void currentIndexChanged(int index);
	void openReleased();
	void protocolEnableChanged(bool enable);
public:
	void refreshOpenState(bool checked);
private:
	int tabWidth = 64;
	QPointer<QPushButton> pOpenButton;
	QPointer<QPushButton> pProtocolButton;
};


#endif //VERSA_ASSISTANT_VERSATABBAR_H
