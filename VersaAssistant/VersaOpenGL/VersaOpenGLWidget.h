#ifndef VERSAOPENGLWIDGET_H
#define VERSAOPENGLWIDGET_H

#include <string>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include "VersaOpenGLModel.h"
#include <QTimer>

class VersaOpenGLWidget : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
Q_OBJECT
public:
	QSharedPointer<VersaNode> mRootNode = nullptr;
	VersaOpenGLWidget(QWidget *parent = 0, const QString filepath = "");
	~VersaOpenGLWidget();
	void cleanup();
	VersaNode *findNode(VersaNode *currentNode, const QString &name);
	VersaNode *findNodeParent(VersaNode *currentNode, const QString &name);
	void changeChildParent(VersaNode *currentNode, const QString &child, const QString &parent);
	void updataTranslateMatrix(VersaNode *pNode);
signals:
	void loadAlready();
protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int width, int height) override;
	bool event(QEvent *event) override;
private:
	void createShaderProgram(QString vShader, QString fShader);
	bool createBuffers();
	void createAttributes();
	void createLightingAndMatrices();
	void setMaterialUniforms(const VersaMaterial *material);
	void drawNode(const VersaNode *pNode, QMatrix4x4 drawScaleMatrix);
	
	VersaScene mVersaScene;//实例化场景
	
	QOpenGLShaderProgram mShaderProgram;
	
	QOpenGLVertexArrayObject mVao;
	QOpenGLBuffer mVertexBuffer;
	QOpenGLBuffer mNormalBuffer;
	QOpenGLBuffer mTextureUVBuffer;
	QOpenGLBuffer mIndexBuffer;
	
	QMatrix4x4 mProjection; //投影
	QMatrix4x4 mCameraView; //相机视角
	VersaLight mLightInfo;
	VersaMaterial mMaterialInfo;//默认材质
	
	const QString mFilepath;
	bool m_isCreateBuffers = false;
	bool m_isRendering = false;
};

#endif // VERSAOPENGLWIDGET_H
