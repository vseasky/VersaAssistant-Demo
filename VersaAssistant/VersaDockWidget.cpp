#include <QTextEdit>
#include "VersaDockWidget.h"
#include "VersaQCustomplot/VersaQCustomPlot.h"
#include "VersaQColorWidget.h"
#include "VersaConfig.h"
#include "VersaPhysicalCom.h"
#include "VersaNormalSendWidget.h"
#include "VersaProtocolWidget.h"

VersaDockWidget::VersaDockWidget(QWidget *parent) : QWidget(parent)
{
	
	this->setAttribute(Qt::WA_TranslucentBackground, false);
	// 创建一个容器窗口，自适应大小
	{
		pVersaDockWidget = new QWidget(this);
		pVersaDockWidget->setObjectName("pVersaDockWidget");
		QVBoxLayout *pVersaDockLayout = new QVBoxLayout(this);
		pVersaDockLayout->setContentsMargins(4, 4, 4, 4);
		pVersaDockLayout->setSpacing(0);
		pVersaDockLayout->addWidget(pVersaDockWidget);
		this->setLayout(pVersaDockLayout);
	}
	// 活动窗口
	{
		using namespace ads;
		// 如果选项卡关闭按钮应该是，则取消注释以下行
		// QToolButton 而不是 QPushButton
		// CDockManager::setConfigFlags(CDockManager::configFlags() | CDockManager::TabCloseButtonIsToolButton);
		
		// 如果您想使用不透明的取消对接，请取消注释以下行
		// 不透明分割器调整大小
		// CDockManager::setConfigFlags(CDockManager::DefaultOpaqueConfig);
		
		// 如果您想要固定制表符宽度，请取消注释以下行
		// 如果关闭按钮的可见性发生变化，则不会改变
		CDockManager::setConfigFlag(CDockManager::RetainTabSizeWhenCloseButtonHidden, true);
		
		// 如果您不想在 DockArea 标题栏上显示关闭按钮，请取消注释以下行
		// CDockManager::setConfigFlag(CDockManager::DockAreaHasCloseButton, false);
		
		// 如果您不想在 DockArea 标题栏上取消停靠按钮，请取消注释以下行
		// CDockManager::setConfigFlag(CDockManager::DockAreaHasUndockButton, false);
		
		// 如果您不想在 DockArea 的标题栏上显示选项卡菜单按钮，请取消注释以下行
		CDockManager::setConfigFlag(CDockManager::DockAreaHasTabsMenuButton, false);
		
		// 如果您不希望禁用按钮出现在 DockArea 的标题栏上，请取消注释以下行
		CDockManager::setConfigFlag(CDockManager::DockAreaHideDisabledButtons, true);
		
		// 如果您想仅当有多个选项卡且至少其中一个已省略标题时才在 DockArea 的标题栏上显示选项卡菜单按钮，请取消注释以下行
		// CDockManager::setConfigFlag(CDockManager::DockAreaDynamicTabsMenuButtonVisibility, true);
		
		// 如果您希望浮动容器始终显示应用程序标题而不是活动停靠小部件的标题，请取消注释以下行
		// CDockManager::setConfigFlag(CDockManager::FloatingContainerHasWidgetTitle, false);
		
		// 如果您希望浮动容器显示活动停靠小部件的图标而不是始终显示应用程序图标，请取消注释以下行
		CDockManager::setConfigFlag(CDockManager::FloatingContainerHasWidgetIcon, true);
		
		// 如果您希望主停靠容器（停靠管理器）中的中央小部件不带标题栏，请取消注释以下行
		// 如果启用此代码，您可以在演示中使用日历 0 进行测试
		// 停靠小部件。
		// CDockManager::setConfigFlag(CDockManager::HideSingleCentralWidgetTitleBar, true);
		
		// 取消注释以下行以启用扩展坞的焦点突出显示
		// 具有焦点的小部件
		CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
		
		// 如果您想启用停靠小部件自动隐藏，请取消注释
		CDockManager::setAutoHideConfigFlags({CDockManager::DefaultAutoHideConfig});
		
		// 如果您想启用平均分配，请取消注释
		// 所有包含的停靠小部件的拆分器的可用大小
		// CDockManager::setConfigFlag(CDockManager::EqualSplitOnInsertion, true);
		
		// 如果您想使用鼠标中键（网络浏览器样式）关闭选项卡，请取消注释
		// CDockManager::setConfigFlag(CDockManager::MiddleMouseButtonClosesTab, true);
		
		dockManager = new ads::CDockManager(pVersaDockWidget);
		dockManager->setStyleSheet(ThemeStyleQss());
		
		dockManager->setContentsMargins(0, 0, 0, 0);
		QVBoxLayout *dockLayout = new QVBoxLayout(pVersaDockWidget);
		dockLayout->setContentsMargins(0, 0, 0, 0);
		dockLayout->setSpacing(0);
		dockLayout->addWidget(dockManager);
		// dockManager->setFont(ThemeQFont("FontNormal"));
		pVersaDockWidget->setLayout(dockLayout);
		pVersaDockWidget->setContentsMargins(0, 0, 0, 0);
		
		createReceiveWidget();
		dockManager->addDockWidget(ads::TopDockWidgetArea, receiveWidget);
		
		createSendWidget();
		dockManager->addDockWidget(ads::BottomDockWidgetArea, dockSendWidget);
		
		createProtocolWaveformWidget();
		dockManager->addDockWidget(ads::AutoHideDockAreas, protocolWaveformWidget);
		
		creatStyleWidget();
		dockManager->addDockWidget(ads::AutoHideDockAreas, styleWidget);
		
		createOpenGLWidget();
		dockManager->addDockWidget(ads::AutoHideDockAreas, vseraRobotWidget);
		
		createProtocolWidget();
		dockManager->addDockWidget(ads::LeftAutoHideArea, protocolSendWidget);
		dockManager->addDockWidget(ads::RightAutoHideArea, protocolReadWidget);
	}
	{
		pMaskQWidget = new QWidget(this);
		QVBoxLayout *maskLayout = new QVBoxLayout(pMaskQWidget);
		pMaskQWidget->setStyleSheet("QWidget{background-color:rgba(0, 0, 0, 0.50);}");
		pMaskQWidget->setLayout(maskLayout);
		pMaskQWidget->setContentsMargins(0, 0, 0, 0);
		pMaskQWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
		pMaskQWidget->hide();
	}
}

void VersaDockWidget::maskWidgetSetEnable(bool enable)
{
	if (enable)
	{
		pMaskQWidget->show();
	}
	else
	{
		pMaskQWidget->hide();
	}
}

VersaDockWidget::~VersaDockWidget()
{
	if (!dockManager.isNull())
	{
		delete dockManager;
		dockManager = nullptr;
	}
	
	if (!pVersaProtocolSend.isNull())
	{
		delete pVersaProtocolSend;
		pVersaProtocolSend = nullptr;
	}
	
	if (!pVersaProtocolRead.isNull())
	{
		delete pVersaProtocolRead;
		pVersaProtocolRead = nullptr;
	}
	
	if (!pVseraQCustomPlot.isNull())
	{
		delete pVseraQCustomPlot;
		pVseraQCustomPlot = nullptr;
	}
	
	if (!normalSendEdit.isNull())
	{
		delete normalSendEdit;
		normalSendEdit = nullptr;
	}
	
	if (!multSendTextEdit.isNull())
	{
		delete multSendTextEdit;
		multSendTextEdit = nullptr;
	}
	
	if (!pVseraRobot.isNull())
	{
		pVseraRobot->freeRobotWidget();
		delete pVseraRobot;
		pVseraRobot = nullptr;
	}
	
	if (!pReceiveQTextEdit.isNull())
	{
		delete pReceiveQTextEdit;
		pReceiveQTextEdit = nullptr;
	}
	
	if (!pSendMultEnableCheckBox.isNull())
	{
		delete pSendMultEnableCheckBox;
		pSendMultEnableCheckBox = nullptr;
	}
	
	if (!pSendHexEnableCheckBox.isNull())
	{
		delete pSendHexEnableCheckBox;
		pSendHexEnableCheckBox = nullptr;
	}
	
	if (!pSendNewLineEnableCheckBox.isNull())
	{
		delete pSendNewLineEnableCheckBox;
		pSendNewLineEnableCheckBox = nullptr;
	}
	
	if (!pSendTimerEnableCheckBox.isNull())
	{
		delete pSendTimerEnableCheckBox;
		pSendTimerEnableCheckBox = nullptr;
	}
	
	if (!pSendEnterEnableCheckBox.isNull())
	{
		delete pSendEnterEnableCheckBox;
		pSendEnterEnableCheckBox = nullptr;
	}
	
	if (!pVersaDockWidget.isNull())
	{
		delete pVersaDockWidget;
		pVersaDockWidget = nullptr;
	}
	
	if (!pMaskQWidget.isNull())
	{
		delete pMaskQWidget;
		pMaskQWidget = nullptr;
	}
	
	if (!dockSendWidget.isNull())
	{
		delete dockSendWidget;
		dockSendWidget = nullptr;
	}
	
	if (!receiveWidget.isNull())
	{
		delete receiveWidget;
		receiveWidget = nullptr;
	}
	
	if (!protocolWaveformWidget.isNull())
	{
		delete protocolWaveformWidget;
		protocolWaveformWidget = nullptr;
	}
	
	if (!styleWidget.isNull())
	{
		delete styleWidget;
		styleWidget = nullptr;
	}
	
	if (!vseraRobotWidget.isNull())
	{
		vseraRobotWidget->hide();
		delete vseraRobotWidget;
		vseraRobotWidget = nullptr;
	}
	
	if (!protocolSendWidget.isNull())
	{
		delete protocolSendWidget;
		protocolSendWidget = nullptr;
	}
	
	if (!protocolReadWidget.isNull())
	{
		delete protocolReadWidget;
		protocolReadWidget = nullptr;
	}
	
	if (!pSendTcpAddrCombox.isNull())
	{
		delete pSendTcpAddrCombox;
		pSendTcpAddrCombox = nullptr;
	}
	
	if (!pSendTcpPortCombox.isNull())
	{
		delete pSendTcpPortCombox;
		pSendTcpPortCombox = nullptr;
	}
}

void VersaDockWidget::addPeerAddress(QString peerAddress)
{
	if (pSendTcpAddrCombox != nullptr)
	{
		if (pSendTcpAddrCombox->count() >= SendAddrNum)
		{
			pSendTcpAddrCombox->removeItem(0);
		}
		QRegularExpression regex("^(?:\\d{1,3}\\.){3}\\d{1,3}$");
		if (regex.match(peerAddress).hasMatch())
		{
			int index = pSendTcpAddrCombox->findText(peerAddress);
			if (index == -1)
			{
				pSendTcpAddrCombox->addItem(peerAddress);
			}
		}
	}
}

void VersaDockWidget::addPeerPort(quint16 port)
{
	if (pSendTcpPortCombox != nullptr)
	{
		if (pSendTcpPortCombox->count() >= SendPortNum)
		{
			pSendTcpPortCombox->removeItem(0);
		}
		QString portStr = QString("%1").arg(port);
		int index = pSendTcpPortCombox->findText(QString(portStr));
		if (index == -1)
		{
			pSendTcpPortCombox->addItem(portStr);
		}
	}
}

void VersaDockWidget::creatViewMenu(QMenu *pQMenu)
{
	pQMenu->addAction(dockSendWidget->toggleViewAction());
	pQMenu->addAction(receiveWidget->toggleViewAction());
	pQMenu->addAction(protocolWaveformWidget->toggleViewAction());
	pQMenu->addAction(styleWidget->toggleViewAction());
	pQMenu->addAction(vseraRobotWidget->toggleViewAction());
	pQMenu->addAction(protocolSendWidget->toggleViewAction());
	pQMenu->addAction(protocolReadWidget->toggleViewAction());
	
	dockSendWidget->toggleView(false);
	receiveWidget->toggleView(false);
	protocolWaveformWidget->toggleView(false);
	styleWidget->toggleView(false);
	vseraRobotWidget->toggleView(false);
	protocolSendWidget->toggleView(false);
	protocolReadWidget->toggleView(false);
}

void VersaDockWidget::creatStyleWidget(void)
{
	// 创建一个可移动窗口 pProtocolWaveformCDockWidget
	ads::CDockWidget *pStyleCDockWidget = new ads::CDockWidget(tr("颜色板"));
	pStyleCDockWidget->setStyleSheet(ThemeStyleQss());
	pStyleCDockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
	pStyleCDockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
	// styleWidget->setFont(ThemeQFont("FontNormal"));
	pStyleCDockWidget->setContentsMargins(0, 0, 0, 0);
	pStyleCDockWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
	
	// 创建一个内容窗口 ProtocolWaveform 父元素为 styleCDockWidget
	QWidget *styleQWidget = new QWidget(pStyleCDockWidget);
	styleQWidget->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *styleCDockWidgetLayout = new QVBoxLayout(styleQWidget);
	styleCDockWidgetLayout->setContentsMargins(0, 0, 0, 0);
	styleQWidget->setLayout(styleCDockWidgetLayout);
	
	VersaQColorWidget *pVseraQColorWidget = new VersaQColorWidget(styleQWidget);
	styleCDockWidgetLayout->addWidget(pVseraQColorWidget);
	
	pStyleCDockWidget->setWidget(styleQWidget);
	styleWidget = pStyleCDockWidget;
}

void VersaDockWidget::sendFileText()
{
	if (normalSendEdit == nullptr)
	{
		return;
	}
	normalSendEdit->readyWriteFileText(pSendTcpAddrCombox->currentText(),
	                                   pSendTcpPortCombox->currentText().toInt());
}


// 发送数据窗口
void VersaDockWidget::createSendWidget(void)
{
	ads::CDockWidget *pSendCDockWidget = new ads::CDockWidget(tr("发送"));
	pSendCDockWidget->setStyleSheet(ThemeStyleQss());
	pSendCDockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
	pSendCDockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
	pSendCDockWidget->setContentsMargins(0, 0, 0, 0);
	pSendCDockWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
	// pSendCDockWidget->setFont(ThemeQFont("FontNormal"));
	// 添加一个布局窗口
	QWidget *pSendQWidget = new QWidget(pSendCDockWidget);
	pSendQWidget->setContentsMargins(0, 0, 0, 0);
	QHBoxLayout *pSendLayout = new QHBoxLayout(pSendQWidget);
	pSendLayout->setSpacing(0);
	pSendLayout->setContentsMargins(0, 0, 0, 4);
	pSendQWidget->setLayout(pSendLayout);
	pSendCDockWidget->setWidget(pSendQWidget);
	// 添加发送编辑区
	QTabWidget *pSendTabWidget = new QTabWidget(pSendQWidget);
	pSendTabWidget->setContentsMargins(0, 0, 0, 0);
	
	// 添加常规发送编辑窗口
	QWidget *normalSendEditWidget = new QWidget(pSendTabWidget);
	QVBoxLayout *normalSendEditLayout = new QVBoxLayout(normalSendEditWidget);
	normalSendEditLayout->setContentsMargins(0, 0, 0, 0);
	normalSendEditWidget->setLayout(normalSendEditLayout);
	normalSendEdit = new VersaNormalSendWidget(normalSendEditWidget);
	normalSendEditLayout->addWidget(normalSendEdit);
	pSendTabWidget->addTab(normalSendEditWidget, tr("单条发送"));
	
	// 添加多条发送编辑窗口
	QWidget *multSendEditWidget = new QWidget(pSendTabWidget);
	QVBoxLayout *multSendEditLayout = new QVBoxLayout(multSendEditWidget);
	multSendEditLayout->setContentsMargins(0, 0, 0, 0);
	multSendEditWidget->setLayout(multSendEditLayout);
	multSendTextEdit = new VersaMultSendWidget(multSendEditWidget);
	multSendEditLayout->addWidget(multSendTextEdit);
	pSendTabWidget->addTab(multSendEditWidget, tr("多条发送"));
	
	// 发送控制区域
	QWidget *pSendControlQWidget = new QWidget(pSendQWidget);
	pSendControlQWidget->setContentsMargins(0, 0, 0, 0);
	QHBoxLayout *pSendControlLayout = new QHBoxLayout(pSendControlQWidget);
	pSendControlLayout->setSpacing(8);
	pSendControlLayout->setContentsMargins(12, 32, 12, 12);
	pSendControlQWidget->setLayout(pSendControlLayout);
	
	QWidget *pSendEditWidget = new QWidget(pSendQWidget);
	pSendEditWidget->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *pSendEditLayout = new QVBoxLayout(pSendEditWidget);
	pSendEditLayout->setSpacing(8);
	pSendEditLayout->setContentsMargins(0, 0, 0, 0);
	pSendEditWidget->setLayout(pSendEditLayout);
	pSendEditWidget->setMinimumWidth(320);
	pSendEditWidget->setMaximumWidth(320);
	
	
	QWidget *pSendButtonWidget = new QWidget(pSendQWidget);
	pSendButtonWidget->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *pSendButtonLayout = new QVBoxLayout(pSendButtonWidget);
	pSendButtonLayout->setSpacing(8);
	pSendButtonLayout->setContentsMargins(0, 0, 0, 0);
	pSendButtonWidget->setLayout(pSendButtonLayout);
	pSendButtonWidget->setFixedWidth(160);
	pSendButtonWidget->setMinimumHeight(160);
	pSendButtonWidget->setMaximumWidth(160);
	
	// 功能使能按钮
	pSendMultEnableCheckBox = new QCheckBox(pSendEditWidget);     // 多条发送
	pSendHexEnableCheckBox = new QCheckBox(pSendEditWidget);     // 16进制
	pSendNewLineEnableCheckBox = new QCheckBox(pSendEditWidget); // 加入换行
	pSendTimerEnableCheckBox = new QCheckBox(pSendEditWidget);     // 定时发送
	pSendEnterEnableCheckBox = new QCheckBox(pSendEditWidget);      // 使能enter键发送
	{
		pSendMultEnableCheckBox->setMinimumWidth(120);
		pSendHexEnableCheckBox->setMinimumWidth(120);
		pSendNewLineEnableCheckBox->setMinimumWidth(120);
		pSendTimerEnableCheckBox->setMinimumWidth(120);
		// pSendEnterEnableCheckBox->setMinimumWidth(120);
		
		pSendMultEnableCheckBox->setMaximumWidth(120);
		pSendHexEnableCheckBox->setMaximumWidth(120);
		pSendNewLineEnableCheckBox->setMaximumWidth(120);
		pSendTimerEnableCheckBox->setMaximumWidth(120);
		
		// pSendEnterEnableCheckBox->setMaximumWidth(120);
		pSendMultEnableCheckBox->setText(tr("多条发送"));
		pSendHexEnableCheckBox->setText(tr("Hex发送"));
		pSendNewLineEnableCheckBox->setText(tr("追加换行"));
		pSendTimerEnableCheckBox->setText(tr("定时发送"));
		
		pSendEnterEnableCheckBox->setMinimumHeight(32);
		pSendEnterEnableCheckBox->setText(tr("回车发送"));
	}
	
	pSendTcpAddrCombox = new QComboBox(pSendEditWidget); // TCP Server模式
	{
		pSendTcpAddrCombox->setEditable(true);
		pSendTcpAddrCombox->setView(new QListView(pSendTcpAddrCombox));
		pSendTcpAddrCombox->setMinimumHeight(32);
		pSendTcpAddrCombox->setValidator(new QRegExpValidator(QRegExp(RegExpIpAddr), pSendTcpAddrCombox));
		pSendTcpAddrCombox->addItem("192.168.0.1");
	}
	pSendTcpPortCombox = new QComboBox(pSendEditWidget); // TCP Server模式
	{
		pSendTcpPortCombox->setEditable(true);
		pSendTcpPortCombox->setView(new QListView(pSendTcpPortCombox));
		pSendTcpPortCombox->setMinimumHeight(32);
		pSendTcpPortCombox->setValidator(new QRegExpValidator(QRegExp(RegExpIpPort), pSendTcpPortCombox));
		pSendTcpPortCombox->addItem("32401");
	}
	
	QWidget *pSendTimerWidget = new QWidget(pSendEditWidget);
	QSpinBox *pSendTimerQSpinBox = new QSpinBox(pSendTimerWidget); // 定时时间
	{
		QHBoxLayout *pSendTimerCtr = new QHBoxLayout(pSendTimerWidget);
		{
			pSendTimerWidget->setMinimumHeight(32);
			
			pSendTimerCtr->setContentsMargins(0, 0, 0, 0);
			pSendTimerQSpinBox->setMinimum(10);
			pSendTimerQSpinBox->setMaximum(10000);
			pSendTimerQSpinBox->setMinimumWidth(112);
			QLabel *pSendTimerUnit = new QLabel(tr("ms/次"), pSendTimerWidget); // 单位
			pSendTimerCtr->addWidget(pSendTimerQSpinBox);
			pSendTimerCtr->addWidget(pSendTimerUnit);
		}
		pSendTimerWidget->setLayout(pSendTimerCtr);
	}
	
	{
		// 第一列
		QWidget *pLineQWidget = new QWidget(pSendEditWidget);
		{
			pLineQWidget->setContentsMargins(0, 0, 0, 0);
			QHBoxLayout *pLineLayout = new QHBoxLayout(pLineQWidget);
			pLineLayout->setSpacing(8);
			pLineLayout->setContentsMargins(0, 0, 0, 0);
			pLineQWidget->setLayout(pLineLayout);
			
			pLineLayout->addWidget(pSendMultEnableCheckBox);
			pLineLayout->addWidget(pSendTcpAddrCombox);
		}
		pSendEditLayout->addWidget(pLineQWidget);
	}
	{
		// 第二列
		QWidget *pLineQWidget = new QWidget(pSendEditWidget);
		{
			pLineQWidget->setContentsMargins(0, 0, 0, 0);
			QHBoxLayout *pLineLayout = new QHBoxLayout(pLineQWidget);
			pLineLayout->setSpacing(8);
			pLineLayout->setContentsMargins(0, 0, 0, 0);
			pLineQWidget->setLayout(pLineLayout);
			pLineLayout->addWidget(pSendHexEnableCheckBox);
			pLineLayout->addWidget(pSendTcpPortCombox);
		}
		pSendEditLayout->addWidget(pLineQWidget);
	}
	
	{
		// 第三列
		QWidget *pLineQWidget = new QWidget(pSendEditWidget);
		{
			pLineQWidget->setContentsMargins(0, 0, 0, 0);
			QHBoxLayout *pLineLayout = new QHBoxLayout(pLineQWidget);
			pLineLayout->setSpacing(8);
			pLineLayout->setContentsMargins(0, 0, 0, 0);
			pLineQWidget->setLayout(pLineLayout);
			pLineLayout->addWidget(pSendNewLineEnableCheckBox);
			pLineLayout->addWidget(pSendEnterEnableCheckBox);
		}
		pSendEditLayout->addWidget(pLineQWidget);
	}
	
	{
		// 第四列
		QWidget *pLineQWidget = new QWidget(pSendEditWidget);
		{
			pLineQWidget->setContentsMargins(0, 0, 0, 0);
			QHBoxLayout *pLineLayout = new QHBoxLayout(pLineQWidget);
			pLineLayout->setSpacing(8);
			pLineLayout->setContentsMargins(0, 0, 0, 0);
			pLineQWidget->setLayout(pLineLayout);
			
			pLineLayout->addWidget(pSendTimerEnableCheckBox);
			pLineLayout->addWidget(pSendTimerWidget);
		}
		pSendEditLayout->addWidget(pLineQWidget);
	}
	pSendEditLayout->addStretch(1);
	
	// 发送的控制
	QPushButton *pSendTextButton = new QPushButton(pSendButtonWidget);
	pSendTextButton->setMinimumHeight(72);
	pSendTextButton->setFixedHeight(72);
	pSendTextButton->setIconSize(QSize(32, 32));
	pSendTextButton->setObjectName("pSendTextButton");
	pSendTextButton->setIcon(ThemeIcon("IconSend3", "DockWidget", 48));
	pSendTextButton->setText(tr("发送数据"));
	QPushButton *pClearTextButton = new QPushButton(pSendButtonWidget);
	pClearTextButton->setMinimumHeight(72);
	pClearTextButton->setFixedHeight(72);
	pClearTextButton->setIconSize(QSize(32, 32));
	pClearTextButton->setObjectName("pClearTextButton");
	pClearTextButton->setIcon(ThemeIcon("IconClean", "DockWidget", 48));
	pClearTextButton->setText(tr("清空数据"));
	pSendButtonLayout->addWidget(pSendTextButton);
	pSendButtonLayout->addWidget(pClearTextButton);
	pSendButtonLayout->addStretch(1);
	
	// 多条发送
	{
		pSendMultEnableCheckBox->setChecked(VersaRegisterGet(eValueEnableSendMult));
		connect(pSendMultEnableCheckBox, &QCheckBox::stateChanged, this, [=](int value)
		{
			VersaRegisterSet(eValueEnableSendMult, pSendMultEnableCheckBox->isChecked());
		});
	}
	
	// hex显示格式
	{
		pSendHexEnableCheckBox->setChecked(VersaRegisterGet(eValueEnableSendHex));
		normalSendEdit->setHexEnable(VersaRegisterGet(eValueEnableSendHex));
		multSendTextEdit->setHexEnable(VersaRegisterGet(eValueEnableSendHex));
		connect(pSendHexEnableCheckBox, &QCheckBox::stateChanged, this, [=](int value)
		{
			VersaRegisterSet(eValueEnableSendHex, pSendHexEnableCheckBox->isChecked());
			normalSendEdit->setHexEnable(pSendHexEnableCheckBox->isChecked());
			multSendTextEdit->setHexEnable(pSendHexEnableCheckBox->isChecked());
		});
	}
	
	// 追加换行
	{
		pSendNewLineEnableCheckBox->setChecked(VersaRegisterGet(eValueEnableSendNewLine));
		normalSendEdit->setNewLineEnable(VersaRegisterGet(eValueEnableSendNewLine));
		multSendTextEdit->setNewLineEnable(VersaRegisterGet(eValueEnableSendNewLine));
		connect(pSendNewLineEnableCheckBox, &QCheckBox::stateChanged, this, [=](int value)
		{
			VersaRegisterSet(eValueEnableSendNewLine, pSendNewLineEnableCheckBox->isChecked());
			normalSendEdit->setNewLineEnable(pSendNewLineEnableCheckBox->isChecked());
			multSendTextEdit->setNewLineEnable(pSendNewLineEnableCheckBox->isChecked());
		});
	}
	
	// Enter 发送
	{
		pSendEnterEnableCheckBox->setChecked(VersaRegisterGet(eValueEnableSendEnter));
		normalSendEdit->setEnterEnable(VersaRegisterGet(eValueEnableSendEnter));
		multSendTextEdit->setEnterEnable(VersaRegisterGet(eValueEnableSendEnter));
		connect(pSendEnterEnableCheckBox, &QCheckBox::stateChanged, this, [=](int value)
		{
			VersaRegisterSet(eValueEnableSendEnter, pSendEnterEnableCheckBox->isChecked());
			normalSendEdit->setEnterEnable(pSendEnterEnableCheckBox->isChecked());
			multSendTextEdit->setEnterEnable(pSendEnterEnableCheckBox->isChecked());
		});
	}
	
	// 编码格式改变
	{
		if (VersaUserConfigByteGet(eVersaTextCodec) != "")
		{
			normalSendEdit->setTextCodec(VersaUserConfigByteGet(eVersaTextCodec));
			multSendTextEdit->setTextCodec(VersaUserConfigByteGet(eVersaTextCodec));
		}
		connect(this, &VersaDockWidget::textCodeChanged, this, [=](const QByteArray &codeText)
		{
			normalSendEdit->setTextCodec(codeText);
			multSendTextEdit->setTextCodec(codeText);
		});
	}
	
	// 发送数据
	connect(multSendTextEdit, &VersaMultSendWidget::readyWriteSignals,
	        VersaPhysicalPort, &VersaPhysicalCom::protocolNoneWrite,
	        Qt::QueuedConnection);
	
	// 清空数据
	connect(pClearTextButton, &QPushButton::released,
	        VersaPhysicalPort,
	        &VersaPhysicalCom::clearAllBuffer,
	        Qt::QueuedConnection);
	
	// 新的连接
	{
		connect(VersaPhysicalPort, &VersaPhysicalCom::newConnectPeerAddress,
		        this, &VersaDockWidget::addPeerAddress,
		        Qt::QueuedConnection);
		
		connect(VersaPhysicalPort, &VersaPhysicalCom::newConnectPeerPort,
		        this, &VersaDockWidget::addPeerPort,
		        Qt::QueuedConnection);
	}
	
	// 发送数据
	connect(normalSendEdit, &VersaNormalSendWidget::readyWriteSignals,
	        VersaPhysicalPort, &VersaPhysicalCom::protocolNoneWrite,
	        Qt::QueuedConnection);
	
	// 发送数据
	connect(pSendTextButton, &QPushButton::released, this,
	        [=]()
	        {
		        normalSendEdit->readyWrite(pSendTcpAddrCombox->currentText(),
		                                   pSendTcpPortCombox->currentText().toInt());
	        });
	
	connect(pSendTcpAddrCombox, &QComboBox::currentTextChanged, this, [=]()
	{
		normalSendEdit->changeTcp(pSendTcpAddrCombox->currentText(),
		                          pSendTcpPortCombox->currentText().toInt());
	});
	connect(pSendTcpPortCombox, &QComboBox::currentTextChanged, this, [=]()
	{
		normalSendEdit->changeTcp(pSendTcpAddrCombox->currentText(),
		                          pSendTcpPortCombox->currentText().toInt());
	});
	normalSendEdit->changeTcp(pSendTcpAddrCombox->currentText(),
	                          pSendTcpPortCombox->currentText().toInt());
	// 定时发送
	pTimerSend = new QTimer(this);
	connect(pTimerSend, &QTimer::timeout, this,
	        [=]()
	        {
		        if (pSendMultEnableCheckBox->isChecked())
		        {
			        if (multSendTextEdit != nullptr)
			        {
				        multSendTextEdit->readyWrite();
			        }
		        }
		        else
		        {
			        normalSendEdit->readyWrite(pSendTcpAddrCombox->currentText(),
			                                   pSendTcpPortCombox->currentText().toInt());
		        }
	        });
	connect(pSendTimerEnableCheckBox, &QCheckBox::released, this, [=]()
	{
		if (pSendTimerEnableCheckBox->isChecked())
		{
			pTimerSend->stop();
			pSendTimerQSpinBox->setEnabled(false);
			pTimerSend->setInterval(pSendTimerQSpinBox->value());
			pTimerSend->setTimerType(Qt::PreciseTimer);
			pTimerSend->start();
		}
		else
		{
			pTimerSend->stop();
			pSendTimerQSpinBox->setEnabled(true);
		}
	});
	
	pSendControlLayout->addWidget(pSendEditWidget);
	pSendControlLayout->addWidget(pSendButtonWidget);
	pSendLayout->addWidget(pSendTabWidget);
	pSendLayout->addWidget(pSendControlQWidget);
	
	// 加载数据
	normalSendEdit->loadDataBuffer(VersaUserConfigByteGet(eByteNormalWidgetSend));
	multSendTextEdit->loadDataBuffer(VersaUserConfigByteGet(eByteMultWidgetSend), 128);
	dockSendWidget = pSendCDockWidget;
}

// 接收数据窗口
void VersaDockWidget::createReceiveWidget(void)
{
	// 创建一个可移动窗口 pReceiveCDockWidget
	ads::CDockWidget *pReceiveCDockWidget = new ads::CDockWidget(tr("接收"));
	pReceiveCDockWidget->setStyleSheet(ThemeStyleQss());
	pReceiveCDockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
	pReceiveCDockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
	// pReceiveCDockWidget->setFont(ThemeQFont("FontNormal"));
	pReceiveCDockWidget->setContentsMargins(0, 0, 0, 0);
	// pReceiveCDockWidget->setFont(ThemeQFont("Roboto_8"));
	pReceiveCDockWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
	
	// 创建一个内容窗口 pReceiveQWidget 父元素为 pReceiveCDockWidget
	QWidget *pReceiveQWidget = new QWidget(pReceiveCDockWidget);
	pReceiveQWidget->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *receiveWidgetLayout = new QVBoxLayout(pReceiveQWidget);
	receiveWidgetLayout->setSpacing(0);
	receiveWidgetLayout->setContentsMargins(0, 0, 0, 0);
	pReceiveQWidget->setLayout(receiveWidgetLayout);
	
	// 0.创建窗口设置 pReceiveControlQWidget 父元素为 pReceiveQWidget
	QWidget *pReceiveControlQWidget = new QWidget(pReceiveQWidget);
	pReceiveControlQWidget->setContentsMargins(0, 0, 0, 0);
	pReceiveControlQWidget->setMaximumHeight(32);
	
	// 1.创建接收数据显示窗口（不可编辑）  pReceiveQTextEdit 父元素为 pReceiveQWidget
	pReceiveQTextEdit = new VersaReceiveWidget(pReceiveQWidget);
	{
		pReceiveQTextEdit->setContentsMargins(0, 0, 0, 0);
	}
	pReceiveQTextEdit->setTextBuffer(&VersaPhysicalPort->mVersaReadData);
	
	// 编码格式改变
	connect(this, &VersaDockWidget::textCodeChanged,
	        pReceiveQTextEdit, &VersaReceiveWidget::setTextCodec);
	if (VersaUserConfigByteGet(eVersaTextCodec) != "")
	{
		pReceiveQTextEdit->setTextCodec(VersaUserConfigByteGet(eVersaTextCodec));
	}
	
	connect(VersaPhysicalPort, &VersaPhysicalCom::refreshReadBuffer,
	        pReceiveQTextEdit, &VersaReceiveWidget::refreshBuffer,
	        Qt::QueuedConnection);
	
	{
		QHBoxLayout *pReceiveSettingLayout = new QHBoxLayout(pReceiveControlQWidget);
		pReceiveSettingLayout->setSpacing(0);
		pReceiveSettingLayout->setContentsMargins(0, 0, 0, 0);
		pReceiveControlQWidget->setLayout(pReceiveSettingLayout);
		
		QCheckBox *pReceiveHexEnableCheckBox = new QCheckBox(pReceiveControlQWidget);          // 16进制格式
		QCheckBox *pReceiveTimestampEnableCheckBox = new QCheckBox(pReceiveControlQWidget);      // 时间戳
		QCheckBox *pReceiveShowIpAddrEnaableCheckBox = new QCheckBox(pReceiveControlQWidget); // 关键字高亮
		QCheckBox *pReceiveShowSendTextCheckBox = new QCheckBox(pReceiveControlQWidget);      // 显示 Tx 数据
		
		pReceiveHexEnableCheckBox->setText("Hex显示");
		pReceiveHexEnableCheckBox->setChecked(VersaRegisterGet(eValueEnableReceiveHex));
		connect(pReceiveHexEnableCheckBox, &QCheckBox::stateChanged, this, [=]()
		{
			VersaRegisterSet(eValueEnableReceiveHex, pReceiveHexEnableCheckBox->isChecked());
			pReceiveQTextEdit->setHexEnable(pReceiveHexEnableCheckBox->isChecked());
		});
		
		pReceiveTimestampEnableCheckBox->setText("时间戳");
		pReceiveTimestampEnableCheckBox->setChecked(VersaRegisterGet(eValueEnableReceiveTimestamp));
		connect(pReceiveTimestampEnableCheckBox, &QCheckBox::stateChanged, this, [=]()
		{
			VersaRegisterSet(eValueEnableReceiveTimestamp, pReceiveTimestampEnableCheckBox->isChecked());
			QMetaObject::invokeMethod(VersaPhysicalPort, "setTimestampEnabel",
			                          Qt::QueuedConnection,
			                          Q_ARG(bool, pReceiveTimestampEnableCheckBox->isChecked()));
		});
		QMetaObject::invokeMethod(VersaPhysicalPort, "setTimestampEnabel",
		                          Qt::QueuedConnection,
		                          Q_ARG(bool, pReceiveTimestampEnableCheckBox->isChecked()));
		pReceiveShowIpAddrEnaableCheckBox->setText("显示IP地址");
		pReceiveShowIpAddrEnaableCheckBox->setChecked(VersaRegisterGet(eValueEnableReceiveShowIpAddr));
		connect(pReceiveShowIpAddrEnaableCheckBox, &QCheckBox::stateChanged, this, [=]()
		{
			VersaRegisterSet(eValueEnableReceiveShowIpAddr, pReceiveShowIpAddrEnaableCheckBox->isChecked());
			QMetaObject::invokeMethod(VersaPhysicalPort, "setShowIpAddrEnaabl",
			                          Qt::QueuedConnection,
			                          Q_ARG(bool, pReceiveShowIpAddrEnaableCheckBox->isChecked()));
		});
		QMetaObject::invokeMethod(VersaPhysicalPort, "setShowIpAddrEnaabl",
		                          Qt::QueuedConnection,
		                          Q_ARG(bool, pReceiveShowIpAddrEnaableCheckBox->isChecked()));
		pReceiveShowSendTextCheckBox->setText("显示Tx");
		pReceiveShowSendTextCheckBox->setChecked(VersaRegisterGet(eValueEnableReceiveShowSendText));
		connect(pReceiveShowSendTextCheckBox, &QCheckBox::stateChanged, this, [=]()
		{
			VersaRegisterSet(eValueEnableReceiveShowSendText, pReceiveShowSendTextCheckBox->isChecked());
			QMetaObject::invokeMethod(VersaPhysicalPort, "setShowSendTextEnabel",
			                          Qt::QueuedConnection,
			                          Q_ARG(bool, pReceiveShowSendTextCheckBox->isChecked()));
		});
		QMetaObject::invokeMethod(VersaPhysicalPort, "setShowSendTextEnabel",
		                          Qt::QueuedConnection,
		                          Q_ARG(bool, pReceiveShowSendTextCheckBox->isChecked()));
		pReceiveSettingLayout->addWidget(pReceiveHexEnableCheckBox);
		pReceiveSettingLayout->addWidget(pReceiveTimestampEnableCheckBox);
		pReceiveSettingLayout->addWidget(pReceiveShowIpAddrEnaableCheckBox);
		pReceiveSettingLayout->addWidget(pReceiveShowSendTextCheckBox);
		pReceiveSettingLayout->addStretch(1);
	}
	
	receiveWidgetLayout->addWidget(pReceiveControlQWidget);
	receiveWidgetLayout->addWidget(pReceiveQTextEdit);
	
	pReceiveCDockWidget->setWidget(pReceiveQWidget);
	receiveWidget = pReceiveCDockWidget;
}

void VersaDockWidget::createProtocolWaveformWidget(void)
{
	// 创建一个可移动窗口 pProtocolWaveformCDockWidget
	ads::CDockWidget *pProtocolWaveformCDockWidget = new ads::CDockWidget(tr("波形"));
	pProtocolWaveformCDockWidget->setStyleSheet(ThemeStyleQss());
	pProtocolWaveformCDockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
	pProtocolWaveformCDockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
	pProtocolWaveformCDockWidget->setContentsMargins(0, 0, 0, 0);
	pProtocolWaveformCDockWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
	// protocolWaveformWidget->setFont(ThemeQFont("FontNormal"));
	
	// 创建一个内容窗口 ProtocolWaveform 父元素为 pProtocolWaveformCDockWidget
	QWidget *ProtocolWaveform = new QWidget(pProtocolWaveformCDockWidget);
	ProtocolWaveform->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *pProtocolWaveformWidgetLayout = new QVBoxLayout(ProtocolWaveform);
	pProtocolWaveformWidgetLayout->setContentsMargins(0, 0, 0, 0);
	ProtocolWaveform->setLayout(pProtocolWaveformWidgetLayout);
	
	pVseraQCustomPlot = new VersaQCustomPlot(ProtocolWaveform,
	                                         VersaRegisterGet(eValueEnableOpenGl));
	pProtocolWaveformWidgetLayout->addWidget(pVseraQCustomPlot);
	
	pProtocolWaveformCDockWidget->setWidget(ProtocolWaveform);
	protocolWaveformWidget = pProtocolWaveformCDockWidget;
}

void VersaDockWidget::createOpenGLWidget(void)
{
	// 创建一个可移动窗口 openglCDockWidget
	ads::CDockWidget *pVersaRobotCDockWidget = new ads::CDockWidget(tr("姿态"));
	pVersaRobotCDockWidget->setStyleSheet(ThemeStyleQss());
	pVersaRobotCDockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
	pVersaRobotCDockWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
	// pVersaRobotCDockWidget->setFont(ThemeQFont("FontNormal"));
	pVersaRobotCDockWidget->setContentsMargins(0, 0, 0, 0);
	pVersaRobotCDockWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
	// 创建一个内容窗口 ProtocolWaveform 父元素为 openglCDockWidget
	QWidget *pVseraRobotWidget = new QWidget(pVersaRobotCDockWidget);
	pVseraRobotWidget->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *pVersaRobotCDockWidgetLayout = new QVBoxLayout(pVseraRobotWidget);
	pVersaRobotCDockWidgetLayout->setContentsMargins(0, 0, 0, 0);
	pVseraRobotWidget->setLayout(pVersaRobotCDockWidgetLayout);
	QByteArray dirpathByte = VersaUserConfigByteGet(eVersaAppModeFile);
	QString dirpath = QString::fromUtf8(dirpathByte);
	pVseraRobot = new VersaRobotWidget(pVseraRobotWidget, dirpath);
	pVersaRobotCDockWidgetLayout->addWidget(pVseraRobot);
	pVersaRobotCDockWidget->setWidget(pVseraRobotWidget);
	vseraRobotWidget = pVersaRobotCDockWidget;
}

void VersaDockWidget::createRealtimeProtocolWidget(void)
{
}

void VersaDockWidget::createProtocolDataWidget(void)
{
}

void VersaDockWidget::createProtocolWidget(void)
{
	// 创建一个可移动窗口 pProtocolSendWidget
	{
		ads::CDockWidget *pProtocolSendWidget = new ads::CDockWidget(tr("协议发送"));
		pProtocolSendWidget->setStyleSheet(ThemeStyleQss());
		pProtocolSendWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
		pProtocolSendWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
		// pProtocolSendWidget->setFont(ThemeQFont("FontNormal"));
		pProtocolSendWidget->setContentsMargins(0, 0, 0, 0);
		pProtocolSendWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
		// 创建一个内容窗口 ProtocolWaveform 父元素为 openglCDockWidget
		pVersaProtocolSend = new VersaProtocolWidget(VersaUserConfigByteGet(eByteProtocolSend),
		                                             VersaProtocolWidget::SendProtocolWidget,
		                                             this);
		pVersaProtocolSend->setContentsMargins(0, 0, 0, 0);
		pProtocolSendWidget->setWidget(pVersaProtocolSend);
		protocolSendWidget = pProtocolSendWidget;
	}
	
	{
		ads::CDockWidget *pProtocolReadWidget = new ads::CDockWidget(tr("协议接收"));
		pProtocolReadWidget->setStyleSheet(ThemeStyleQss());
		pProtocolReadWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
		pProtocolReadWidget->setFeature(ads::CDockWidget::DockWidgetPinnable, false);
		// protocolReadWidget->setFont(ThemeQFont("FontNormal"));
		pProtocolReadWidget->setContentsMargins(0, 0, 0, 0);
		pProtocolReadWidget->setIcon(ThemeIcon("IconDockWidget", "AppBackgroundColor", 32));
		// 创建一个内容窗口 ProtocolWaveform 父元素为 openglCDockWidget
		pVersaProtocolRead = new VersaProtocolWidget(VersaUserConfigByteGet(eByteProtocolRead),
		                                             VersaProtocolWidget::ReadProtocolWidget,
		                                             this);
		pVersaProtocolRead->setContentsMargins(0, 0, 0, 0);
		pProtocolReadWidget->setWidget(pVersaProtocolRead);
		protocolReadWidget = pProtocolReadWidget;
	}
	
	VersaPhysicalPort->setProtocolTxStruct(&pVersaProtocolSend->protocolStruct);
	VersaPhysicalPort->setProtocolRxStruct(&pVersaProtocolRead->protocolStruct);
	
	connect(pVersaProtocolSend, &VersaProtocolWidget::protocolVersa,
	        VersaPhysicalPort, &VersaPhysicalCom::protocolVersaWrite,
	        Qt::QueuedConnection);
	
	connect(VersaPhysicalPort, &VersaPhysicalCom::refreshProtocolRx,
	        pVersaProtocolRead, &VersaProtocolWidget::updataReadSlots,
	        Qt::QueuedConnection);
	
	// 波形
	connect(pVersaProtocolRead, &VersaProtocolWidget::addGraphsData,
	        pVseraQCustomPlot, &VersaQCustomPlot::addGraphsData,
	        Qt::QueuedConnection);
	
	// 姿态
	connect(pVersaProtocolRead, &VersaProtocolWidget::addGraphsData,
	        pVseraRobot, &VersaRobotWidget::setNodeRobotValue,
	        Qt::QueuedConnection);
	
	// 新的连接
	connect(VersaPhysicalPort, &VersaPhysicalCom::newConnectPeerAddress,
	        pVersaProtocolSend, &VersaProtocolWidget::addPeerAddress,
	        Qt::QueuedConnection);
	
	connect(VersaPhysicalPort, &VersaPhysicalCom::newConnectPeerPort,
	        pVersaProtocolSend, &VersaProtocolWidget::addPeerPort,
	        Qt::QueuedConnection);
}

void VersaDockWidget::createHelpWidget(void)
{
}

void VersaDockWidget::createAboutWidget(void)
{
}

void VersaDockWidget::createProtocolContentWidget(void)
{
}
