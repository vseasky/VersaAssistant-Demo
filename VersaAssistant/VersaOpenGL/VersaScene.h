//
// Created by vSeasky on 2023/9/12.
//

#ifndef VERSA_ASSISTANT_VERSASCENE_H
#define VERSA_ASSISTANT_VERSASCENE_H

#include <QMatrix4x4>
#include <QSharedPointer>

struct VersaMaterial
{
	QString mName;
	QVector3D mAmbient;
	QVector3D mDiffuse;
	QVector3D mSpecular;
	float mShinines;
};

struct VersaLight
{
	QVector4D mPosition;
	QVector3D mIntensity;
};

struct VersaMesh
{
	QString mName;
	unsigned int mIndexCount;
	unsigned int mIndexOffset;
	QSharedPointer<VersaMaterial> material = nullptr;
};

enum VersaNodeAction
{
	noAction = 0, rotationAction, transformAction, moveAction, colorAction
};

struct VersaNode
{
	VersaNodeAction mNodeAction = noAction;
	QString mName;
	
	QMatrix4x4 mTransformMatrix;
	QMatrix4x4 mRelativeMatrix;//处理相对位置
	QMatrix4x4 mScaleMatrix;
	QMatrix4x4 mTranslateMatrix;
	QMatrix4x4 mRotationMatrix;
	
	QVector3D mRotation3DCoordinates;
	QVector3D mRotation3DAxisEnable;
	
	QVector3D mTranslate3D;
	QVector3D mRotation3DAngle;
	float mScale = 1;//默认不缩放
	
	
	QVector<QSharedPointer<VersaMesh> > mMeshes;
	QVector<VersaNode> mNodes;
};

struct VersaScene : public QObject
{
	QVector<float> mVertices;   // 顶点
	QVector<float> mNormals;    // 法线
	QVector<unsigned int> mIndices; // 指数
	QVector<float> mTangents;   // 切线
	QVector<float> mBitangents; // 双切线
	
	QVector<QVector<float>> mTextureUv; // 纹理 UV
	QVector<unsigned int> mTextureUvComponents; // 纹理 UV 组件
	QVector<QSharedPointer<VersaMesh>> mMeshes; // 网格
	QVector<QSharedPointer<VersaMaterial> > mMaterials; // 材料
	QSharedPointer<VersaNode> mRootNode;
	
	VersaNode rootNode;
};

#endif //VERSA_ASSISTANT_VSERASCENE_H
