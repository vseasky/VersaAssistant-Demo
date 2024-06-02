//
// Created by vSeasky on 2023/9/20.
//

#include "VersaNormalSendWidget.h"
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextCodec>
#include <QRegularExpression>
#include <QMimeData>
#include <QDebug>

QString keepHexOnly(const QString &input)
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

VersaNormalSendWidget::VersaNormalSendWidget(QWidget *parent)
		: QTextEdit(parent)
{
	// 注册 QByteArray 类型
	qRegisterMetaType<QByteArray>("QByteArray&");
	connect(this, &QTextEdit::textChanged, this,
	        &VersaNormalSendWidget::currentTextChanged);
}

VersaNormalSendWidget::~VersaNormalSendWidget()
{

}

void VersaNormalSendWidget::loadDataBuffer(const QByteArray &buffer)
{
	this->setText(buffer);
}

void VersaNormalSendWidget::restoreDataBuffer(const QByteArray &buffer)
{
	this->setText(buffer);
}

QByteArray VersaNormalSendWidget::getSaveDataBuffer()
{
	return this->toPlainText().toUtf8();
}

void VersaNormalSendWidget::readyWriteFileText(const QString &address, quint16 port)
{
	
	QString dirpath = QFileDialog::getOpenFileName(nullptr, QStringLiteral("发送文件"),
	                                               qApp->applicationDirPath()+"/readyWrite.txt",
	                                               QString(tr("*.*")));
	if (dirpath != NULL)
	{
		QFile file(dirpath);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::critical(NULL,
			                      tr("提示"),
			                      tr("无法打开该文件！"));
			return;
		}
		else
		{
			QTextStream readStream(&file);
			QByteArray byteArray; // 创建 QByteArray 对象
			while (!readStream.device()->atEnd())
			{
				byteArray.clear();
				byteArray = readStream.device()->read(1024);
				emit readyWriteSignals(address, port, byteArray);
			}
			file.close();
		}
	}
}

void VersaNormalSendWidget::changeTcp(const QString &address, quint16 port)
{
	sendAddr = address;
	sendPort = port;
}

/*!
 * 准备发送数据
 * @param address
 * @param port
 */
void VersaNormalSendWidget::readyWrite(const QString &address, quint16 port)
{
	sendAddr = address;
	sendPort = port;
	if (registerEnableHex == true)
	{
		mSendBuffer = QByteArray::fromHex(toPlainText().replace(" ", "").toUtf8());
	}
	else
	{
		mSendBuffer = QTextCodec::codecForName(textCode)->fromUnicode(toPlainText().toUtf8());
	}
	if (registerNewLineEnable == true)
	{
		mSendBuffer.append("\n");
	}
	if (mSendBuffer.size() == 124)
	{
		mSendBuffer.append('\0');
	}
	emit readyWriteSignals(address, port, mSendBuffer);
}

void VersaNormalSendWidget::insertFromMimeData(const QMimeData *source)
{
	if (source->hasText())
	{
		QString plainText = source->text();
		insertPlainText(plainText);
	}
}

void VersaNormalSendWidget::keyPressEvent(QKeyEvent *event)
{
	if ((registerEnterEnable == true) && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
	{
		emit readyWrite(sendAddr, sendPort);
	}
	else
	{
		// 其他键盘事件处理
		QTextEdit::keyPressEvent(event);
	}
}

void VersaNormalSendWidget::setTextCodec(const QByteArray &codeText)
{
	textCode = codeText;
}

void VersaNormalSendWidget::setHexEnable(bool enValue)
{
	registerEnableHex = enValue;
	if (registerEnableHex == true)
	{
		// 正常文本切换为 16 进制文本
		textBuffer = this->toPlainText().toUtf8().toHex();
		setPlainText(QString(textBuffer).toUpper().replace(QRegularExpression("(..)"), "\\1 "));
	}
	else
	{
		//  16 进制文本 转换为正常文本
		textBuffer = QByteArray::fromHex(this->toPlainText().replace(" ", "").toUtf8());
		setPlainText(textBuffer);
	}
}

void VersaNormalSendWidget::setNewLineEnable(bool enValue)
{
	registerNewLineEnable = enValue;
}


void VersaNormalSendWidget::setEnterEnable(bool enValue)
{
	registerEnterEnable = enValue;
}

void VersaNormalSendWidget::currentTextChanged()
{
	if (registerEnableHex == true)
	{
		QRegularExpression hexSpacePattern("^([0-9A-Fa-f]{2} )*([0-9A-Fa-f]{1,2})?$");
		QRegularExpressionMatch match = hexSpacePattern.match(this->toPlainText());
		if (match.hasMatch() == false)
		{
			QTimer::singleShot(0, this, [=]()
			{
				QString currentString;
				currentString = keepHexOnly(this->toPlainText()).toUpper().replace(QRegularExpression("(..)"), "\\1 ");
				setPlainText(currentString);
				moveCursor(QTextCursor::End);
			});
		}
	}
}
