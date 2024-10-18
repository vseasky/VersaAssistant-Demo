/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaQuickWidget.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSAQUICKWIDGET_H
#define VERSA_ASSISTANT_VERSAQUICKWIDGET_H

#include <QPointer>
#include <QWidget>
#include <QPushButton>

class VersaQuickWidget : public QWidget
{
Q_OBJECT
public:
	VersaQuickWidget(QWidget *parent = nullptr);
	~VersaQuickWidget();
	void initUI();
	void toggleSidebar();
	void showSidebar();
	void hideSidebar();
private:
	QPointer<QPushButton> sidebarButton;
	QPointer<QWidget> sidebar;
	bool sidebar_hidden;
	int sidebar_width;
};


#endif //VERSA_ASSISTANT_VERSAQUICKWIDGET_H
