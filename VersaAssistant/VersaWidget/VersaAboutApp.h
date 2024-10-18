/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaAboutApp.h
 * @Description  : 
 */


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
