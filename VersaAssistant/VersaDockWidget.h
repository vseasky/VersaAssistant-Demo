/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaDockWidget.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSADOCKWIDGET_H
#define VERSA_ASSISTANT_VERSADOCKWIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QDockWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "DockAreaTitleBar.h"
#include "DockAreaTabBar.h"
#include "DockWidgetTab.h"
#include "FloatingDockContainer.h"
#include "DockComponentsFactory.h"
#include "DockSplitter.h"
#include "VersaProtocolWidget.h"
#include "VersaReceiveWidget.h"
#include "VersaQCustomPlot.h"
#include "VersaRobotWidget.h"
#include "VersaMultSendWidget.h"
#include "VersaNormalSendWidget.h"
#include <QPointer>

class VersaDockWidget : public QWidget
{
Q_OBJECT
public:
	QPointer<ads::CDockManager> dockManager = nullptr;
	
	QPointer<VersaProtocolWidget> pVersaProtocolSend = nullptr;
	QPointer<VersaProtocolWidget> pVersaProtocolRead = nullptr;
	QPointer<VersaQCustomPlot> pVseraQCustomPlot = nullptr;
	QPointer<VersaNormalSendWidget> normalSendEdit = nullptr;
	QPointer<VersaMultSendWidget> multSendTextEdit = nullptr;
	QPointer<VersaRobotWidget> pVseraRobot = nullptr;
	QPointer<VersaReceiveWidget> pReceiveQTextEdit = nullptr;
	QPointer<QWidget> pMaskQWidget = nullptr;
	
	QPointer<QCheckBox> pSendMultEnableCheckBox = nullptr;
	QPointer<QCheckBox> pSendHexEnableCheckBox = nullptr;
	QPointer<QCheckBox> pSendNewLineEnableCheckBox = nullptr;
	QPointer<QCheckBox> pSendTimerEnableCheckBox = nullptr;
	QPointer<QCheckBox> pSendEnterEnableCheckBox = nullptr;
	
	
	VersaDockWidget(QWidget *parent = nullptr);
	~VersaDockWidget();
	
	void createSendWidget(void);//创建发送部件
	void createReceiveWidget(void);//创建接收部件
	void createProtocolWaveformWidget(void);//创建协议波形部件
	void createOpenGLWidget(void);//创建协议3D姿态部件
	void createRealtimeProtocolWidget(void);//创建实时协议部件
	void createProtocolDataWidget(void);//创建协议数据部件
	void createProtocolWidget(void);//创建协议发送部件
	void createHelpWidget(void);//创建帮助部件
	void createAboutWidget(void);//创建关于部件
	void createProtocolContentWidget(void);//创建协议内容部件
	void creatStyleWidget(void);//创建Style显示部件
	void creatViewMenu(QMenu *pQMenu);
	void maskWidgetSetEnable(bool enable);
	
	void setTextCode(const QByteArray &codeText)
	{
		emit textCodeChanged(codeText);
	};

public slots:
	void addPeerAddress(QString peerAddress);
	void addPeerPort(quint16 port);
	void sendFileText();
	
	void maskWidgetEnable()
	{
		maskWidgetSetEnable(true);
	};
	
	void maskWidgetDisable()
	{
		maskWidgetSetEnable(false);
	}

protected:
	void resizeEvent(QResizeEvent *event) override
	{
		pMaskQWidget->resize(pVersaDockWidget->size());
		pMaskQWidget->setGeometry(pVersaDockWidget->geometry());
	}

signals:
	void clicked(); // 定义一个点击信号
	void textCodeChanged(const QByteArray &codeText);
private:
	QPointer<QTimer> pTimerSend = nullptr;
	QPointer<QWidget> pVersaDockWidget = nullptr;
	
	QPointer<ads::CDockWidget> dockSendWidget = nullptr;
	QPointer<ads::CDockWidget> receiveWidget = nullptr;
	QPointer<ads::CDockWidget> protocolWaveformWidget = nullptr;
	QPointer<ads::CDockWidget> styleWidget = nullptr;
	QPointer<ads::CDockWidget> vseraRobotWidget = nullptr;
	QPointer<ads::CDockWidget> protocolSendWidget = nullptr;
	QPointer<ads::CDockWidget> protocolReadWidget = nullptr;
	QPointer<QComboBox> pSendTcpAddrCombox = nullptr;
	QPointer<QComboBox> pSendTcpPortCombox = nullptr;
	
	
	int SendAddrNum = 20;
	int SendPortNum = 20;
};


#endif //VERSA_ASSISTANT_DOCKWIDGETINIT_H
