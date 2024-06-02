//
// Created by vSeasky on 2023/9/14.
//

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
