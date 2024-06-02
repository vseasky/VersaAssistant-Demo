#include "VersaOpenGLWidget.h"
#include <QEvent>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

VersaOpenGLWidget::VersaOpenGLWidget(QWidget *parent, const QString filepath) :
		mVertexBuffer(QOpenGLBuffer::VertexBuffer),
		mIndexBuffer(QOpenGLBuffer::IndexBuffer), //必须在初始化时赋值
		mFilepath(filepath), //模型加载路径
		m_isCreateBuffers(false)
{
	//0x6cbca80
	setParent(parent);
	//设置OpenGL的版本信息
	QSurfaceFormat requestedFormat;
	// requestedFormat.setDepthBufferSize(24);
	// requestedFormat.setSamples(8);
	requestedFormat.setRenderableType(QSurfaceFormat::OpenGL);
	requestedFormat.setProfile(QSurfaceFormat::CoreProfile);
	//指定opengl版本为3.3
	requestedFormat.setVersion(3, 3);
	setFormat(requestedFormat);
	setCursor(Qt::ArrowCursor);
	
	setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
}

void VersaOpenGLWidget::cleanup()
{
	if (m_isRendering == false)
	{
		return;
	}
	
	m_isRendering = false;
	
	if (mRootNode)
	{
		mRootNode.clear();
		mRootNode = nullptr;
	}
	
	// 释放 OpenGL 相关资源
	mVertexBuffer.destroy();
	mNormalBuffer.destroy();
	mTextureUVBuffer.destroy();
	mIndexBuffer.destroy();
	
	if (mVao.isCreated())
	{
		mVao.destroy();
	}
	
	makeCurrent();
	mShaderProgram.release();
	mShaderProgram.removeAllShaders();
	doneCurrent();
	
	// // 释放场景对象
	mRootNode->mNodes.clear();
	mRootNode->mMeshes.clear();
}

VersaOpenGLWidget::~VersaOpenGLWidget()
{
	if (m_isRendering == false)
	{
		return;
	}
	
	m_isRendering = false;
	
	if (mRootNode)
	{
		mRootNode.clear();
		mRootNode = nullptr;
	}
	
	// 释放 OpenGL 相关资源
	mVertexBuffer.destroy();
	mNormalBuffer.destroy();
	mTextureUVBuffer.destroy();
	mIndexBuffer.destroy();
	
	if (mVao.isCreated())
	{
		mVao.destroy();
	}
	
	makeCurrent();
	mShaderProgram.release();
	mShaderProgram.removeAllShaders();
	doneCurrent();
};

// 创建着色器程序
void VersaOpenGLWidget::createShaderProgram(QString vShader, QString fShader)
{
	// 编译顶点着色器
	if (!mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vShader.toUtf8()))
	{
		qCritical() << "Unable to compile vertex shader. Log:" << mShaderProgram.log();
	}
	// 编译片段着色器
	if (!mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fShader.toUtf8()))
	{
		
		qCritical() << "Unable to compile fragment shader. Log:" << mShaderProgram.log();
	}
	// 将着色器链接到一个程序中
	if (!mShaderProgram.link())
	{
		qCritical() << "Unable to link shader program. Log:" << mShaderProgram.log();
	}
	mShaderProgram.bind();
}

// 创建缓冲区
bool VersaOpenGLWidget::createBuffers()
{
	if (mFilepath == "")
	{
		return false;
	}
	
	QFile file(mFilepath);
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}
	
	QString jsonString = file.readAll();
	file.close();
	// 将 JSON 字符串解析为 QJsonDocument 对象
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
	
	if (jsonDoc.isEmpty())
	{
		return false;
	}
	
	QJsonArray ModelPath = jsonDoc["ModelPath"].toArray();
	
	if (ModelPath.isEmpty())
	{
		return false;
	}
	
	VersaOpenGLModel model(&mVersaScene);
	for (uint16_t iModel = 0; iModel < ModelPath.size(); iModel++)
	{
		if (!model.importer(ModelPath[iModel].toString()))
		{
			return false;
		}
	}
	
	// 1.初始化并绑定VAO
	mVao.create();
	mVao.bind();
	// 2.创建并绑定顶点缓冲对象（VBO）
	mVertexBuffer.create();
	mVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	mVertexBuffer.bind();
	mVertexBuffer.allocate(&mVersaScene.mVertices[0], mVersaScene.mVertices.size() * sizeof(float));
	
	// 3.创建并绑定法线缓冲对象（VBO）
	mNormalBuffer.create();
	mNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	mNormalBuffer.bind();
	mNormalBuffer.allocate(&mVersaScene.mNormals[0], mVersaScene.mNormals.size() * sizeof(float));
	
	// 4.创建并绑定纹理坐标缓冲对象（VBO）
	if (mVersaScene.mTextureUv.size() != 0)
	{
		// 创建一个缓冲区并将顶点数据复制到其中
		mTextureUVBuffer.create();
		mTextureUVBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		mTextureUVBuffer.bind();
		int texSize = 0;
		for (int ii = 0; ii < mVersaScene.mTextureUv.size(); ++ii)
		{
			texSize += mVersaScene.mTextureUv.at(ii).size();
		}
		
		mTextureUVBuffer.allocate(&mVersaScene.mTextureUv[0][0], texSize * sizeof(float));
	}
	
	// 5.创建并绑定索引缓冲对象（EBO）
	mIndexBuffer.create();
	mIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	mIndexBuffer.bind();
	mIndexBuffer.allocate(&mVersaScene.mIndices[0], mVersaScene.mIndices.size() * sizeof(unsigned int));
	
	mRootNode = mVersaScene.mRootNode;
	// 获取 VersaRobot json 配置 对象
	QJsonObject versaRobotObj = jsonDoc.object()["VersaRobot"].toObject();
	if (!versaRobotObj.isEmpty())
	{
		// 模型名称
		QStringList modelsObjName = versaRobotObj.keys();
		// 模型个数
		for (uint16_t iObj = 0; iObj < modelsObjName.count(); iObj++)
		{
			// 获取 modelsObjName[iObj] 对象的json配置
			QJsonObject modelObj = versaRobotObj.value(modelsObjName[iObj]).toObject();
			VersaNode *modelNodeObj = nullptr;
			modelNodeObj = findNode(&mVersaScene.rootNode, modelsObjName[iObj]);
			if (modelNodeObj != nullptr)
			{
				QStringList nodesObjName = modelObj.keys();
				
				for (uint16_t iNode = 0; iNode < nodesObjName.count(); iNode++)
				{
					// 获取 robot-node1 对象
					QJsonObject nodeObj = modelObj.value(nodesObjName[iNode]).toObject();
					// 获取对象配置的可操作动作
					QString nodeObjAction = nodeObj.value("action").toString();
					
					if (nodeObjAction == "rotation")// 旋转动作
					{
						// 获取 rotation_coordinates 数组
						QJsonArray rotationCoordinatesArr = nodeObj.value("rotation_coordinates").toArray();
						// 获取 rotation_axis 数组
						QJsonArray rotationAxisArrEnable = nodeObj.value("rotation_axis_enable").toArray();
						VersaNode *currentNode = nullptr;
						currentNode = findNode(&mVersaScene.rootNode, nodesObjName[iNode]);
						if (currentNode != nullptr)
						{
							if ((rotationAxisArrEnable.size() >= 3) && (rotationCoordinatesArr.size() >= 3))
							{
								currentNode->mRotation3DCoordinates.setX(rotationCoordinatesArr[0].toDouble());
								currentNode->mRotation3DCoordinates.setY(rotationCoordinatesArr[1].toDouble());
								currentNode->mRotation3DCoordinates.setZ(rotationCoordinatesArr[2].toDouble());
								currentNode->mRotation3DAxisEnable.setX(rotationAxisArrEnable[0].toDouble());
								currentNode->mRotation3DAxisEnable.setY(rotationAxisArrEnable[1].toDouble());
								currentNode->mRotation3DAxisEnable.setZ(rotationAxisArrEnable[2].toDouble());
								currentNode->mNodeAction = rotationAction;
							}
						}
					}
					else if (nodeObjAction == "transform")// 根节点旋转+移动操作
					{
						VersaNode *currentNode = nullptr;
						currentNode = findNode(&mVersaScene.rootNode, nodesObjName[iNode]);
						if (currentNode != nullptr)
						{
							currentNode->mNodeAction = transformAction;
							currentNode->mRotation3DCoordinates.setX(0);
							currentNode->mRotation3DCoordinates.setY(0);
							currentNode->mRotation3DCoordinates.setZ(0);
							currentNode->mRotation3DAxisEnable.setX(1.0);
							currentNode->mRotation3DAxisEnable.setY(1.0);
							currentNode->mRotation3DAxisEnable.setZ(1.0);
						}
					}
					// 设置参考节点
					QString nodeObjParent = nodeObj.value("parent").toString();
					if (!nodeObjParent.isEmpty())
					{
						changeChildParent(&mVersaScene.rootNode, nodesObjName[iNode], nodeObjParent);
					}
				}
			}
		}
		
	}
	
	return true;
}

// 创建属性
void VersaOpenGLWidget::createAttributes()
{
	if (m_isCreateBuffers == false)
	{
		return;
	}
	
	mVao.bind();
	
	mShaderProgram.bind();
	mVertexBuffer.bind();
	mShaderProgram.enableAttributeArray(0);        // 布局位置
	mShaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3);
	
	mNormalBuffer.bind();
	mShaderProgram.enableAttributeArray(1);
	mShaderProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3);
	
	if (!mTextureUVBuffer.isCreated())
	{
		return;
	}
	mTextureUVBuffer.bind();
	mShaderProgram.enableAttributeArray(2);
	mShaderProgram.setAttributeBuffer(2, GL_FLOAT, 0, 2);
	
}

// 设置灯光和矩阵
void VersaOpenGLWidget::createLightingAndMatrices()
{
	// 相机
	mCameraView.setToIdentity();
	mCameraView.lookAt(QVector3D(0.0f, 0, 1.2f),        // 相机的位置，即相机在世界空间中的位置。
	                   QVector3D(0.0f, 0.0f, 0.0f),     // 相机朝向的方向，即相机在世界空间中的朝向。
	                   QVector3D(0.0f, 1.0, 0.0f));     // 相机的上方向，即相机在世界空间中的上方向。
	
	// 投影
	mProjection.setToIdentity();
	mProjection.perspective(45.0f,  // 视角的垂直角度，以弧度为单位
	                        4.0f / 3.0f,  // 视角的宽高比
	                        0.3f,      // 近平面的距离。
	                        1000.0f);   // 远平面的距离。
	
	// 设置光源的位置，即光源在世界空间中的位置。
	mLightInfo.mPosition = QVector4D(0.0f, 0.0f, 30.0f, 0.5f);
	// 设置光源的亮度，即光源的颜色强度。
	mLightInfo.mIntensity = QVector3D(1.0f, 1.0f, 1.0f);
	// 设置材质的环境光，即光源对场景中的所有点的影响。
	mMaterialInfo.mAmbient = QVector3D(0.01f, 0.01f, 0.01f);
	// 设置材质的漫反射光，即光源对场景中的微小区域的影响。
	mMaterialInfo.mDiffuse = QVector3D(.9f, .9f, .9f);
	// 设置材质的镜面光，即光源对场景中的高光区域的影响。
	mMaterialInfo.mSpecular = QVector3D(.1f, .1f, .1f);
	// 设置材质的光泽度，即光源对场景中的高光区域的亮度。
	mMaterialInfo.mShinines = 45.0f;
}

// 计算姿态
void VersaOpenGLWidget::drawNode(const VersaNode *pNode, QMatrix4x4 drawScaleMatrix)
{
	drawScaleMatrix *= pNode->mTransformMatrix;
	QMatrix4x4 modelViewMatrix;
	QMatrix3x3 normalMatrix;
	QMatrix4x4 mvp;
	
	// 先缩放，再旋转，再平移 。
	modelViewMatrix = mCameraView * drawScaleMatrix;
	normalMatrix = modelViewMatrix.normalMatrix();
	mvp = mProjection * modelViewMatrix;
	mShaderProgram.setUniformValue("MV", modelViewMatrix);
	mShaderProgram.setUniformValue("N", normalMatrix);
	mShaderProgram.setUniformValue("MVP", mvp);
	
	for (int imeshes = 0; imeshes < pNode->mMeshes.size(); ++imeshes)
	{
		if (pNode->mMeshes[imeshes]->material->mName == QString("DefaultMaterial"))
		{
			setMaterialUniforms(&mMaterialInfo);
		}
		else
		{
			setMaterialUniforms(pNode->mMeshes[imeshes]->material.data());
		}
		{
			mVao.bind();
			glDrawElements(GL_TRIANGLES,
			               pNode->mMeshes[imeshes]->mIndexCount,
			               GL_UNSIGNED_INT,
			               (const void *) (pNode->mMeshes[imeshes]->mIndexOffset * sizeof(unsigned int)));
			mVao.release();
		}
	}
	// 递归绘制该节点的子节点
	for (int inn = 0; inn < pNode->mNodes.size(); ++inn)
	{
		drawNode(&pNode->mNodes[inn], drawScaleMatrix);
	}
}

// 设置材质
void VersaOpenGLWidget::setMaterialUniforms(const VersaMaterial *material)
{
	mShaderProgram.setUniformValue("Ka", material->mAmbient);
	mShaderProgram.setUniformValue("Kd", material->mDiffuse);
	mShaderProgram.setUniformValue("Ks", material->mSpecular);
	mShaderProgram.setUniformValue("shininess", material->mShinines);
}

VersaNode *VersaOpenGLWidget::findNode(VersaNode *currentNode, const QString &name)
{
	VersaNode *pVersaNode = nullptr;
	if (currentNode->mName == name)
	{
		return currentNode;
	}
	// 递归绘制该节点的子节点
	for (int inn = 0; inn < currentNode->mNodes.size(); ++inn)
	{
		pVersaNode = findNode(&currentNode->mNodes[inn], name);
		if (pVersaNode != nullptr)
		{
			return pVersaNode;
		}
	}
	return nullptr;
}


VersaNode *VersaOpenGLWidget::findNodeParent(VersaNode *currentNode, const QString &name)
{
	VersaNode *pVersaNode = nullptr;
	// 递归绘制该节点的子节点
	for (int inn = 0; inn < currentNode->mNodes.size(); ++inn)
	{
		if (currentNode->mNodes[inn].mName == name)
		{
			return currentNode;
		}
		pVersaNode = findNodeParent(&currentNode->mNodes[inn], name);
		if (pVersaNode != nullptr)
		{
			return pVersaNode;
		}
	}
	return nullptr;
}

void VersaOpenGLWidget::changeChildParent(VersaNode *currentNode, const QString &child, const QString &parent)
{
	VersaNode *oldParent = findNodeParent(currentNode, child);
	VersaNode *currentChild = findNode(currentNode, child);
	VersaNode *newParent = findNode(currentNode, parent);
	// 顶级节点不允许被修改，所有需要修改的一定有父元素
	if ((oldParent != nullptr) &&
	    (currentChild != nullptr) &&
	    (newParent != nullptr))
	{
		if (parent != oldParent->mName)
		{
			newParent->mNodes.append(*currentChild);
			for (uint16_t iNode = 0; iNode < oldParent->mNodes.size(); iNode++)
			{
				if (currentChild->mName == oldParent->mNodes[iNode].mName)
				{
					oldParent->mNodes.remove(iNode);
					break;
				}
			}
		}
	}
}

/*!
 * 姿态求解，更新为四元数
 * @param pNode
 */
void VersaOpenGLWidget::updataTranslateMatrix(VersaNode *pNode)
{
	// 平移矩阵
	pNode->mTranslateMatrix.setToIdentity();
	pNode->mTranslateMatrix.translate(pNode->mTranslate3D);
	
	QQuaternion pitch, yaw, roll;
	
	// 绕轴旋转矩阵
	if (pNode->mRotation3DAxisEnable.x() != 0)
	{
		pitch = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0),
		                                      pNode->mRotation3DAngle.x());
	}
	if (pNode->mRotation3DAxisEnable.y() != 0)
	{
		yaw = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0),
		                                    pNode->mRotation3DAngle.y());
	}
	if (pNode->mRotation3DAxisEnable.z() != 0)
	{
		roll = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1),
		                                     pNode->mRotation3DAngle.z());
	}
	
	QQuaternion rotation = yaw * pitch * roll;
	
	// 旋转矩阵
	pNode->mRotationMatrix.setToIdentity();
	pNode->mRotationMatrix.translate(pNode->mRotation3DCoordinates.x(),
	                                 pNode->mRotation3DCoordinates.y(),
	                                 pNode->mRotation3DCoordinates.z());
	pNode->mRotationMatrix.rotate(rotation);
	pNode->mRotationMatrix.translate(-pNode->mRotation3DCoordinates.x(),
	                                 -pNode->mRotation3DCoordinates.y(),
	                                 -pNode->mRotation3DCoordinates.z());
	
	// 缩放矩阵
	pNode->mScaleMatrix.setToIdentity();
	pNode->mScaleMatrix.scale(pNode->mScale);
	
	// 相对位置矩阵，控制中不操作，为模型加载时的相对位置
	// mRelativeMatrix
	
	// 最终的变换矩阵
	pNode->mTransformMatrix =
			pNode->mTranslateMatrix *
			pNode->mRotationMatrix *
			pNode->mScaleMatrix *
			pNode->mRelativeMatrix;
}


void VersaOpenGLWidget::initializeGL()
{
	if (!isValid())
	{
		return;
	}
	makeCurrent();
	// 1.初始化 Open GL 函数
	this->initializeOpenGLFunctions();
	// 2.创建着色器程序
	createShaderProgram(":/ads_fragment.vert", ":/ads_fragment.frag");
	// 3.创建缓冲区
	m_isCreateBuffers = createBuffers();
	if (m_isCreateBuffers == false)
	{
		doneCurrent();
		m_isRendering = false;
		return;
	}
	// 4.创建属性
	createAttributes();
	// 5.创建灯光和矩阵
	createLightingAndMatrices();
	// 6.全局启用
	glEnable(GL_DEPTH_TEST);
	// 7.透明色
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	doneCurrent();
	m_isRendering = true;
	emit loadAlready();
}

void VersaOpenGLWidget::paintGL()
{
	if (!m_isRendering)
	{
		return;
	}
	// 清除颜色和深度缓冲区
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!isValid())
	{
		return;
	}
	// 设置光照信息的着色器
	mShaderProgram.setUniformValue("lightPosition", mLightInfo.mPosition);
	mShaderProgram.setUniformValue("lightIntensity", mLightInfo.mIntensity);
	drawNode(mRootNode.data(), QMatrix4x4());
	update();
	doneCurrent();
}

void VersaOpenGLWidget::resizeGL(int width, int height)
{
	if (!m_isRendering)
	{
		return;
	}
	// 清除颜色和深度缓冲区
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!isValid())
	{
		return;
	}
	glViewport(0, 0, width, height);
	mProjection.setToIdentity();
	mProjection.perspective(65,  // 视角的垂直角度，以弧度为单位 45-90
	                        (float) width / height,// 视角的宽高比
	                        0.2f,    // 近平面的距离。
	                        10000.0f); // 远平面的距离。
	update();
	doneCurrent();
}

// 有一些无法解决的问题,暂时无法绘制
bool VersaOpenGLWidget::event(QEvent *event)
{
	if (!(event->type() == QEvent::Show
	      || event->type() == QEvent::ShowToParent
	      || event->type() == QEvent::PolishRequest
	      || event->type() == QEvent::WindowActivate
	      || event->type() == QEvent::WindowChangeInternal
			// || event->type() == QEvent::DynamicPropertyChange
	))
	{
		QOpenGLWidget::event(event);
	}
}