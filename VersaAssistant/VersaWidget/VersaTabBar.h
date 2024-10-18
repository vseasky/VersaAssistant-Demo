/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaTabBar.h
 * @Description  : 
 */

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
