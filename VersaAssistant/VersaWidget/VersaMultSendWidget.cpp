/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaMultSendWidget.cpp
 * @Description  : 
 */


#include "VersaMultSendWidget.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QTextCodec>
#include "VersaConfig.h"
#include <QDebug>

QString VersaMultTextkeepHexOnly(const QString &input)
{
	QRegularExpression regex("[0-9a-fA-F]+");
	QRegularExpressionMatchIterator i = regex.globalMatch(input);
	QString hexOnly;
	while (i.hasNext())
	{
		QRegularExpressionMatch match = i.next();
		hexOnly += match.captured(0);
	}
	return hexOnly;
}

VersaMultSendWidget::VersaMultSendWidget(QWidget *parent) :
		QWidget(parent)
{

}

void VersaMultSendWidget::loadDataBuffer(const QByteArray &buffer, int count)
{
	if (buffer.size() > 0)
	{
		restoreDataBuffer(buffer, count);
	}
	else
	{
		defaultConfigDataBuffer(count);
	}
	pMultQTableLayout = new QVBoxLayout(this);
	pMultQTableLayout->setSpacing(0);
	pMultQTableLayout->setContentsMargins(0, 0, 0, 0);
	pMultQTableWidget = creatMultTextWidget();
	pMultQTableLayout->addWidget(pMultQTableWidget);
	this->setLayout(pMultQTableLayout);
}

VersaMultSendWidget::~VersaMultSendWidget()
{
// 释放指针所指向的对象
	if (!pMultQTableLayout.isNull())
	{
		delete pMultQTableLayout;
		pMultQTableLayout = nullptr;
	}
	
	if (!pMultQTableWidget.isNull())
	{
		delete pMultQTableWidget;
		pMultQTableWidget = nullptr;
	}
}

QPointer<QTableWidget> VersaMultSendWidget::creatMultTextWidget()
{
	QTableWidget *tableWidget = new QTableWidget(this);
	tableWidget->setColumnCount(5);
	if (maxRowCount == 0)
	{
		return nullptr;
	}
	
	tableWidget->setRowCount(maxRowCount);
	// 自动调整列宽
	tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// 固定行高
	tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	tableWidget->verticalHeader()->setDefaultSectionSize(48);
	tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
	tableWidget->setColumnWidth(0, 48);
	tableWidget->setColumnWidth(1, 128);
	tableWidget->setColumnWidth(2, 96);
	tableWidget->setColumnWidth(4, 48);
	
	{
		QStringList headers;
		headers << tr("计时")
		        << tr("目标地址")
		        << tr("目标端口")
		        << tr("消息")
		        << tr("发送");
		tableWidget->setHorizontalHeaderLabels(headers);
	}
	
	for (uint16_t iRow = 0; iRow < maxRowCount; iRow++)
	{
		insertMultRow(tableWidget, iRow);
	}
	
	connect(tableWidget, &QTableWidget::itemChanged, this, [=](QTableWidgetItem *item)
	{
		if (item->column() == 3)
		{
			bool textHexCheck = false;
			if (registerEnableHex == true)
			{
				QRegularExpression hexSpacePattern("^([0-9A-Fa-f]{2} )*([0-9A-Fa-f]{1,2})?$");
				QRegularExpressionMatch match = hexSpacePattern.match(item->text());
				textHexCheck = match.hasMatch();
				if (textHexCheck == false)
				{
					QTimer::singleShot(0, this, [=]()
					{
						QString currentString;
						currentString = VersaMultTextkeepHexOnly(
								item->text()).toUpper()
								.replace(QRegularExpression("(..)"),
								         "\\1 ");
						item->setText(currentString);
					});
				}
			}
		}
	});
	
	return tableWidget;
}

void VersaMultSendWidget::resizeMultSendWidget(const int count)
{
	if ((pMultQTableLayout != nullptr) && (pMultQTableWidget != nullptr))
	{
		pMultQTableLayout->removeWidget(pMultQTableWidget);
		pMultQTableWidget->deleteLater();
		pMultQTableWidget = nullptr;
		
		maxRowCount = count;
		mVectorTimerEnable.resize(maxRowCount);
		mVectorAddress.resize(maxRowCount);
		mVectorPort.resize(maxRowCount);
		mVectorData.resize(maxRowCount);
		
		pMultQTableWidget = creatMultTextWidget();
		
		pMultQTableLayout->addWidget(pMultQTableWidget);
	}
}


void VersaMultSendWidget::defaultConfigDataBuffer(int count)
{
	QVector<int> dataLength;
	dataLength.resize(4);
	maxRowCount = count;
	mVectorTimerEnable.resize(maxRowCount);
	mVectorAddress.resize(maxRowCount);
	mVectorPort.resize(maxRowCount);
	mVectorData.resize(maxRowCount);
	for (uint32_t iRow = 0; iRow < maxRowCount; iRow++)
	{
		mVectorTimerEnable[iRow] = false;
		mVectorAddress[iRow].append("192.168.0.1");
		mVectorPort[iRow] = 32401;
	}
}

void VersaMultSendWidget::restoreDataBuffer(const QByteArray &buffer, int count)
{
	QDataStream stream(buffer);
	QVector<int> dataLength;
	dataLength.resize(4);
	if (buffer.size() >= sizeof(maxRowCount) + sizeof(dataLength))
	{
		stream >> maxRowCount;
		stream >> dataLength;
		if (buffer.size() >=
		    sizeof(maxRowCount) +
		    sizeof(dataLength) +
		    dataLength[0] * sizeof(int) +
		    dataLength[1] * sizeof(int) +
		    dataLength[2] * sizeof(int) +
		    dataLength[3] * sizeof(int))
		{
			if (dataLength[0] > 0)
			{
				mVectorTimerEnable.resize(dataLength[0]);
				stream >> mVectorTimerEnable;
			}
			if (dataLength[1] > 0)
			{
				mVectorAddress.resize(dataLength[1]);
				stream >> mVectorAddress;
			}
			if (dataLength[2] > 0)
			{
				mVectorPort.resize(dataLength[2]);
				stream >> mVectorPort;
			}
			if (dataLength[3] > 0)
			{
				mVectorData.resize(dataLength[3]);
				stream >> mVectorData;
			}
		}
		return;
	}
	defaultConfigDataBuffer(count);
	return;
}

QByteArray VersaMultSendWidget::getSaveDataBuffer()
{
	QByteArray mDataBuffer;
	QDataStream stream(&mDataBuffer, QIODevice::WriteOnly);
	
	QVector<int> dataLength;
	dataLength.resize(4);
	
	dataLength[0] = mVectorTimerEnable.size();
	dataLength[1] = mVectorAddress.size();
	dataLength[2] = mVectorPort.size();
	dataLength[3] = mVectorData.size();
	
	stream << maxRowCount;
	stream << dataLength;
	
	if (dataLength[0] > 0)
	{
		stream << mVectorTimerEnable;
	}
	if (dataLength[1] > 0)
	{
		stream << mVectorAddress;
	}
	if (dataLength[2] > 0)
	{
		stream << mVectorPort;
	}
	if (dataLength[3] > 0)
	{
		stream << mVectorData;
	}
	return mDataBuffer;
}

// 发送某一行的数据
void VersaMultSendWidget::writeIndex(int index)
{
	QString address;
	quint16 port = 0;
	QByteArray data;
	if (index < maxRowCount)
	{
		if (registerEnableHex == true)
		{
			data = QByteArray::fromHex(mVectorData[index].replace(" ", "").toUtf8());
		}
		else
		{
			data = QTextCodec::codecForName(textCode)->fromUnicode(mVectorData[index].toUtf8());
		}
		if (data.size() > 0)
		{
			if (registerEnableNewline == true)
			{
				data.append("\n");
			}
			if (data.size() == 124)
			{
				data.append('\0');
			}
			emit readyWriteSignals(mVectorAddress[index], mVectorPort[index], data);
		}
	}
}

void VersaMultSendWidget::setTextCodec(const QByteArray &codeText)
{
	textCode = codeText;
}

void VersaMultSendWidget::readyWrite()
{
	int thisIndex = currentIndex;
	// 直到找到第一个可以发送的数据
	while (mVectorTimerEnable[currentIndex] == false)
	{
		currentIndex++;
		if (currentIndex >= maxRowCount)
		{
			currentIndex = 0;
		}
		// 如果遍历完全部也没找到
		if (thisIndex == currentIndex)
		{
			return;
		}
	}
	writeIndex(currentIndex);
	currentIndex++;
	if (currentIndex >= maxRowCount)
	{
		currentIndex = 0;
	}
}

void VersaMultSendWidget::setHexEnable(bool enValue)
{
	if (pMultQTableWidget == nullptr)
	{
		return;
	}
	registerEnableHex = enValue;
	if (registerEnableHex == true)
	{
		// 正常文本切换为 16 进制文本
		for (uint16_t iRow = 0; iRow < maxRowCount; iRow++)
		{
			QLineEdit *lineEditData = qobject_cast<QLineEdit *>(pMultQTableWidget->cellWidget(iRow, 3));
			{
				QByteArray textBuffer = lineEditData->text().toUtf8().toHex();
				lineEditData->setText(QString(textBuffer)
						                      .toUpper()
						                      .replace(QRegularExpression("(..)"), "\\1 "));
			}
		}
	}
	else
	{
		// 正常文本切换为 16 进制文本
		for (uint16_t iRow = 0; iRow < maxRowCount; iRow++)
		{
			QLineEdit *lineEditData = qobject_cast<QLineEdit *>(pMultQTableWidget->cellWidget(iRow, 3));
			if (lineEditData)
			{
				QByteArray textBuffer =
						QByteArray::fromHex(lineEditData->text()
								                    .replace(" ", "")
								                    .toUtf8());
				lineEditData->setText(QString(textBuffer));
			}
			
		}
	}
}

void VersaMultSendWidget::setNewLineEnable(bool enValue)
{
	registerEnableNewline = enValue;
}

void VersaMultSendWidget::keyPressEvent(QKeyEvent *event)
{
	if ((registerEnterEnable == true) && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
	{
		emit readyWrite();
	}
	else
	{
		// 其他键盘事件处理
		QWidget::keyPressEvent(event);
	}
}

void VersaMultSendWidget::setEnterEnable(bool enValue)
{
	registerEnterEnable = enValue;
}

void VersaMultSendWidget::insertMultRow(QTableWidget *tableWidget, int row)
{
	QPushButton *multTimerEnableButton = new QPushButton(tableWidget);
	multTimerEnableButton->setCheckable(true);
	multTimerEnableButton->setIcon(ThemeIcon("IconCheck1", "AppBackgroundColor", 32));
	multTimerEnableButton->setChecked(mVectorTimerEnable[row]);
	tableWidget->setCellWidget(row, 0, multTimerEnableButton);
	connect(multTimerEnableButton, &QPushButton::clicked, this, [this, row](bool checked)
	{
		mVectorTimerEnable[row] = checked;
	});
	
	QPushButton *pMultSenButton = new QPushButton(tableWidget);
	pMultSenButton->setIcon(ThemeIcon("IconSend3", "DockWidget", 32));
	tableWidget->setCellWidget(row, 4, pMultSenButton);
	connect(pMultSenButton, &QPushButton::released, this, [this, row]()
	{
		writeIndex(row);
	});
	
	QLineEdit *lineEditAddr = new QLineEdit(tableWidget);
	lineEditAddr->setValidator(new QRegExpValidator(QRegExp(RegExpIpAddr), lineEditAddr));
	lineEditAddr->setText(mVectorAddress[row]);
	tableWidget->setCellWidget(row, 1, lineEditAddr);
	connect(lineEditAddr, &QLineEdit::textChanged, this, [this, row](const QString &text)
	{
		mVectorAddress[row] = text;
	});
	
	QLineEdit *lineEditPort = new QLineEdit(tableWidget);
	lineEditPort->setValidator(new QRegExpValidator(QRegExp(RegExpIpPort), lineEditPort));
	lineEditPort->setText(QString("%1").arg(mVectorPort[row]));
	tableWidget->setCellWidget(row, 2, lineEditPort);
	connect(lineEditPort, &QLineEdit::textChanged, this, [this, row](const QString &text)
	{
		mVectorPort[row] = text.toInt();
	});
	
	QLineEdit *lineEditData = new QLineEdit(tableWidget);
	lineEditData->setText(mVectorData[row]);
	tableWidget->setCellWidget(row, 3, lineEditData);
	connect(lineEditData, &QLineEdit::textChanged, this, [this, row](const QString &text)
	{
		mVectorData[row] = text.toUtf8();
	});
	
}
