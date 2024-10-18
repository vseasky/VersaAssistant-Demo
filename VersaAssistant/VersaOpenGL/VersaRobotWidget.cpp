/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaOpenGL\VersaRobotWidget.cpp
 * @Description  : 
 */


#include "VersaRobotWidget.h"
#include <QThread>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QTreeWidget>
#include <QLineEdit>
#include <QHeaderView>
#include <QApplication>
#include <QMouseEvent>
#include <QDockWidget>
#include <QSplitter>

class RobotwheelEventFilter : public QObject
{
Q_OBJECT

public:
	RobotwheelEventFilter(QObject *parent = nullptr) : QObject(parent)
	{}

protected:
	bool eventFilter(QObject *obj, QEvent *event) override
	{
		if (obj->isWidgetType() && event->type() == QEvent::Wheel)
		{
			QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
			// 禁用鼠标滚轮事件
			wheelEvent->ignore();
			return true;
		}
		return false;
	}
};


VersaRobotWidget::VersaRobotWidget(QWidget *parent, const QString filepath) :
		QWidget(parent)
{
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->setSpacing(0);
	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	
	pOpenGLWidget = new VersaOpenGLWidget(this, filepath);
	pNodeWidget = new QWidget(this);
	splitter->addWidget(pOpenGLWidget);
	splitter->addWidget(pNodeWidget);
	
	pLayout->addWidget(splitter);
	this->setLayout(pLayout);
	
	// 第一个子窗口的大小为75，第二个子窗口的大小为25
	QList<int> sizes;
	sizes << 75 << 25;
	splitter->setSizes(sizes);
	splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	connect(pOpenGLWidget, &VersaOpenGLWidget::loadAlready, this, [=]()
	{
		creatRootNodeWidget(pOpenGLWidget->mRootNode.data());
	});
	
	refreshTimer.setTimerType(Qt::PreciseTimer);
	
	refreshTimer.setInterval(25);
	
	connect(&refreshTimer, &QTimer::timeout, this, &VersaRobotWidget::refreshValue);
	
	refreshTimer.start();
}

VersaRobotWidget::~VersaRobotWidget()
{
	refreshValueEnable = false;
	refreshTimer.stop();
	freeRobotWidget();
	// 释放指针所指向的对象
	if (!pVersaScene.isNull())
	{
		delete pVersaScene;
		pVersaScene = nullptr;
	}
	
	if (!pRootNodeCom.isNull())
	{
		delete pRootNodeCom;
		pRootNodeCom = nullptr;
	}
	
	if (!pOpenGLWidget.isNull())
	{
		delete pOpenGLWidget;
		pOpenGLWidget = nullptr;
	}
	
	if (!pNodeWidget.isNull())
	{
		delete pNodeWidget;
		pNodeWidget = nullptr;
	}
	
	if (!pNodeLayout.isNull())
	{
		delete pNodeLayout;
		pNodeLayout = nullptr;
	}
	
	if (!pNodeTreeWidget.isNull())
	{
		delete pNodeTreeWidget;
		pNodeTreeWidget = nullptr;
	}
	
	if (!pNodeControlWidget.isNull())
	{
		delete pNodeControlWidget;
		pNodeControlWidget = nullptr;
	}
	
	if (currentQMatrix4x4 != nullptr)
	{
		delete currentQMatrix4x4;
		currentQMatrix4x4 = nullptr;
	}
	
	if (currentQVector3D != nullptr)
	{
		delete currentQVector3D;
		currentQVector3D = nullptr;
	}
	
	for (auto slider: listQSlider)
	{
		if (slider && !slider->parent())
		{
			slider->deleteLater();
			slider = nullptr;
		}
	}
	listQSlider.clear();
	for (auto spinBox: listQSpinBox)
	{
		if (spinBox && !spinBox->parent())
		{
			spinBox->deleteLater();
			spinBox = nullptr;
		}
	}
	listQSpinBox.clear();
}

/*!
 * 添加控制小部件
 * @param pNode
 */
void VersaRobotWidget::addNodeControl(VersaNode *pNode)
{
	if ((pNode->mName == ""))
	{
		return;
	}
	
	if (pNode->mNodeAction == rotationAction)// 绕单轴旋转
	{
		// 添加旋转操作小部件
		int currentRowCount = pNodeControlWidget->rowCount();
		
		pNodeControlWidget->insertRow(currentRowCount);
		pNodeControlWidget->setVerticalHeaderItem(currentRowCount, new QTableWidgetItem(QString(pNode->mName)));
		
		QLineEdit *pLineEditName = new QLineEdit(pNodeControlWidget);
		pLineEditName->setText(tr("旋转"));
		pLineEditName->setReadOnly(true);
		pLineEditName->setAlignment(Qt::AlignCenter);
		
		QSpinBox *pQSpinBoxEditID = new QSpinBox(pNodeControlWidget);
		pQSpinBoxEditID->setValue(currentRowCount + 1);
		pQSpinBoxEditID->setAlignment(Qt::AlignCenter);
		RobotwheelEventFilter *filterQSpinBoxEdit = new RobotwheelEventFilter(pQSpinBoxEditID);
		pQSpinBoxEditID->installEventFilter(filterQSpinBoxEdit);
		listQSpinBox.append(pQSpinBoxEditID);
		
		QSlider *pControlQSlider = new QSlider(Qt::Horizontal, pNodeControlWidget);
		RobotwheelEventFilter *filterControlQSlider = new RobotwheelEventFilter(pControlQSlider);
		pControlQSlider->installEventFilter(filterControlQSlider);
		listQSlider.append(pControlQSlider);
		
		pNodeControlWidget->setCellWidget(currentRowCount, 0, pLineEditName);
		pNodeControlWidget->setCellWidget(currentRowCount, 1, pQSpinBoxEditID);
		pNodeControlWidget->setCellWidget(currentRowCount, 2, pControlQSlider);
		double calcValue = 0;
		// 设置取值范围 (-1000000.0, 1000000.0)
		pControlQSlider->setRange(-1000000.0, 1000000.0);
		// 设置初始值
		pControlQSlider->setValue(0);
		calcValue = 1000000.0 / 180.0;
		// 控件值改变
		connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
		{
			if (pNode->mNodeAction == rotationAction)
			{
				double rotationAngle = (float(value) / calcValue);
				if (pNode->mRotation3DAxisEnable.x() != 0)
				{
					pNode->mRotation3DAngle.setX(rotationAngle);
				}
				if (pNode->mRotation3DAxisEnable.y() != 0)
				{
					pNode->mRotation3DAngle.setY(rotationAngle);
				}
				if (pNode->mRotation3DAxisEnable.z() != 0)
				{
					pNode->mRotation3DAngle.setZ(rotationAngle);
				}
				// 更新为四元数，姿态求解
				pOpenGLWidget->updataTranslateMatrix(pNode);
			}
		});
	}
	else if (pNode->mNodeAction == transformAction)
	{
		// 添加移动和旋转小部件
		for (uint8_t iAction = 0; iAction < 6; iAction++)
		{
			int currentRowCount = pNodeControlWidget->rowCount();
			pNodeControlWidget->insertRow(currentRowCount);
			pNodeControlWidget->setVerticalHeaderItem(currentRowCount, new QTableWidgetItem(QString(pNode->mName)));
			
			QLineEdit *pLineEditName = new QLineEdit(pNodeControlWidget);
			pLineEditName->setReadOnly(true);
			pLineEditName->setAlignment(Qt::AlignCenter);
			
			QSpinBox *pQSpinBoxEditID = new QSpinBox(pNodeControlWidget);
			pQSpinBoxEditID->setValue(currentRowCount + 1);
			pQSpinBoxEditID->setAlignment(Qt::AlignCenter);
			RobotwheelEventFilter *filterQSpinBoxEdit = new RobotwheelEventFilter(pQSpinBoxEditID);
			pQSpinBoxEditID->installEventFilter(filterQSpinBoxEdit);
			listQSpinBox.append(pQSpinBoxEditID);
			
			QSlider *pControlQSlider = new QSlider(Qt::Horizontal, pNodeControlWidget);
			RobotwheelEventFilter *filterControlQSlider = new RobotwheelEventFilter(pControlQSlider);
			pControlQSlider->installEventFilter(filterControlQSlider);
			listQSlider.append(pControlQSlider);
			
			pNodeControlWidget->setCellWidget(currentRowCount, 0, pLineEditName);
			pNodeControlWidget->setCellWidget(currentRowCount, 1, pQSpinBoxEditID);
			pNodeControlWidget->setCellWidget(currentRowCount, 2, pControlQSlider);
			double calcValue = 0;
			if (iAction < 3)
			{
				// 设置取值范围 (-1000000.0, 1000000.0)
				pControlQSlider->setRange(-1000000.0, 1000000.0);
				// 设置初始值
				pControlQSlider->setValue(0);
				calcValue = 1000000.0 / 2.0;
			}
			else if (iAction >= 3)
			{
				// 旋转操作
				// 设置取值范围 (-1000000.0, 1000000.0)
				pControlQSlider->setRange(-1000000.0, 1000000.0);
				// 设置初始值
				pControlQSlider->setValue(0);
				calcValue = 1000000.0 / 180.0;
			}
			switch (iAction)
			{
				// 平移操作
				case 0:
				{
					pLineEditName->setText(tr("平移X"));
					connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
					{
						if (pNode->mNodeAction == transformAction)
						{
							double moveValue = (float(value) / calcValue);
							pNode->mTranslate3D.setX(moveValue);
							pOpenGLWidget->updataTranslateMatrix(pNode);
						}
					});
					break;
				}
				case 1:
				{
					pLineEditName->setText(tr("平移Y"));
					connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
					{
						if (pNode->mNodeAction == transformAction)
						{
							double moveValue = (float(value) / calcValue);
							pNode->mTranslate3D.setY(moveValue);
							pOpenGLWidget->updataTranslateMatrix(pNode);
						}
					});
					break;
				}
				case 2:
				{
					pLineEditName->setText(tr("平移Z"));
					connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
					{
						if (pNode->mNodeAction == transformAction)
						{
							double moveValue = (float(value) / calcValue);
							pNode->mTranslate3D.setZ(moveValue);
							pOpenGLWidget->updataTranslateMatrix(pNode);
						}
					});
					break;
				}
					
					// 旋转操作
				case 3:
				{
					
					pLineEditName->setText(tr("旋转X"));
					connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
					{
						double rotationAngle = (float(value) / calcValue);
						if (pNode->mNodeAction == transformAction)
						{
							pNode->mRotation3DAngle.setX(rotationAngle);
							pOpenGLWidget->updataTranslateMatrix(pNode);
						}
					});
					break;
				}
				case 4:
				{
					pLineEditName->setText(tr("旋转Y"));
					connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
					{
						double rotationAngle = (float(value) / calcValue);
						if (pNode->mNodeAction == transformAction)
						{
							pNode->mRotation3DAngle.setY(rotationAngle);
							pOpenGLWidget->updataTranslateMatrix(pNode);
						}
					});
					break;
				}
				case 5:
				{
					pLineEditName->setText(tr("旋转Z"));
					connect(pControlQSlider, &QSlider::valueChanged, this, [=](int value)
					{
						double rotationAngle = (float(value) / calcValue);
						if (pNode->mNodeAction == transformAction)
						{
							pNode->mRotation3DAngle.setZ(rotationAngle);
							pOpenGLWidget->updataTranslateMatrix(pNode);
						}
					});
					break;
				}
			}
			
		}
		
	}
}

void VersaRobotWidget::freeRobotWidget()
{
	pOpenGLWidget->cleanup();
}

// 解析
void VersaRobotWidget::parseNode(VersaNode *pNode, QTreeWidgetItem *pItem)
{
	QTreeWidgetItem *pChildItem = new QTreeWidgetItem(QStringList() << pNode->mName);
	addNodeControl(pNode);
	pItem->addChild(pChildItem);
	// 递归绘制该节点的子节点
	for (int inn = 0; inn < pNode->mNodes.size(); ++inn)
	{
		parseNode(&pNode->mNodes[inn], pChildItem);
	}
}

void VersaRobotWidget::refreshValue()
{
	if (refreshValueEnable == false)
	{
		return;
	}
	
	if ((dateValue.size() > 0) && (listQSlider.size() > 0))
	{
		for (uint16_t iSlider = 0; iSlider < qMin(dateValue.size(), listQSlider.size()); iSlider++)
		{
			int index = listQSpinBox[iSlider]->value() - 1;
			// 更新数据
			if (index < dateValue.size())
			{
				listQSlider[iSlider]->setValue(dateValue[index]);
				listQSlider[iSlider]->triggerAction(QAbstractSlider::SliderMove);
			}
		}
	}
	refreshValueEnable = false;
}

void VersaRobotWidget::setNodeRobotValue(const QVector<double> &keys, const QVector<double> &values)
{
	static int length = 0;
	dateValue = values;
	if (length != dateValue.size())
	{
		length = dateValue.size();
		for (uint16_t iSpinbox = 0; iSpinbox < listQSpinBox.size(); iSpinbox++)
		{
			listQSpinBox[iSpinbox]->setMinimum(1);
			listQSpinBox[iSpinbox]->setMaximum(length);
		}
	}
	refreshValueEnable = true;
}

/*!
 * 创建根节点小部件
 * @param rootNode
 */
void VersaRobotWidget::creatRootNodeWidget(VersaNode *rootNode)
{
	QSplitter *splitter = new QSplitter(Qt::Vertical, this);
	pNodeLayout = new QVBoxLayout(pNodeWidget);
	pNodeLayout->setContentsMargins(0, 0, 0, 0);
	pNodeLayout->setSpacing(0);
	// 层级显示窗口
	pNodeTreeWidget = new QTreeWidget(pNodeWidget);
	// 控制窗口
	pNodeControlWidget = new QTableWidget(pNodeWidget);
	
	pNodeControlWidget->setColumnCount(3);
	pNodeControlWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	pNodeControlWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	pNodeControlWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	pNodeControlWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	pNodeControlWidget->setColumnWidth(0, 64);
	pNodeControlWidget->setColumnWidth(1, 64);
	QStringList headers;
	headers << tr("操作")
	        << tr("数据")
	        << tr("控制");
	pNodeControlWidget->setHorizontalHeaderLabels(headers);
	
	splitter->addWidget(pNodeTreeWidget);
	splitter->addWidget(pNodeControlWidget);
	
	
	pNodeLayout->addWidget(splitter);
	pNodeWidget->setLayout(pNodeLayout);
	
	// 第一个子窗口的大小为20，第二个子窗口的大小为80
	splitter->setStretchFactor(0, 15); // 第一个子窗口的伸展因子为2
	splitter->setStretchFactor(1, 85); // 第二个子窗口的伸展因子为1
	splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	{
		pNodeTreeWidget->setHeaderLabels(QStringList() << "Robot");
		listQSlider.clear();
		listQSpinBox.clear();
		QTreeWidgetItem *pTopItem = new QTreeWidgetItem(QStringList() << rootNode->mName);
		addNodeControl(rootNode);
		
		for (uint16_t irootNode = 0; irootNode < rootNode->mNodes.size(); irootNode++)
		{
			parseNode(&rootNode->mNodes[irootNode], pTopItem);
		}
		pNodeTreeWidget->addTopLevelItem(pTopItem);
	}
}

#include "VersaRobotWidget.moc"