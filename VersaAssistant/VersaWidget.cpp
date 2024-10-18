/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaWidget.cpp
 * @Description  : 
 */
#include "VersaWidget.h"
#include "VersaConfig.h"
#include "VersaMessageBox.h"
#include "VersaQuickWidget.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QEvent>
#include <QHoverEvent>
#include "VersaPhysicalCom.h"

VersaWidget::VersaWidget(QWidget *parent,
                         bool resizeEnable,
                         bool shadowBorder,
                         bool winNativeEvent) : QFramelessWidget(parent,
                                                                 resizeEnable,
                                                                 shadowBorder,
                                                                 winNativeEvent)
{
    restoreVersaInfo();
    pVersaLicenseWidget = new VersaLicenseWidget(nullptr, VersaUserLicense());
    pVersaAboutAppWidget = new VersaAboutApp(nullptr);
    // 创建主窗口
    {
        pMainVersaWidget = new QWidget(this);
        pMainVersaWidget->setObjectName("MainVersaWidget");
        pMainVersaWidget->setContentsMargins(1, 1, 1, 1);
        QVBoxLayout *pMainVersaBoxLayout = new QVBoxLayout();
        pMainVersaBoxLayout->setSpacing(0);
        pMainVersaBoxLayout->setContentsMargins(1, 1, 1, 1);
        pMainVersaBoxLayout->addWidget(pMainVersaWidget);
        this->setLayout(pMainVersaBoxLayout);
    }
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    pMainVersaWidget->setLayout(gridLayout);

    // 添加任务栏窗口
    {
        pVersaAppBar = new VersaAppBar(64, 48, pMainVersaWidget);
        pVersaAppBar->setContentsMargins(0, 0, 0, 0);
        this->framelessHelper()->setTitleBar(pVersaAppBar);

        gridLayout->addWidget(pVersaAppBar, 0, 0, 1, 2);
        // 信号与槽
        {
            connect(pVersaAppBar->minPushButton,
                    &QPushButton::clicked,
                    this,
                    &VersaWidget::on_btnMin_clicked);

            connect(pVersaAppBar->maxPushButton,
                    &QPushButton::clicked,
                    this,
                    &VersaWidget::on_btnMax_clicked);

            connect(pVersaAppBar->closePushButton,
                    &QPushButton::clicked,
                    this,
                    &VersaWidget::on_btnExit_clicked);

            connect(framelessHelper(),
                    &QFramelessHelper::maximizedChanged,
                    pVersaAppBar,
                    &VersaAppBar::slotMaximizedChanged);
        }
    }
    // 添加侧边栏按钮窗口
    {
        pVersaTabBar = new VersaTabBar(pMainVersaWidget);
        pVersaTabBar->setContentsMargins(0, 0, 0, 0);
        gridLayout->addWidget(pVersaTabBar, 1, 0, 1, 1);
    }
    // 添加主内容窗口
    {
        pVersaWidget = new QWidget(pMainVersaWidget);
        pVersaWidget->setContentsMargins(0, 0, 0, 0);
        gridLayout->addWidget(pVersaWidget, 1, 1, 1, 1);

        // 添加遮蔽和主内容窗口
        {
            QVBoxLayout *pVersaDockLayout = new QVBoxLayout();
            pVersaDockLayout->setSpacing(0);
            pVersaDockLayout->setContentsMargins(0, 0, 0, 0);
            pVersaWidget->setLayout(pVersaDockLayout);
            // 主内容窗口
            {
                pVersaDockWidget = new VersaDockWidget(pVersaWidget);
                pVersaDockWidget->setContentsMargins(0, 0, 0, 0);
                pVersaDockLayout->addWidget(pVersaDockWidget);
            }
            // 滑出菜单窗口
            {
                pVersaSideWidget = new VersaSideWidget(VersaUserConfigByteGet(eVersaSideBarInfo), pVersaWidget);
                pVersaSideWidget->setContentsMargins(0, 0, 0, 0);
                pVersaSideWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
                // 信号与槽
                {
                    connect(pVersaSideWidget, &VersaSideWidget::showSidebarStart,
                            pVersaDockWidget,
                            &VersaDockWidget::maskWidgetEnable);
                    connect(pVersaSideWidget, &VersaSideWidget::hideSidebarEnd,
                            pVersaDockWidget,
                            &VersaDockWidget::maskWidgetDisable);
                }
            }
        }
    }

    // 信号与槽
    {
        connect(pVersaTabBar, &VersaTabBar::currentIndexChanged,
                pVersaSideWidget, &VersaSideWidget::setStackedLayout,
                Qt::QueuedConnection);

        connect(pVersaTabBar, &VersaTabBar::currentIndexChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setPhysicaIndex,
                Qt::QueuedConnection);

        connect(pVersaTabBar, &VersaTabBar::openReleased,
                VersaPhysicalPort, &VersaPhysicalCom::toggleOpenReleased,
                Qt::QueuedConnection);

        connect(pVersaTabBar, &VersaTabBar::protocolEnableChanged,
                VersaPhysicalPort, &VersaPhysicalCom::protocolEnableChanged,
                Qt::QueuedConnection);

        connect(VersaPhysicalPort, &VersaPhysicalCom::refreshOpenState,
                pVersaTabBar, &VersaTabBar::refreshOpenState,
                Qt::QueuedConnection);

        connect(VersaPhysicalPort, &VersaPhysicalCom::refreshOpenState,
                pVersaSideWidget, &VersaSideWidget::refreshOpenState,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::serialIndexChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setSerialIndex,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::serialBaudRateChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setSerialBaudRate,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::serialStopBitsChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setSerialStopBits,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::serialDataBitsChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setSerialDataBits,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::serialParrityChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setSerialParrity,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::serialFlowControlChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setSerialFlowControl,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::internetAddress,
                VersaPhysicalPort, &VersaPhysicalCom::setInternetAddress,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::internetPort,
                VersaPhysicalPort, &VersaPhysicalCom::setInternetPort,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::internetType,
                VersaPhysicalPort, &VersaPhysicalCom::setInternetIndex,
                Qt::QueuedConnection);

        connect(VersaPhysicalPort, &VersaPhysicalCom::physicaLibusbChanged,
                pVersaSideWidget, &VersaSideWidget::setPhysicaLibusbCombox,
                Qt::QueuedConnection);

        connect(VersaPhysicalPort, &VersaPhysicalCom::physicaInternetChanged,
                pVersaSideWidget, &VersaSideWidget::setPhysicaInternetCombox,
                Qt::QueuedConnection);

        connect(VersaPhysicalPort, &VersaPhysicalCom::physicaSerialChanged,
                pVersaSideWidget, &VersaSideWidget::setPhysicaSerialCombox,
                Qt::QueuedConnection);

        connect(pVersaSideWidget, &VersaSideWidget::usbIndexChanged,
                VersaPhysicalPort, &VersaPhysicalCom::setLibusbIndex,
                Qt::QueuedConnection);

        VersaPhysicalPort->enableReady(true);
    }

    configMenuBar();

    refreshTimer.setInterval(100);
    refreshTimer.setTimerType(Qt::PreciseTimer);
    refreshTimer.stop();
    connect(&refreshTimer, &QTimer::timeout,
            this, &VersaWidget::timeout);
    refreshTimer.start();

    restoreVersaWidgetUi();
    isInitReady = true;
}

VersaWidget::~VersaWidget()
{
    if (isInitReady == true)
    {
        isInitReady = false;
        refreshTimer.stop();
        saveVersaInfo();
        VersaFree();
    }
}

void VersaWidget::VersaFree()
{
    isInitReady = false;
    VersaConfigFree();
    VersaMessageFree;
    VersaPhysicalPortFree;

    if (!pVersaAboutAppWidget.isNull())
    {
        pVersaAboutAppWidget->deleteLater();
        pVersaAboutAppWidget = nullptr;
    }
    if (!pVersaLicenseWidget.isNull())
    {
        pVersaLicenseWidget->deleteLater();
        pVersaLicenseWidget = nullptr;
    }

    if (!pVersaWidget.isNull())
    {
        pVersaWidget->deleteLater();
        pVersaWidget = nullptr;
    }

    if (!pMainVersaWidget.isNull())
    {
        pMainVersaWidget->deleteLater();
        pMainVersaWidget = nullptr;
    }

    if (!pVersaAppBar.isNull())
    {
        pVersaAppBar->deleteLater();
        pVersaAppBar = nullptr;
    }

    if (!pVersaTabBar.isNull())
    {
        pVersaTabBar->deleteLater();
        pVersaTabBar = nullptr;
    }

    // if (pVersaStateBar != nullptr)
    // {
    // 	qDebug() << "pVersaStateBar";
    // 	pVersaStateBar->deleteLater();
    // 	pVersaStateBar = nullptr;
    // }

    if (!pVersaDockWidget.isNull())
    {
        pVersaDockWidget->deleteLater();
        pVersaDockWidget = nullptr;
    }

    if (!pVersaSideWidget.isNull())
    {
        pVersaSideWidget->deleteLater();
        pVersaSideWidget = nullptr;
    }
}

void VersaWidget::configMenuBar()
{
    QMenu *fileMenu = pVersaAppBar->menuBar->addMenu(tr("文件"));
    fileMenu->setParent(pVersaAppBar->menuBar);
    QMenu *setupMenu = pVersaAppBar->menuBar->addMenu(tr("设置"));
    setupMenu->setParent(pVersaAppBar->menuBar);
    QMenu *viewMenu = pVersaAppBar->menuBar->addMenu(tr("窗口"));
    viewMenu->setParent(pVersaAppBar->menuBar);
    QAction *pactAction = pVersaAppBar->menuBar->addAction(tr("协议"));
    pactAction->setParent(pVersaAppBar->menuBar);
    QAction *helpAction = pVersaAppBar->menuBar->addAction(tr("帮助"));
    helpAction->setParent(pVersaAppBar->menuBar);
    QMenu *aboutMenu = pVersaAppBar->menuBar->addMenu(tr("关于"));
    aboutMenu->setParent(pVersaAppBar->menuBar);
    // 文件
    {
        QAction *pSaveDate = fileMenu->addAction(tr("保存原始数据"),
                                                 VersaPhysicalPort,
                                                 &VersaPhysicalCom::saveReceiveData,
                                                 Qt::QueuedConnection);

        QAction *pOpenDate = fileMenu->addAction(tr("导入原始数据"),
                                                 VersaPhysicalPort,
                                                 &VersaPhysicalCom::readReceiveFileData,
                                                 Qt::QueuedConnection);

        connect(
                VersaPhysicalPort, &VersaPhysicalCom::newQMessageBox, this,
                [=](const QString &tittle, const QString &message)
                {
                    QMessageBox::critical(nullptr, tittle, message);
                },
                Qt::QueuedConnection);

        fileMenu->addSeparator();
        QAction *pSaveWaveformData = fileMenu->addAction(tr("保存波形数据"),
                                                         this->pVersaDockWidget->pVseraQCustomPlot.data(),
                                                         &VersaQCustomPlot::saveWaveformData);
        QAction *pSaveWaveformBmp = fileMenu->addAction(tr("保存波形图片"),
                                                        this->pVersaDockWidget->pVseraQCustomPlot.data(),
                                                        &VersaQCustomPlot::saveWaveformImage);
        QAction *pOpenWaveformData = fileMenu->addAction(tr("导入波形数据"),
                                                         this->pVersaDockWidget->pVseraQCustomPlot.data(),
                                                         &VersaQCustomPlot::readWaveformData);
        fileMenu->addSeparator();
        QAction *pSendFile = fileMenu->addAction(tr("发送文件"),
                                                 this->pVersaDockWidget.data(),
                                                 &VersaDockWidget::sendFileText);
        // 清空统计
        QAction *pCleanStatistical = fileMenu->addAction(tr("清空统计"),
                                                         VersaPhysicalPort,
                                                         &VersaPhysicalCom::clearStatistics,
                                                         Qt::QueuedConnection);
    }

    {
        // 列举主题
        QMenu *pThemeMenu = setupMenu->addMenu(tr("主题"));
        QActionGroup *pThemeGroup = new QActionGroup(pThemeMenu);
        {
            QStringList themeListName = VersaUser()->getThemeList();
            for (int i = 0; i < themeListName.count(); ++i)
            {
                QString fileName = themeListName.at(i);
                QAction *pFileNameAction = new QAction(fileName, pThemeMenu);
                pFileNameAction->setCheckable(true);
                pThemeGroup->addAction(pFileNameAction);
                pThemeMenu->addAction(pFileNameAction);
                if ((qApp->applicationDirPath() + "/theme/" + fileName) == VersaUser()->UserThemeFile)
                {
                    pFileNameAction->setChecked(true);
                }
            }
            connect(pThemeGroup, &QActionGroup::triggered, this, [=](QAction *action)
            {
                if (action->text() != "")
                {
                    VersaUser()->changeUserThemeFile(qApp->applicationDirPath() + "/theme/" + action->text());
                }
            });
        }

        QMenu *pCodeMenu = setupMenu->addMenu(tr("编码格式"));
        QActionGroup *pTextCodecGroup = new QActionGroup(pCodeMenu);
        {

            if (VersaUserConfigByteGet(eVersaTextCodec) == "")
            {
                VersaUserConfigByteSet(eVersaTextCodec, "UTF-8");
                saveVersaInfo();
            }
            QList<QByteArray> listLableCodecs = QTextCodec::availableCodecs();
            for (int iCodecIndex = 0; iCodecIndex < listLableCodecs.count(); iCodecIndex++)
            {
                QAction *pAction = new QAction(listLableCodecs[iCodecIndex], pCodeMenu);
                pAction->setCheckable(true);
                pTextCodecGroup->addAction(pAction);
                pCodeMenu->addAction(pAction);

                if (VersaUserConfigByteGet(eVersaTextCodec) == listLableCodecs[iCodecIndex])
                {
                    pAction->setChecked(true);
                }

                if ((iCodecIndex == 0) && (VersaUserConfigByteGet(eVersaTextCodec) == ""))
                {
                    pAction->setChecked(true);
                }
            }
            connect(pTextCodecGroup, &QActionGroup::triggered,
                    this, [=](QAction *action)
                    {
                        VersaUserConfigByteSet(eVersaTextCodec, action->text().toUtf8());
                        this->pVersaDockWidget->setTextCode(action->text().toUtf8());
                        saveVersaInfo();
                    });
        }
        QMenu *pLayoutMenu = setupMenu->addMenu(tr("界面配置"));
        {
            // 创建自定义布局设置，此方式可以设置控件的数量
            QMenu *pLayoutCustomize = pLayoutMenu->addMenu(tr("自定义"));
            {
                QMenu *maxProtocolSizeQMenu = pLayoutCustomize->addMenu(tr("协议最大解析长度"));
                {
                    QWidget *maxProtocolSizeWidget = new QWidget(maxProtocolSizeQMenu);
                    QHBoxLayout *maxProtocolSizeLayout = new QHBoxLayout(maxProtocolSizeWidget);
                    maxProtocolSizeLayout->setSpacing(0);
                    maxProtocolSizeLayout->setContentsMargins(0, 0, 0, 0);
                    maxProtocolSizeWidget->setLayout(maxProtocolSizeLayout);

                    QSpinBox *maxProtocolSizeSpinBox = new QSpinBox(maxProtocolSizeWidget);
                    maxProtocolSizeSpinBox->setMinimum(0);
                    maxProtocolSizeSpinBox->setMaximum(28);
                    maxProtocolSizeSpinBox->setValue(28);
                    maxProtocolSizeLayout->addWidget(maxProtocolSizeSpinBox);

                    QPushButton *maxProtocolSizeButton = new QPushButton(maxProtocolSizeWidget);
                    maxProtocolSizeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    maxProtocolSizeLayout->addWidget(maxProtocolSizeButton);

                    QWidgetAction *maxProtocolSizeWidgetAction = new QWidgetAction(maxProtocolSizeQMenu);
                    maxProtocolSizeWidgetAction->setDefaultWidget(maxProtocolSizeWidget);
                    maxProtocolSizeQMenu->addAction(maxProtocolSizeWidgetAction);

                    connect(maxProtocolSizeButton, &QPushButton::released, this, [=]()
                    {
                        int maxNum = maxProtocolSizeSpinBox->value();
                        if (pVersaDockWidget->pVersaProtocolRead != nullptr)
                        {
                            pVersaDockWidget->pVersaProtocolRead->resizeProtocolTable(maxNum);
                        }
                        if (pVersaDockWidget->pVersaProtocolSend != nullptr)
                        {
                            pVersaDockWidget->pVersaProtocolSend->resizeProtocolTable(maxNum);
                        }
                        VersaUserConfigByteSet(eByteProtocolRead,
                                               pVersaDockWidget->pVersaProtocolRead->getSaveDataBuffer());
                        VersaUserConfigByteSet(eByteProtocolSend,
                                               pVersaDockWidget->pVersaProtocolSend->getSaveDataBuffer());
                        saveVersaInfo();
                    });
                }

                QMenu *maxMultSendSize = pLayoutCustomize->addMenu(tr("多条发送编辑数量"));
                {
                    QWidget *maxMultSizeWidget = new QWidget(maxMultSendSize);
                    QHBoxLayout *maxMultSizeLayout = new QHBoxLayout(maxMultSizeWidget);
                    maxMultSizeLayout->setSpacing(0);
                    maxMultSizeLayout->setContentsMargins(0, 0, 0, 0);
                    maxMultSizeWidget->setLayout(maxMultSizeLayout);

                    QSpinBox *maxMultSizeSpinBox = new QSpinBox(maxMultSizeWidget);
                    maxMultSizeSpinBox->setMinimum(0);
                    maxMultSizeSpinBox->setMaximum(128);
                    maxMultSizeSpinBox->setValue(128);
                    maxMultSizeLayout->addWidget(maxMultSizeSpinBox);

                    QPushButton *maxMultSizeButton = new QPushButton(maxMultSizeWidget);
                    maxMultSizeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    maxMultSizeLayout->addWidget(maxMultSizeButton);

                    QWidgetAction *maxMultSizeWidgetAction = new QWidgetAction(maxMultSendSize);
                    maxMultSizeWidgetAction->setDefaultWidget(maxMultSizeWidget);
                    maxMultSendSize->addAction(maxMultSizeWidgetAction);

                    connect(maxMultSizeButton, &QPushButton::released, this, [=]()
                    {
                        int maxNum = maxMultSizeSpinBox->value();
                        pVersaDockWidget->multSendTextEdit->resizeMultSendWidget(maxNum);
                        VersaUserConfigByteSet(eByteMultWidgetSend,
                                               pVersaDockWidget->multSendTextEdit->getSaveDataBuffer());
                        saveVersaInfo();
                    });
                }
            }
            QAction *pModeSelect = pLayoutMenu->addAction(tr("设置模型配置"));
            connect(pModeSelect, &QAction::triggered, this, [=]()
            {
                QString dirpath = QFileDialog::getOpenFileName(this, QStringLiteral("设置模型配置"),
                                                               qApp->applicationDirPath() + "/model.json",
                                                               QString(tr("*.json")));
                if (dirpath != NULL)
                {
                    QByteArray dirpathByte = dirpath.toUtf8();
                    VersaUserConfigByteSet(eVersaAppModeFile, dirpathByte);
                    saveVersaInfo();
                }
            });

            QMenu *pEnableOpenGLMenu = pLayoutMenu->addMenu(tr("设置GPU绘图"));
            QActionGroup *pEnableOpenGLGroup = new QActionGroup(pThemeMenu);
            {
                QAction *pEnableAction = new QAction(tr("启用GPU绘图"), pEnableOpenGLMenu);
                pEnableAction->setCheckable(true);
                pEnableOpenGLGroup->addAction(pEnableAction);
                pEnableOpenGLMenu->addAction(pEnableAction);

                QAction *pDisEnableAction = new QAction(tr("禁用GPU绘图"), pEnableOpenGLMenu);
                pDisEnableAction->setCheckable(true);
                pEnableOpenGLGroup->addAction(pDisEnableAction);
                pEnableOpenGLMenu->addAction(pDisEnableAction);


                // if (VersaRegisterGet(eValueEnableOpenGl) == true)
                // {
                // 	pEnableAction->setChecked(true);
                // }
                // else
                // {
                // 	pDisEnableAction->setChecked(true);
                // }

                connect(pEnableOpenGLGroup, &QActionGroup::triggered, this, [=](QAction *action)
                {
                    if (action->text() == tr("启用GPU绘图"))
                    {
                        int checkIndex = 0;
                        pVersaDockWidget->pVseraQCustomPlot->setOpenGl(true);
                        VersaRegisterSet(eValueEnableOpenGl, true);
                    } else
                    {
                        int checkIndex = 1;
                        pVersaDockWidget->pVseraQCustomPlot->setOpenGl(false);
                        VersaRegisterSet(eValueEnableOpenGl, false);
                    }
                    saveVersaInfo();
                });
            }
            pLayoutMenu->addSeparator();
            // 选择布局
            QMenu *pLayoutSelect = pLayoutMenu->addMenu(tr("选择配置"));
            QActionGroup *pSelectGroup = new QActionGroup(pLayoutSelect);
            {
                QStringList fileNameList = VersaUser()->getConfigList();
                bool isChecked = false;
                for (int i = 0; i < fileNameList.size(); ++i)
                {
                    QString fileName = fileNameList.at(i);
                    QAction *pFileNameAction = new QAction(fileName, pLayoutSelect);
                    pFileNameAction->setCheckable(true);
                    pSelectGroup->addAction(pFileNameAction);
                    pLayoutSelect->addAction(pFileNameAction);
                    if ((qApp->applicationDirPath() + "/config/" + fileName) == VersaUser()->UserConfigFile)
                    {
                        isChecked = true;
                        pFileNameAction->setChecked(true);
                    }
                    // 直到最后一个
                    if ((isChecked == false) && (i == fileNameList.size() - 1))
                    {
                        pFileNameAction->setChecked(true);
                    }
                }
                connect(pSelectGroup, &QActionGroup::triggered, this, [=](QAction *action)
                {
                    if (VersaUser()->getConfigList().count() == pSelectGroup->actions().count())
                    {
                        // 更改配置文件
                        VersaUser()->changeUserConfigFile(qApp->applicationDirPath() + "/config/" + action->text());
                        setupMenu->hide();
                        // 刷新UI
                        restoreVersaWidgetUi();
                        saveVersaInfo();
                    }
                });
            }
            // 存储当前布局
            QAction *pSaveLayout = pLayoutMenu->addAction(tr("存储当前配置"));
            connect(pSaveLayout, &QAction::triggered, this, [=]()
            {
                saveVersaInfo();
            });
            // 存储当先布局为新布局
            QAction *pSaveLayoutAs = pLayoutMenu->addAction(tr("将当前配置存储为"));
            connect(pSaveLayoutAs, &QAction::triggered, this, [=]()
            {
                QString dirpath = QFileDialog::getSaveFileName(this, QStringLiteral("将当前配置存储为"),
                                                               qApp->applicationDirPath() + "/config/config.ini",
                                                               QString(tr("*.ini")));
                if (dirpath != NULL)
                {
                    VersaUser()->UserConfigFile = dirpath;
                    saveVersaInfo();
                    QTimer::singleShot(0, this, [=]()
                    {
                        pLayoutSelect->clear();
                        QStringList fileNameList = VersaUser()->getConfigList();
                        for (int i = 0; i < fileNameList.size(); ++i)
                        {
                            QString fileName = fileNameList.at(i);
                            QAction *pFileNameAction = new QAction(fileName, pLayoutSelect);
                            pFileNameAction->setCheckable(true);
                            pSelectGroup->addAction(pFileNameAction);
                            pLayoutSelect->addAction(pFileNameAction);
                            if ((qApp->applicationDirPath() + "/config/" + fileName) == dirpath)
                            {
                                pFileNameAction->setChecked(true);
                            }
                        }
                    });
                }
            });
        }
    }

    {
        connect(pactAction, &QAction::triggered,
                [=]() { QDesktopServices::openUrl(QUrl("https://docs.liuwei.vin/projects/VersaAssistant/md/Protocol.html")); });
        connect(helpAction, &QAction::triggered,
                [=]() { QDesktopServices::openUrl(QUrl("https://docs.liuwei.vin/projects/VersaAssistant")); });
    }
    {
        QAction *managementLicense = aboutMenu->addAction(tr("授权管理"));
        connect(managementLicense, &QAction::triggered, [=]()
        {
            if (!pVersaLicenseWidget.isNull())
            {
                // 计算子窗口在主窗口中居中显示的位置
                int x = this->geometry().x() +
                        (this->geometry().width() - pVersaLicenseWidget->geometry().width()) / 2;
                int y = this->geometry().y();

                pVersaLicenseWidget->move(x, y); // 将子窗口移动到计算出的位置
                pVersaLicenseWidget->show();
            }
        });

        QAction *aboutApp = aboutMenu->addAction(tr("关于"));
        connect(aboutApp, &QAction::triggered, [=]()
        {
            if (!pVersaAboutAppWidget.isNull())
            {
                // 计算子窗口在主窗口中居中显示的位置
                int x = this->geometry().x() +
                        (this->geometry().width() - pVersaAboutAppWidget->geometry().width()) / 2;
                int y = this->geometry().y();

                pVersaAboutAppWidget->move(x, y); // 将子窗口移动到计算出的位置
                pVersaAboutAppWidget->show();
            }
        });
    }
    this->pVersaDockWidget->creatViewMenu(viewMenu);
}

void VersaWidget::timeout()
{
    if (isInitReady == false)
    {
        return;
    }

    pVersaAppBar->stateTxQLabel->setText(
            QString("RxSz:%1 kB\nRxSp:%2 kB/s")
                    .arg(QString::number(VersaPhysicalPort->readCnt, 'f', 2))
                    .arg(QString::number(VersaPhysicalPort->readSpeed, 'f', 2)));

    pVersaAppBar->stateRxQLabel->setText(
            QString("TxSz:%1 kB\nTxSp:%2 kB/s")
                    .arg(QString::number(VersaPhysicalPort->sendCnt, 'f', 2))
                    .arg(QString::number(VersaPhysicalPort->sendSpeed, 'f', 2)));
}

void VersaWidget::on_btnMin_clicked()
{
    showMinimized();
}

void VersaWidget::on_btnMax_clicked()
{
    if (isMaximized())
    {
        showNormal();
    } else
    {
        showMaximized();
    }
}

void VersaWidget::on_btnExit_clicked()
{
    isInitReady = false;

    refreshTimer.stop();
    saveVersaInfo();
    VersaFree();

    this->close();
}

void VersaWidget::saveVersaInfo()
{
    VersaUserConfigByteSet(eByteNormalWidgetSend,
                           pVersaDockWidget->normalSendEdit->getSaveDataBuffer());
    VersaUserConfigByteSet(eByteMultWidgetSend,
                           pVersaDockWidget->multSendTextEdit->getSaveDataBuffer());
    VersaUserConfigByteSet(eByteProtocolRead,
                           pVersaDockWidget->pVersaProtocolRead->getSaveDataBuffer());
    VersaUserConfigByteSet(eByteProtocolSend,
                           pVersaDockWidget->pVersaProtocolSend->getSaveDataBuffer());
    VersaUserConfigByteSet(eVersaSideBarInfo,
                           pVersaSideWidget->getSaveDataBuffer());

    VersaUserGeometry() = saveGeometry();
    VersaUserDockState() = pVersaDockWidget->dockManager->saveState();
    VersaSaveInfo();
}

void VersaWidget::restoreVersaInfo()
{
    VersaRestoreInfo();
}

//============================================================================
void VersaWidget::restoreVersaWidgetUi()
{
    if (isInitReady == true)
    {
        this->hide();
    }
    restoreGeometry(VersaUserGeometry());
    pVersaDockWidget->dockManager->restoreState(VersaUserDockState());
    if (isInitReady == true)
    {
        QTimer::singleShot(1, this, [=]()
        {
            // 显示窗口
            this->show();
        });
    }
}

void VersaWidget::resizeEvent(QResizeEvent *event)
{
    if (!pVersaSideWidget.isNull())
    {
        pVersaSideWidget->resizeSidebar();
    }
}
