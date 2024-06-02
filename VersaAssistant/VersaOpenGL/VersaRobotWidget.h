//
// Created by vSeasky on 2023/9/25.
//

#ifndef VERSA_ASSISTANT_VERSAROBOTWIDGET_H
#define VERSA_ASSISTANT_VERSAROBOTWIDGET_H

#include <QWidget>
#include <QPointer>
#include <VersaScene.h>
#include <QComboBox>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QDockWidget>
#include <QTimer>
#include <QMap>
#include "VersaOpenGLWidget.h"

class VersaRobotWidget : public QWidget
{
Q_OBJECT
public:
	VersaRobotWidget(QWidget *parent = 0, const QString filepath = "");
	~VersaRobotWidget();
	
	void addNodeControl(VersaNode *pNode);
	void creatRootNodeWidget(VersaNode *rootNode);
	void parseNode(VersaNode *pNode, QTreeWidgetItem *pItem);
	void freeRobotWidget();
public slots:
	void refreshValue();
	void setNodeRobotValue(const QVector<double> &keys, const QVector<double> &values);

protected:
private:
	QPointer<VersaScene> pVersaScene = nullptr; // 实例化场景
	QPointer<VersaOpenGLWidget> pOpenGLWidget = nullptr;
	
	QPointer<QComboBox> pRootNodeCom = nullptr;
	QPointer<QWidget> pNodeWidget = nullptr; // 信息显示
	QPointer<QVBoxLayout> pNodeLayout = nullptr;
	QPointer<QTreeWidget> pNodeTreeWidget = nullptr;
	QPointer<QTableWidget> pNodeControlWidget = nullptr;
	
	QMatrix4x4 *currentQMatrix4x4 = nullptr;
	QVector3D *currentQVector3D = nullptr;
	
	QVector<QSlider *> listQSlider;
	QVector<QSpinBox *> listQSpinBox;
	QVector<double> dateValue;
	bool refreshValueEnable = false;
	QTimer refreshTimer;
	int MaxSiderWidth = 450;
	int MinSiderWidth = 12;
};

#endif // VERSA_ASSISTANT_VERSAROBOTWIDGET_H
