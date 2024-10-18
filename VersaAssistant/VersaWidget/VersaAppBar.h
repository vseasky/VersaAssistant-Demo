/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaAppBar.h
 * @Description  : 
 */
#ifndef VERSA_ASSISTANT_VERSAAPPBAR_H
#define VERSA_ASSISTANT_VERSAAPPBAR_H

#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QWidget>
#include <QPointer>

class VersaAppBar : public QWidget
{
Q_OBJECT
public:
	QPointer<QPushButton> minPushButton;
	QPointer<QPushButton> maxPushButton;
	QPointer<QPushButton> closePushButton;
	QPointer<QPushButton> logoPushButton;
	QPointer<QLabel> appNameLabel;
	QPointer<QWidget> menuBarWidget;
	QPointer<QMenuBar> menuBar;
	QPointer<QLabel> stateTxQLabel;
	QPointer<QLabel> stateRxQLabel;

	VersaAppBar(int height, int icon_size, QWidget *parent = nullptr);
	~VersaAppBar();
public slots:
	void slotMaximizedChanged(bool bMax);
protected:

private:
	const int iconSize = 32;
	const int titleHeight = 64;
};

#endif //VERSA_ASSISTANT_VERSAAPPBAR_H
