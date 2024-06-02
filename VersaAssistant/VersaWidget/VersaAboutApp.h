//
// Created by vSeasky on 2023/10/24.
//

#ifndef VERSADEBUGASSISTANTPROJECT_VERSAABOUTAPP_H
#define VERSADEBUGASSISTANTPROJECT_VERSAABOUTAPP_H

#include <QWidget>
#include <QCloseEvent>
#include <QTextEdit>
#include <QTableWidget>

class VersaAboutApp : public QWidget
{
Q_OBJECT
public:
	VersaAboutApp(QWidget *parent = nullptr);
	~VersaAboutApp();
private:
	QTableWidget tableWidget;
protected:
	void closeEvent(QCloseEvent *event) override
	{
		event->ignore(); // 忽略关闭事件
		Q_UNUSED(event);
		hide(); // 隐藏窗口
	}
};


#endif //VERSADEBUGASSISTANTPROJECT_VERSAABOUTAPP_H
