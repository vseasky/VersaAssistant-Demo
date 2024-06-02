//
// Created by vSeasky on 2023/9/23.
//

#include "VersaProtocolWidget.h"
#include <QDataStream>
#include <QPushButton>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QDateTime>
#include <QEvent>
#include <QWheelEvent>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include "VersaConfig.h"
#include "VersaDataControl.h"
#include <QDebug>

class wheelEventFilter : public QObject
{
Q_OBJECT

public:
    wheelEventFilter(QObject *parent = nullptr) : QObject(parent)
    {
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj->isWidgetType() && event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
            if (QComboBox *comboBox = qobject_cast<QComboBox *>(obj))
            {
                // 禁用鼠标滚轮事件
                wheelEvent->ignore();
                return true;
            }
        }
        return false;
    }
};

VersaProtocolWidget::VersaProtocolWidget(const QByteArray &buffer,
                                         ProtocolWidgetMode mode,
                                         QWidget *parent) :
        QWidget(parent),
        protocolMode(mode)
{
    protocolStruct.frame.frame_user.data.pData = NULL;
    protocolStruct.message.pData = NULL;
    clearVector();
    infoControl.resize(3);
    infoControl[0] = "1";
    infoControl[1] = "192.168.0.1";
    infoControl[2] = "32401";
    // 恢复配置数据
    if (buffer.size() > 0)
    {
        restoreDataBuffer(buffer, MAX_PROTOCOL_NUM);
    } else
    {
        defautlDataBuffer(MAX_PROTOCOL_NUM);
    }

    pProtocolWidgetLayout = new QVBoxLayout();
    pProtocolWidgetLayout->setSpacing(0);
    pProtocolWidgetLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(pProtocolWidgetLayout);

    if (protocolMode == SendProtocolWidget)
    {
        pProtocolWidgetLayout->addWidget(creatProtocolInfo());
    }

    currentProtocolTable = creatProtocolTable();
    pProtocolWidgetLayout->addWidget(currentProtocolTable);

    startDateTime = QDateTime::currentDateTime();
    mTimer.setTimerType(Qt::PreciseTimer);
    connect(&mTimer, &QTimer::timeout, this, &VersaProtocolWidget::timeout);

    mRefreshTimer.setTimerType(Qt::PreciseTimer);
    connect(&mRefreshTimer, &QTimer::timeout, this, &VersaProtocolWidget::refreshEditText);
    mRefreshTimer.setInterval(refreshSpeed);
    mRefreshTimer.start();

    refreshReadHeaderEnable = false;
    refreshSendHeaderEnable = false;
}


void VersaProtocolWidget::addPeerAddress(QString peerAddress)
{
    if ((protocolMode == SendProtocolWidget) && (pInternetAddrCombox != nullptr))
    {
        QRegularExpression regex("^(?:\\d{1,3}\\.){3}\\d{1,3}$");
        if (regex.match(peerAddress).hasMatch())
        {
            int index = pInternetAddrCombox->findText(peerAddress);
            if (index == -1)
            {
                pInternetAddrCombox->addItem(peerAddress);
            }
        }
    }
}

void VersaProtocolWidget::addPeerPort(quint16 port)
{
    if ((protocolMode == SendProtocolWidget) && (pInternetPortCombox != nullptr))
    {
        QString portStr = QString("%1").arg(port);
        int index = pInternetPortCombox->findText(QString(portStr));
        if (index == -1)
        {
            pInternetPortCombox->addItem(portStr);
        }
    }
}

QPointer<QTableWidget> VersaProtocolWidget::creatProtocolInfo()
{
    QTableWidget *protocolInfo = new QTableWidget(this);
    protocolInfo->setColumnCount(5);
    protocolInfo->setRowCount(1);
    protocolInfo->resizeRowsToContents();
    protocolInfo->horizontalHeader()->setMinimumHeight(48);
    protocolInfo->horizontalHeader()->setFixedHeight(48);
    protocolInfo->verticalHeader()->setMinimumWidth(48);
    protocolInfo->verticalHeader()->setFixedWidth(48);

    // 自动调整列宽
    protocolInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 固定行高
    protocolInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    protocolInfo->verticalHeader()->setDefaultSectionSize(48);
    protocolInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    protocolInfo->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    protocolInfo->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    protocolInfo->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    protocolInfo->setColumnWidth(0, 96);
    protocolInfo->setColumnWidth(1, 96);
    protocolInfo->setColumnWidth(2, 128);
    protocolInfo->setColumnWidth(4, 48);
    protocolInfo->setMaximumHeight(96);
    QStringList headers;
    headers << tr("计时")
            << tr("(ms/次)")
            << tr("目标地址")
            << tr("目标端口")
            << tr("发送");
    protocolInfo->setHorizontalHeaderLabels(headers);

    // 调用基类的插入行方法
    QPushButton *timerEnableButton = new QPushButton(protocolInfo);
    QString currentStyleSheet = timerEnableButton->styleSheet();
    timerEnableButton->setCheckable(true);
    timerEnableButton->setIcon(ThemeIcon("IconCheck1", "AppBackgroundColor", 32));
    protocolInfo->setCellWidget(0, 0, timerEnableButton);
    connect(timerEnableButton, &QPushButton::released, this, [=]()
    {
        if (timerEnableButton->isChecked())
        {
            mTimer.start();
            if (editHeaderDataLen != nullptr)
            {
                editHeaderDataLen->setReadOnly(true);
            }
        } else
        {
            mTimer.stop();
            if (editHeaderDataLen != nullptr)
            {
                editHeaderDataLen->setReadOnly(false);
            }
        }
    });


    QPushButton *pSenButton = new QPushButton(protocolInfo);
    protocolInfo->setCellWidget(0, 4, pSenButton);
    pSenButton->setIcon(ThemeIcon("IconSend3", "DockWidget", 32));
    connect(pSenButton, &QPushButton::released, this, [=]()
    {
        emit protocolVersa(mAddress, mPort);
    });
    QSpinBox *pTimerQSpinBox = new QSpinBox(protocolInfo);//定时时间
    {
        pTimerQSpinBox->setMinimum(1);
        pTimerQSpinBox->setMaximum(10000);
        pTimerQSpinBox->setMinimumWidth(96);
        pTimerQSpinBox->setMinimumHeight(48);
        pTimerQSpinBox->setValue(infoControl[0].toUInt());
        mTimer.setInterval(pTimerQSpinBox->value());
        connect(pTimerQSpinBox, &QSpinBox::textChanged, this, [=]()
        {
            if (!mTimer.isActive())
            {
                mTimer.setInterval(pTimerQSpinBox->value());
                infoControl[0] = pTimerQSpinBox->text();
            }
        });


    }
    protocolInfo->setCellWidget(0, 1, pTimerQSpinBox);

    pInternetAddrCombox = new QComboBox(protocolInfo);
    protocolInfo->setCellWidget(0, 2, pInternetAddrCombox);
    {
        pInternetAddrCombox->setEditable(true);
        pInternetAddrCombox->setView(new QListView(pInternetAddrCombox));
        pInternetAddrCombox->setMinimumHeight(32);
        pInternetAddrCombox->setValidator(new QRegExpValidator(QRegExp(RegExpIpAddr), pInternetAddrCombox));
        pInternetAddrCombox->addItem(infoControl[1]);

    }
    pInternetPortCombox = new QComboBox(protocolInfo);
    protocolInfo->setCellWidget(0, 3, pInternetPortCombox);
    {
        pInternetPortCombox->setEditable(true);
        pInternetPortCombox->setView(new QListView(pInternetPortCombox));
        pInternetPortCombox->setMinimumHeight(32);
        pInternetPortCombox->setValidator(new QRegExpValidator(QRegExp(RegExpIpPort), pInternetPortCombox));
        pInternetPortCombox->addItem(infoControl[2]);
    }

    connect(pInternetAddrCombox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, [=]()
    {
        mAddress = pInternetAddrCombox->currentText();
        infoControl[1] = pInternetAddrCombox->currentText();
    });
    mAddress = pInternetAddrCombox->currentText();
    connect(pInternetPortCombox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, [=]()
    {
        mPort = pInternetPortCombox->currentText().toUInt();
        infoControl[2] = pInternetPortCombox->currentText();
    });
    mPort = pInternetPortCombox->currentText().toUInt();
    return protocolInfo;
}

QPointer<QTableWidget> VersaProtocolWidget::creatProtocolTable()
{
    if (currentMaxRowCount == 0)
    {
        return nullptr;
    }

    // 初始化协议处理结构体
    init_protocol(&protocolStruct, currentMaxRowCount);

    QTableWidget *protocolTable = new QTableWidget(this);
    protocolTable->setColumnCount(5);
    protocolTable->setRowCount(currentMaxRowCount + 3);

    protocolTable->horizontalHeader()->setMinimumHeight(48);
    protocolTable->horizontalHeader()->setFixedHeight(48);
    protocolTable->verticalHeader()->setMinimumWidth(48);
    protocolTable->verticalHeader()->setFixedWidth(48);
    protocolTable->setSpan(1, 0, 1, 5);

    protocolTable->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    protocolTable->setVerticalHeaderItem(0, new QTableWidgetItem(QString("帧头")));
    protocolTable->setVerticalHeaderItem(1, new QTableWidgetItem(QString("公式")));
    protocolTable->setVerticalHeaderItem(2, new QTableWidgetItem(QString("名称")));
    // 协议内容
    {
        if (editHeaderCmd == nullptr)
        {
            editHeaderCmd = new QLineEdit(protocolTable);
            editHeaderCmd->setValidator(new QRegExpValidator(QRegExp("^0x[0-9A-Fa-f]{1,2}"), protocolTable));
            protocolTable->setCellWidget(0, 0, editHeaderCmd);
            connect(editHeaderCmd, &QLineEdit::textChanged, this, [=](const QString &text)
            {
                if (ProtocolTableEnable = true)
                {
                    bool ok;
                    uint8_t cmd = text.toUInt(&ok, 16);
                    if (ok)
                    {
                        protocolStruct.frame.frame_user.header.cmd = cmd;
                    }
                }
            });
        }
        if (editHeaderDeviceType == nullptr)
        {
            editHeaderDeviceType = new QLineEdit(protocolTable);
            editHeaderDeviceType->setValidator(
                    new QRegExpValidator(QRegExp("^0x[0-9A-Fa-f]{1,4}"), protocolTable));
            protocolTable->setCellWidget(0, 1, editHeaderDeviceType);
            connect(editHeaderDeviceType, &QLineEdit::textChanged, this, [=](const QString &text)
            {
                if (ProtocolTableEnable = true)
                {
                    bool ok;
                    uint16_t deviceType = editHeaderDeviceType->text().toUInt(&ok, 16);
                    if (ok)
                    {
                        protocolStruct.frame.frame_user.header.device_type = deviceType;
                    }
                }
            });
        }

        if (editHeaderDeviceId == nullptr)
        {
            editHeaderDeviceId = new QLineEdit(protocolTable);
            editHeaderDeviceId->setValidator(new QRegExpValidator(QRegExp("^0x[0-9A-Fa-f]{1,8}"), protocolTable));
            protocolTable->setCellWidget(0, 2, editHeaderDeviceId);
            connect(editHeaderDeviceId, &QLineEdit::textChanged, this, [=](const QString &text)
            {
                if (ProtocolTableEnable = true)
                {
                    bool ok;
                    uint32_t deviceId = editHeaderDeviceId->text().toUInt(&ok, 16);
                    if (ok)
                    {
                        protocolStruct.frame.frame_user.header.device_id = deviceId;
                    }
                }
            });
        }
        if (editHeaderDataId == nullptr)
        {
            editHeaderDataId = new QLineEdit(protocolTable);
            editHeaderDataId->setValidator(new QRegExpValidator(QRegExp("^0x[0-9A-Fa-f]{1,4}"), protocolTable));
            protocolTable->setCellWidget(0, 3, editHeaderDataId);
            connect(editHeaderDataId, &QLineEdit::textChanged, this, [=](const QString &text)
            {
                if (ProtocolTableEnable = true)
                {
                    bool ok;
                    uint16_t dataId = editHeaderDataId->text().toUInt(&ok, 16);
                    if (ok)
                    {
                        protocolStruct.frame.frame_user.header.data_id = dataId;
                    }
                }
            });
        }
        if (editHeaderDataLen == nullptr)
        {
            editHeaderDataLen = new QSpinBox(protocolTable);
            editHeaderDataLen->setMaximum(currentMaxRowCount);
            editHeaderDataLen->setMinimum(0);
            protocolTable->setCellWidget(0, 4, editHeaderDataLen);
            connect(editHeaderDataLen, &QSpinBox::textChanged, this, [=]()
            {
                if (ProtocolTableEnable = true)
                {
                    uint16_t dataLen = editHeaderDataLen->value();
                    protocolStruct.frame.frame_user.data.data_len = qMin((int) dataLen, currentMaxRowCount);
                }
            });
        }
    }

    // 设置数据名称
    for (int iRow = 3; iRow < protocolTable->rowCount(); iRow++)
    {
        protocolTable->setVerticalHeaderItem(iRow, new QTableWidgetItem(QString::number(iRow - 2)));
    }

    // 设置数据帧头名称
    {
        QStringList headers;
        // 发送数据 = 原始数据 + 缩放比例 * 控件值
        headers << tr("Cmd")
                << tr("Device")
                << tr("DeviceId")
                << tr("DataId")
                << tr("Data uLen");

        protocolTable->setHorizontalHeaderLabels(headers);
    }

    // 设置数据处理名称
    {
        QLineEdit *mathEdit = new QLineEdit(protocolTable);
        protocolTable->setCellWidget(1, 0, mathEdit);
        mathEdit->setReadOnly(true);
        if (protocolMode == SendProtocolWidget)
        {
            mathEdit->setText(tr("发送数据 = 原始数据 + 缩放比例 * 增量值"));

            QLineEdit *lineEdit1 = new QLineEdit(protocolTable);
            lineEdit1->setText(tr("数据格式"));
            lineEdit1->setReadOnly(true);
            protocolTable->setCellWidget(2, 0, lineEdit1);

            QLineEdit *lineEdit2 = new QLineEdit(protocolTable);
            lineEdit2->setText(tr("原始数据"));
            lineEdit2->setReadOnly(true);
            protocolTable->setCellWidget(2, 1, lineEdit2);

            QLineEdit *lineEdit3 = new QLineEdit(protocolTable);
            lineEdit3->setText(tr("发送数据"));
            lineEdit3->setReadOnly(true);
            protocolTable->setCellWidget(2, 2, lineEdit3);

            QLineEdit *lineEdit4 = new QLineEdit(protocolTable);
            lineEdit4->setText(tr("缩放比例"));
            lineEdit4->setReadOnly(true);
            protocolTable->setCellWidget(2, 3, lineEdit4);

            QLineEdit *lineEdit5 = new QLineEdit(protocolTable);
            lineEdit5->setText(tr("增量值"));
            lineEdit5->setReadOnly(true);
            protocolTable->setCellWidget(2, 4, lineEdit5);
        } else if (protocolMode == ReadProtocolWidget)
        {
            mathEdit->setText(tr("数据波动 = (原始数据 + 数据偏移) * 缩放比例"));

            QLineEdit *lineEdit1 = new QLineEdit(protocolTable);
            lineEdit1->setText(tr("数据格式"));
            lineEdit1->setReadOnly(true);
            protocolTable->setCellWidget(2, 0, lineEdit1);

            QLineEdit *lineEdit2 = new QLineEdit(protocolTable);
            lineEdit2->setText(tr("原始数据"));
            lineEdit2->setReadOnly(true);
            protocolTable->setCellWidget(2, 1, lineEdit2);

            QLineEdit *lineEdit3 = new QLineEdit(protocolTable);
            lineEdit3->setText(tr("数据偏移"));
            lineEdit3->setReadOnly(true);
            protocolTable->setCellWidget(2, 2, lineEdit3);

            QLineEdit *lineEdit4 = new QLineEdit(protocolTable);
            lineEdit4->setText(tr("缩放比例"));
            lineEdit4->setReadOnly(true);
            protocolTable->setCellWidget(2, 3, lineEdit4);

            QLineEdit *lineEdit5 = new QLineEdit(protocolTable);
            lineEdit5->setText(tr("数据波动"));
            lineEdit5->setReadOnly(true);
            protocolTable->setCellWidget(2, 4, lineEdit5);
        }
    }

    // 添加自定义控件
    for (uint16_t iRow = 0; iRow < currentMaxRowCount; iRow++)
    {
        // 从第三行开始插入 rowCount 行
        insertProtocolRow(protocolTable, 3, iRow);
    }

    // 自动调整列宽
    protocolTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    protocolTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    protocolTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    protocolTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    protocolTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    protocolTable->setColumnWidth(0, 96);
    protocolTable->setColumnWidth(1, 96);
    protocolTable->setColumnWidth(2, 96);
    protocolTable->setColumnWidth(3, 96);

    protocolTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ProtocolTableEnable = true;
    refreshEditHeader();
    return protocolTable;
}

void VersaProtocolWidget::deleteProtocolTable()
{
    if (pProtocolWidgetLayout != nullptr)
    {
        if (currentProtocolTable != nullptr)
        {
            pProtocolWidgetLayout->removeWidget(currentProtocolTable);
            // 初始化协议处理结构体
            deinit_protocol(&protocolStruct);
            currentProtocolTable->deleteLater();
            editHeaderCmd = nullptr;
            editHeaderDeviceType = nullptr;
            editHeaderDeviceId = nullptr;
            editHeaderDataId = nullptr;
            editHeaderDataLen = nullptr;
            currentProtocolTable = nullptr;
        }
    }
}

VersaProtocolWidget::~VersaProtocolWidget()
{
    mRefreshTimer.stop();
    mTimer.stop();
    deleteProtocolTable();

    if (!editHeaderCmd.isNull())
    {
        delete editHeaderCmd;
        editHeaderCmd = nullptr;
    }

    if (!editHeaderDeviceType.isNull())
    {
        delete editHeaderDeviceType;
        editHeaderDeviceType = nullptr;
    }

    if (!editHeaderDeviceId.isNull())
    {
        delete editHeaderDeviceId;
        editHeaderDeviceId = nullptr;
    }

    if (!editHeaderDataId.isNull())
    {
        delete editHeaderDataId;
        editHeaderDataId = nullptr;
    }

    if (!editHeaderDataLen.isNull())
    {
        delete editHeaderDataLen;
        editHeaderDataLen = nullptr;
    }

    if (!pProtocolWidgetLayout.isNull())
    {
        delete pProtocolWidgetLayout;
        pProtocolWidgetLayout = nullptr;
    }

    if (!currentProtocolTable.isNull())
    {
        delete currentProtocolTable;
        currentProtocolTable = nullptr;
    }

    if (!pInternetAddrCombox.isNull())
    {
        delete pInternetAddrCombox;
        pInternetAddrCombox = nullptr;
    }

    if (!pInternetPortCombox.isNull())
    {
        delete pInternetPortCombox;
        pInternetPortCombox = nullptr;
    }

}

/*!
 * 显示内容修改触发原始数据修改
 */
void VersaProtocolWidget::updataEditHeader()
{
    if (editHeaderCmd == nullptr ||
        editHeaderDeviceType == nullptr ||
        editHeaderDeviceId == nullptr ||
        editHeaderDataId == nullptr ||
        editHeaderDataLen == nullptr ||
        ProtocolTableEnable == false)
    {
        return;
    }

    bool ok;
    uint8_t cmd = editHeaderCmd->text().toUInt(&ok, 16);
    if (ok)
    {
        protocolStruct.frame.frame_user.header.cmd = cmd;
    }

    uint16_t deviceType = editHeaderDeviceType->text().toUInt(&ok, 16);
    if (ok)
    {
        protocolStruct.frame.frame_user.header.device_type = deviceType;
    }

    uint32_t deviceId = editHeaderDeviceId->text().toUInt(&ok, 16);
    if (ok)
    {
        protocolStruct.frame.frame_user.header.device_id = deviceId;
    }

    uint16_t dataId = editHeaderDataId->text().toUInt(&ok, 16);
    if (ok)
    {
        protocolStruct.frame.frame_user.header.data_id = dataId;
    }
    uint16_t dataLen = editHeaderDataLen->text().toUInt();
    protocolStruct.frame.frame_user.data.data_len = qMin((int) dataLen, currentMaxRowCount);

    emit updataEditHeaderSignals();
}

/*!
 * 数据更改触发修改显示内容
 */
void VersaProtocolWidget::refreshEditHeader()
{
    if (editHeaderCmd == nullptr ||
        editHeaderDeviceType == nullptr ||
        editHeaderDeviceId == nullptr ||
        editHeaderDataId == nullptr ||
        editHeaderDataLen == nullptr ||
        ProtocolTableEnable == false)
    {
        return;
    }

    editHeaderCmd->setText(QString("0x%1").arg(
            protocolStruct.frame.frame_user.header.cmd,
            2, 16,
            QLatin1Char('0')));

    editHeaderDeviceType->setText(QString("0x%1").arg(
            protocolStruct.frame.frame_user.header.device_type,
            4, 16,
            QLatin1Char('0')));

    editHeaderDeviceId->setText(QString("0x%1").arg(
            protocolStruct.frame.frame_user.header.device_id,
            8, 16,
            QLatin1Char('0')));

    editHeaderDataId->setText(QString("0x%1").arg(
            protocolStruct.frame.frame_user.header.data_id,
            4, 16,
            QLatin1Char('0')));

    editHeaderDataLen->setValue(
            protocolStruct.frame.frame_user.data.data_len);
}

/*!
 * 更改显示大小
 * @param size
 */
void VersaProtocolWidget::resizeProtocolTable(const int size)
{
    ProtocolTableEnable = false;
    deinit_protocol(&protocolStruct);
    protocolStruct.frame.frame_user.data.pData = NULL;
    protocolStruct.message.pData = NULL;
    deleteProtocolTable();
    // 更改数据大小
    resizeVector(size);
    currentProtocolTable = creatProtocolTable();
    pProtocolWidgetLayout->addWidget(currentProtocolTable);
}


void VersaProtocolWidget::restoreDataBuffer(const QByteArray &buffer, int size)
{
    QDataStream stream(buffer);

    QVector<uint32_t> mUnionValue;
    QVector<uint32_t> mLineEdit1Value;
    QVector<uint32_t> mLineEdit2Value;
    QVector<uint32_t> mLineEdit3Value;
    QVector<uint32_t> mDataControlValue;
    QVector<int32_t> mVectorLength;
    mVectorLength.resize(8);
    if (buffer.size() > sizeof(currentMaxRowCount) +
                        sizeof(protocolStruct.frame.frame_user.header) +
                        sizeof(protocolStruct.frame.frame_user.data.data_len) +
                        sizeof(mVectorLength))
    {
        stream >> currentMaxRowCount;
        stream >> protocolStruct.frame.frame_user.header.cmd;
        stream >> protocolStruct.frame.frame_user.header.device_type;
        stream >> protocolStruct.frame.frame_user.header.device_id;
        stream >> protocolStruct.frame.frame_user.header.data_id;
        stream >> protocolStruct.frame.frame_user.data.data_len;
        stream >> mVectorLength;

        // qDebug() << "restoreDataBuffer";
        // qDebug() << protocolStruct.frame.frame_user.header.cmd;
        // qDebug() << protocolStruct.frame.frame_user.header.device_type;
        // qDebug() << protocolStruct.frame.frame_user.header.device_id;
        // qDebug() << protocolStruct.frame.frame_user.header.data_id;
        // qDebug() << protocolStruct.frame.frame_user.data.data_len;

        if (currentMaxRowCount <= MAX_PROTOCOL_NUM)
        {
            resizeVector(currentMaxRowCount);
            if (buffer.size() > sizeof(currentMaxRowCount) +
                                sizeof(protocolStruct.frame.frame_user.header) +
                                sizeof(protocolStruct.frame.frame_user.data.data_len) +
                                sizeof(mVectorLength) +
                                mVectorLength[0] * sizeof(uint32_t) +
                                mVectorLength[1] * sizeof(uint32_t) +
                                mVectorLength[2] * sizeof(uint32_t) +
                                mVectorLength[3] * sizeof(uint32_t) +
                                mVectorLength[4] * sizeof(uint32_t) +
                                mVectorLength[5] + mVectorLength[6] + mVectorLength[7])
            {

                if (mVectorLength[0] > 0)
                {
                    mUnionValue.resize(mVectorLength[0]);
                    // 恢复 mUnionValue
                    stream >> mUnionValue;
                }

                if (mVectorLength[1] > 0)
                {
                    mLineEdit1Value.resize(mVectorLength[1]);
                    // 恢复 mLineEdit1Value
                    stream >> mLineEdit1Value;
                }

                if (mVectorLength[2] > 0)
                {
                    mLineEdit2Value.resize(mVectorLength[2]);
                    // 恢复 mLineEdit2Value
                    stream >> mLineEdit2Value;
                }

                if (mVectorLength[3] > 0)
                {
                    mLineEdit3Value.resize(mVectorLength[3]);
                    // 恢复 mLineEdit3Value
                    stream >> mLineEdit3Value;
                }

                if (mVectorLength[4] > 0)
                {
                    mDataControlValue.resize(mVectorLength[4]);
                    // 恢复 mDataControlValue
                    stream >> mDataControlValue;
                }

                QByteArray infolist0, infolist1, infolist2;

                if (mVectorLength[5] > 0)
                {
                    infolist0.resize(mVectorLength[5]);
                    stream >> infolist0;
                }
                if (mVectorLength[6] > 0)
                {
                    infolist1.resize(mVectorLength[6]);
                    stream >> infolist1;
                }
                if (mVectorLength[7] > 0)
                {
                    infolist2.resize(mVectorLength[7]);
                    stream >> infolist2;
                }

                infoControl[0] = QString::fromUtf8(infolist0);
                infoControl[1] = QString::fromUtf8(infolist1);
                infoControl[2] = QString::fromUtf8(infolist2);

                VersaDataUnion union_data;
                {
                    for (uint32_t iVec = 0; iVec < qMin(mVectorLength[0], mVectorUnionValue.size()); iVec++)
                    {
                        union_data.data_u32 = mUnionValue[iVec];
                        mVectorUnionValue[iVec] = union_data.data_int32;
                    }
                    for (uint32_t iVec = 0; iVec < qMin(mVectorLength[1], mVectorLineEdit1Value.size()); iVec++)
                    {
                        mVectorLineEdit1Value[iVec].data_u32 = mLineEdit1Value[iVec];
                    }
                    for (uint32_t iVec = 0; iVec < qMin(mVectorLength[2], mVectorLineEdit2Value.size()); iVec++)
                    {
                        mVectorLineEdit2Value[iVec].data_u32 = mLineEdit2Value[iVec];
                    }
                    for (uint32_t iVec = 0; iVec < qMin(mVectorLength[3], mVectorLineEdit3Value.size()); iVec++)
                    {
                        mVectorLineEdit3Value[iVec].data_u32 = mLineEdit3Value[iVec];
                    }
                    for (uint32_t iVec = 0; iVec < qMin(mVectorLength[4], mVectorDataControlValue.size()); iVec++)
                    {
                        union_data.data_u32 = mDataControlValue[iVec];
                        mVectorDataControlValue[iVec] = union_data.data_int32;
                    }
                }
                return;
            }
        }
    }

    defautlDataBuffer(qMin(size, MAX_PROTOCOL_NUM));
    return;
}

void VersaProtocolWidget::defautlDataBuffer(int size)
{
    infoControl[0] = "1";
    infoControl[1] = "192.168.0.1";
    infoControl[2] = "32401";
    currentMaxRowCount = qMin(size, MAX_PROTOCOL_NUM);
    protocolStruct.frame.frame_user.header.cmd = 0xFF;
    protocolStruct.frame.frame_user.header.device_type = 0xFF;
    protocolStruct.frame.frame_user.header.device_id = 0xFF;
    protocolStruct.frame.frame_user.header.data_id = 0xFF;
    protocolStruct.frame.frame_user.data.data_len = currentMaxRowCount;
    resizeVector(currentMaxRowCount);
    for (uint16_t iCount = 0; iCount < currentMaxRowCount; iCount++)
    {
        mVectorLineEdit3Value[iCount].data_float = 1;
    }
}

/*!
 * 存儲數據
 */
QByteArray VersaProtocolWidget::getSaveDataBuffer()
{
    QByteArray mDataBuffer;
    QDataStream stream(&mDataBuffer, QIODevice::WriteOnly);

    VersaDataUnion union_data;

    QVector<uint32_t> mUnionValue;
    QVector<uint32_t> mLineEdit1Value;
    QVector<uint32_t> mLineEdit2Value;
    QVector<uint32_t> mLineEdit3Value;
    QVector<uint32_t> mDataControlValue;

    QVector<int32_t> mVectorLength;
    mVectorLength.resize(8);

    mVectorLength[0] = mVectorUnionValue.size();
    mVectorLength[1] = mVectorLineEdit1Value.size();
    mVectorLength[2] = mVectorLineEdit2Value.size();
    mVectorLength[3] = mVectorLineEdit3Value.size();
    mVectorLength[4] = mVectorDataControlValue.size();
    mVectorLength[5] = infoControl[0].toUtf8().size();
    mVectorLength[6] = infoControl[1].toUtf8().size();
    mVectorLength[7] = infoControl[2].toUtf8().size();

    mUnionValue.resize(mVectorLength[0]);
    mLineEdit1Value.resize(mVectorLength[1]);
    mLineEdit2Value.resize(mVectorLength[2]);
    mLineEdit3Value.resize(mVectorLength[3]);
    mDataControlValue.resize(mVectorLength[4]);

    for (uint32_t iVec = 0; iVec < mVectorLength[0]; iVec++)
    {
        union_data.data_int32 = mVectorUnionValue[iVec];
        mUnionValue[iVec] = union_data.data_u32;
    }

    for (uint32_t iVec = 0; iVec < mVectorLength[1]; iVec++)
    {
        mLineEdit1Value[iVec] = mVectorLineEdit1Value[iVec].data_u32;
    }

    for (uint32_t iVec = 0; iVec < mVectorLength[2]; iVec++)
    {
        mLineEdit2Value[iVec] = mVectorLineEdit2Value[iVec].data_u32;
    }

    for (uint32_t iVec = 0; iVec < mVectorLength[3]; iVec++)
    {
        mLineEdit3Value[iVec] = mVectorLineEdit3Value[iVec].data_u32;
    }

    for (uint32_t iVec = 0; iVec < mVectorLength[4]; iVec++)
    {
        union_data.data_int32 = mVectorDataControlValue[iVec];
        mDataControlValue[iVec] = union_data.data_u32;
    }

    stream << currentMaxRowCount;

    // qDebug() << "getSaveDataBuffer";
    // qDebug() << protocolStruct.frame.frame_user.header.cmd;
    // qDebug() << protocolStruct.frame.frame_user.header.device_type;
    // qDebug() << protocolStruct.frame.frame_user.header.device_id;
    // qDebug() << protocolStruct.frame.frame_user.header.data_id;
    // qDebug() << protocolStruct.frame.frame_user.data.data_len;

    stream << protocolStruct.frame.frame_user.header.cmd;
    stream << protocolStruct.frame.frame_user.header.device_type;
    stream << protocolStruct.frame.frame_user.header.device_id;
    stream << protocolStruct.frame.frame_user.header.data_id;
    stream << protocolStruct.frame.frame_user.data.data_len;


    stream << mVectorLength;

    // 存储 mVectorUnionValue
    if (mVectorLength[0] > 0)
    {
        stream << mUnionValue;
    }
    // 存储 mLineEdit1Value
    if (mVectorLength[1] > 0)
    {
        stream << mLineEdit1Value;
    }
    // 存储 mLineEdit2Value
    if (mVectorLength[2] > 0)
    {
        stream << mLineEdit2Value;
    }
    // 存储 mLineEdit3Value
    if (mVectorLength[3] > 0)
    {
        stream << mLineEdit3Value;
    }

    // 存储 mVectorDataControlValue
    if (mVectorLength[4] > 0)
    {
        stream << mDataControlValue;
    }

    if (mVectorLength[5] > 0)
    {
        stream << infoControl[0].toUtf8();
    }

    if (mVectorLength[6] > 0)
    {
        stream << infoControl[1].toUtf8();
    }
    if (mVectorLength[7] > 0)
    {

        stream << infoControl[2].toUtf8();
    }
    return mDataBuffer;
}

/*!
 * updataReadSlots 更新读取数据
 * @param data
 */
void VersaProtocolWidget::updataReadSlots()
{
    if (protocolMode == ReadProtocolWidget)
    {
        // 更新数据
        for (uint16_t iCount = 0; iCount < protocolStruct.frame.frame_user.data.max_data_len; iCount++)
        {
            updataReadData(iCount);
            refreshReadDataEnable[iCount] = true;
        }
        // 更新显示数据，在定时器更新，避免高刷新率造成ui卡顿
        refreshReadHeaderEnable = true;
        // 更新完成
        emit addGraphsData(readKeys, readValues);
    }
}

void VersaProtocolWidget::refreshEditText()
{
    if (currentMaxRowCount == 0)
    {
        return;
    }

    if (protocolMode == ReadProtocolWidget)
    {
        // 如果使能了更新
        if (refreshReadHeaderEnable == true)
        {
            // 更新读取的数据帧头显示
            refreshEditHeader();
            // 更新完成
            refreshReadHeaderEnable = false;
        }
        for (uint16_t iCount = 0; iCount < protocolStruct.frame.frame_user.data.max_data_len; iCount++)
        {
            if (refreshReadDataEnable[iCount] == true)
            {
                // 更新接收显示数据
                refreshReadData(iCount);
                refreshReadDataEnable[iCount] = false;
            }
        }
    } else
    {
        // 如果使能了更新
        if (refreshSendHeaderEnable == true)
        {
            // 更新读取的数据帧头显示
            refreshEditHeader();
            // 更新完成
            refreshSendHeaderEnable = false;
        }
        for (uint16_t iCount = 0; iCount < protocolStruct.frame.frame_user.data.max_data_len; iCount++)
        {
            if (updataSendDataEnable[iCount] == true)
            {
                // 更新发送显示数据
                refreshSendData(iCount);
                updataSendDataEnable[iCount] = false;
            }
        }
    }
}

void VersaProtocolWidget::timeout()
{
    if (protocolMode == ReadProtocolWidget)
    {
    } else
    {
        // 发送数据
        emit protocolVersa(mAddress, mPort);
    }
}

void VersaProtocolWidget::clearVector()
{
    deinit_protocol(&protocolStruct);
    memset(&protocolStruct, 0, sizeof(protocolStruct));
    protocolStruct.frame.frame_user.data.pData = NULL;
    protocolStruct.message.pData = NULL;
    refreshReadDataEnable.clear();
    updataSendDataEnable.clear();
    readKeys.clear();
    readValues.clear();
    mVectorUnionType.clear();
    mVectorLineEdit1.clear();
    mVectorLineEdit2.clear();
    mVectorLineEdit3.clear();
    mVectorDataControl.clear();
    mVectorUnionValue.clear();
    mVectorLineEdit1Value.clear();
    mVectorLineEdit2Value.clear();
    mVectorLineEdit3Value.clear();
    mVectorDataControlValue.clear();
}

void VersaProtocolWidget::resizeVector(int size)
{
    currentMaxRowCount = size;
    refreshReadDataEnable.resize(size);
    updataSendDataEnable.resize(size);
    readKeys.resize(size);
    readValues.resize(size);
    mVectorUnionType.resize(size);
    mVectorLineEdit1.resize(size);
    mVectorLineEdit2.resize(size);
    mVectorLineEdit3.resize(size);
    mVectorDataControl.resize(size);
    mVectorUnionValue.resize(size);
    mVectorLineEdit1Value.resize(size);
    mVectorLineEdit2Value.resize(size);
    mVectorLineEdit3Value.resize(size);
    mVectorDataControlValue.resize(size);
}

void VersaProtocolWidget::limitInputDouble(QLineEdit *lineEdit)
{
    double value = lineEdit->text().toDouble();
    lineEdit->setText(QString("%1").arg(value));
    QDoubleValidator *validator = new QDoubleValidator(
            -std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            4, lineEdit);
    lineEdit->setValidator(validator);
}

void VersaProtocolWidget::limitInputFloat32(QLineEdit *lineEdit)
{
    float value = lineEdit->text().toDouble();
    lineEdit->setText(QString("%1").arg(value));
    QDoubleValidator *validator = new QDoubleValidator(-3.4028235e38, 3.4028235e38, 4, lineEdit);
    lineEdit->setValidator(validator);
}

void VersaProtocolWidget::limitInputInt32(QLineEdit *lineEdit)
{
    int32_t value = lineEdit->text().toDouble();
    lineEdit->setText(QString("%1").arg(value));
    QIntValidator *validator = new QIntValidator(INT32_MIN, INT32_MAX, lineEdit);
    lineEdit->setValidator(validator);
}

void VersaProtocolWidget::limitInputUInt32(QLineEdit *lineEdit)
{
    uint32_t value = lineEdit->text().toDouble();
    lineEdit->setText(QString("%1").arg(value));
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("(0|[1-9][0-9]{0,9})"), lineEdit);
    lineEdit->setValidator(validator);
}

/*!
 * 计算待发送数据，仅处理数据，不处理刷新，避免高刷新率导致卡顿
 * @param row
 */
void VersaProtocolWidget::updataSendData(int row)
{
    if (protocolMode == SendProtocolWidget)
    {
        double calcValue;
        // 根据 数据类型更新
        switch (mVectorUnionValue[row])
        {
            case 0:
            {
                calcValue = mVectorLineEdit1Value[row].data_float +
                            mVectorLineEdit3Value[row].data_float *
                            mVectorDataControlValue[row];

                mVectorLineEdit2Value[row].data_float = calcValue;
                break;
            }
            case 1:
            {
                calcValue = mVectorLineEdit1Value[row].data_float +
                            mVectorLineEdit3Value[row].data_float *
                            mVectorDataControlValue[row];

                mVectorLineEdit2Value[row].data_int32 = calcValue;
                break;
            }
            case 2:
            {
                calcValue = mVectorLineEdit1Value[row].data_float +
                            mVectorLineEdit3Value[row].data_float *
                            mVectorDataControlValue[row];

                mVectorLineEdit2Value[row].data_u32 = calcValue;
                break;
            }
        }
        if (row < protocolStruct.frame.frame_user.data.max_data_len)
        {
            protocolStruct.frame.frame_user.data.pData[row] = mVectorLineEdit2Value[row].data_u32;
        }
        emit updataSendDataSignals();
    }
}

/*!
 * 数据刷新，应当以一定频率刷新，避免高刷新率导致卡顿
 * @param row
 */
void VersaProtocolWidget::refreshSendData(int row)
{
    if (protocolMode == SendProtocolWidget)
    {
        // 根据 数据类型更新
        switch (mVectorUnionValue[row])
        {
            case 0:
            {
                mVectorLineEdit2[row]->setText(QString::number(mVectorLineEdit2Value[row].data_float, 'f', 4));
                break;
            }
            case 1:
            {
                mVectorLineEdit2[row]->setText(QString("%1").arg(mVectorLineEdit2Value[row].data_int32));
                break;
            }
            case 2:
            {
                mVectorLineEdit2[row]->setText(QString("%1").arg(mVectorLineEdit2Value[row].data_u32));
                break;
            }
        }
    }
}

/*!
 * 更新接收数据,不处理刷新
 * @param row
 */
void VersaProtocolWidget::updataReadData(int row)
{
    if (protocolMode == ReadProtocolWidget)
    {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        if (row < protocolStruct.frame.frame_user.data.max_data_len)
        {
            mVectorLineEdit1Value[row].data_u32 = protocolStruct.frame.frame_user.data.pData[row];
            // 单位 ms
            readKeys[row] = startDateTime.msecsTo(currentDateTime) / 1000.0;
            // 根据 数据类型更新
            switch (mVectorUnionValue[row])
            {
                case 0:
                {
                    readValues[row] = double(mVectorLineEdit1Value[row].data_float +
                                             mVectorLineEdit2Value[row].data_float) *
                                      mVectorLineEdit3Value[row].data_float;

                    mVectorDataControlValue[row] = readValues[row];
                    break;
                }
                case 1:
                {
                    readValues[row] = double(mVectorLineEdit1Value[row].data_int32 +
                                             mVectorLineEdit2Value[row].data_float) *
                                      mVectorLineEdit3Value[row].data_float;
                    mVectorDataControlValue[row] = readValues[row];
                    break;
                }
                case 2:
                {
                    readValues[row] = double(mVectorLineEdit1Value[row].data_u32 +
                                             mVectorLineEdit2Value[row].data_float) *
                                      mVectorLineEdit3Value[row].data_float;

                    mVectorDataControlValue[row] = readValues[row];
                    break;
                }
            }
        }
    }
}

/*!
 * 刷新接收显示数据，仅处理刷新
 * @param row
 */
void VersaProtocolWidget::refreshReadData(int row)
{
    if (protocolMode == ReadProtocolWidget)
    {
        // 根据 数据类型更新
        switch (mVectorUnionType[row]->currentIndex())
        {
            case 0:
            {
                mVectorLineEdit1[row]->setText(QString::number(mVectorLineEdit1Value[row].data_float, 'f', 4));
                mVectorDataControl[row]->pVersaScrollBar->setValue(mVectorDataControlValue[row]);
                break;
            }
            case 1:
            {
                mVectorLineEdit1[row]->setText(QString("%1").arg(mVectorLineEdit1Value[row].data_int32));
                mVectorDataControl[row]->pVersaScrollBar->setValue(mVectorDataControlValue[row]);
                break;
            }
            case 2:
            {
                mVectorLineEdit1[row]->setText(QString("%1").arg(mVectorLineEdit1Value[row].data_u32));
                mVectorDataControl[row]->pVersaScrollBar->setValue(mVectorDataControlValue[row]);
                break;
            }
        }
    }
}


/*!
 * 添加显示控件
 * @param dataTable
 * @param start
 * @param row
 */
void VersaProtocolWidget::insertProtocolRow(QTableWidget *dataTable, int start, int row)
{
    // 调用基类的插入行方法
    mVectorUnionType[row] = new QComboBox(dataTable);
    dataTable->setCellWidget(row + start, 0, mVectorUnionType[row]);

    mVectorLineEdit1[row] = new QLineEdit(dataTable);
    dataTable->setCellWidget(row + start, 1, mVectorLineEdit1[row]);

    mVectorLineEdit2[row] = new QLineEdit(dataTable);
    dataTable->setCellWidget(row + start, 2, mVectorLineEdit2[row]);

    mVectorLineEdit3[row] = new QLineEdit(dataTable);
    dataTable->setCellWidget(row + start, 3, mVectorLineEdit3[row]);

    mVectorDataControl[row] = new VersaDataControl();
    dataTable->setCellWidget(row + start, 4, mVectorDataControl[row]);

    // 禁用鼠标滚轮，避免不小心更改数据格式
    wheelEventFilter *filterUnionType = new wheelEventFilter(mVectorUnionType[row]);
    mVectorUnionType[row]->installEventFilter(filterUnionType);
    mVectorUnionType[row]->addItem("float32");
    mVectorUnionType[row]->addItem("int32");
    mVectorUnionType[row]->addItem("uint32");
    mVectorUnionType[row]->setView(new QListView(mVectorUnionType[row]));
    mVectorUnionType[row]->setCurrentIndex(0);

    mVectorLineEdit1[row]->setText("0");
    mVectorLineEdit2[row]->setText("0");
    mVectorLineEdit3[row]->setText("1.0");

    // 如果是发送窗口
    if (protocolMode == SendProtocolWidget)
    {
        limitInputFloat32(mVectorLineEdit1[row]);// 原始数据
        limitInputFloat32(mVectorLineEdit2[row]);// 发送数据，即最终通过协议接口发送的数据
        limitInputFloat32(mVectorLineEdit3[row]);// 缩放比例。设置为0，可以使 发送数据 = 原始数据
        mVectorLineEdit2[row]->setReadOnly(true);
        // 数据格式被修改，其它也需要修改
        connect(mVectorUnionType[row], QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this, row](int index)
                {
                    mVectorUnionValue[row] = index;
                    switch (index)
                    {
                        case 0:
                        {
                            limitInputFloat32(mVectorLineEdit2[row]);
                            break;
                        }
                        case 1:
                        {
                            limitInputInt32(mVectorLineEdit2[row]);
                            break;
                        }
                        case 2:
                        {
                            limitInputUInt32(mVectorLineEdit2[row]);
                            break;
                        }
                    }
                    updataSendData(row);
                    updataSendDataEnable[row] = true;
                });

        // mVectorLineEdit1 原始数据 - 数据被修改
        connect(mVectorLineEdit1[row], &QLineEdit::textChanged,
                this, [this, row](const QString &text)
                {
                    mVectorLineEdit1Value[row].data_float = text.toFloat();
                    updataSendData(row);
                    updataSendDataEnable[row] = true;
                });

        // mVectorLineEdit2 发送数据被修改，此数据需要计算得到，无法编辑

        // mVectorLineEdit3 数据被修改
        connect(mVectorLineEdit3[row], &QLineEdit::textChanged,
                this, [this, row](const QString &text)
                {
                    mVectorLineEdit3Value[row].data_float = text.toFloat();
                    updataSendData(row);
                    updataSendDataEnable[row] = true;
                });

        // 滚动条，滑块被修改
        connect(mVectorDataControl[row]->pVersaScrollBar, &VersaSlider::valueChanged,
                this, [this, row](int value)
                {
                    mVectorDataControlValue[row] = value;
                    updataSendData(row);
                    updataSendDataEnable[row] = true;
                });

        mVectorUnionType[row]->setCurrentIndex(mVectorUnionValue[row]);
        mVectorLineEdit1[row]->setText(QString("%1").arg(mVectorLineEdit1Value[row].data_float));
        mVectorLineEdit3[row]->setText(QString("%1").arg(mVectorLineEdit3Value[row].data_float));
        mVectorDataControl[row]->pVersaScrollBar->setValue(mVectorDataControlValue[row]);
        updataSendData(row);
        updataSendDataEnable[row] = true;
    } else if (protocolMode == ReadProtocolWidget)// 如果是接收窗口
    {
        limitInputFloat32(mVectorLineEdit1[row]); // 原始数据，即最开始通过协议接收到的数据
        limitInputFloat32(mVectorLineEdit2[row]); // 数据偏移，设置数据偏移量，以便于观察数据波动
        limitInputFloat32(mVectorLineEdit3[row]); // 缩放比例，对偏移后数据进行缩放，以便于观察波动
        mVectorLineEdit1[row]->setReadOnly(true);
        mVectorDataControl[row]->pVersaScrollBar->setEnabled(false);
        // 数据格式被修改
        connect(mVectorUnionType[row], QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this, row](int index)
                {
                    mVectorUnionValue[row] = index;
                    switch (index)
                    {
                        case 0:
                        {
                            limitInputFloat32(mVectorLineEdit1[row]);
                            break;
                        }
                        case 1:
                        {
                            limitInputInt32(mVectorLineEdit1[row]);
                            break;
                        }
                        case 2:
                        {
                            limitInputUInt32(mVectorLineEdit1[row]);
                            break;
                        }
                    }
                    updataReadData(row);
                    refreshReadDataEnable[row] = true;
                });
        // mVectorLineEdit1 原始数据，即最开始通过协议接收到的数据

        // mVectorLineEdit2 原始数据 - 数据被修改
        connect(mVectorLineEdit2[row], &QLineEdit::textChanged,
                this, [this, row](const QString &text)
                {
                    mVectorLineEdit2Value[row].data_float = text.toFloat();
                    updataReadData(row);
                    refreshReadDataEnable[row] = true;
                });

        // mVectorLineEdit3 数据被修改
        connect(mVectorLineEdit3[row], &QLineEdit::textChanged,
                this, [this, row](const QString &text)
                {
                    mVectorLineEdit3Value[row].data_float = text.toFloat();
                    updataReadData(row);
                    refreshReadDataEnable[row] = true;
                });

        mVectorUnionType[row]->setCurrentIndex(mVectorUnionValue[row]);
        mVectorLineEdit2[row]->setText(QString("%1").arg(mVectorLineEdit2Value[row].data_float));
        mVectorLineEdit3[row]->setText(QString("%1").arg(mVectorLineEdit3Value[row].data_float));
        updataReadData(row);
        refreshReadDataEnable[row] = true;
    }
}

#include "VersaProtocolWidget.moc"