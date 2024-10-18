/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaOpenGL\VersaOpenGLModel.cpp
 * @Description  : 
 */
#include <QFile>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "VersaOpenGLModel.h"
#include <QDebug>

/*!
 * 构造函数
 * @param versaScene
 * @param transformToUnit
 */
VersaOpenGLModel::VersaOpenGLModel(VersaScene *versaScene, bool transformToUnit) :
		pVersaScene(versaScene),
		mTransformToUnit(transformToUnit)
{
	pVersaScene->mRootNode.reset(&pVersaScene->rootNode);
	pVersaScene->rootNode.mName = "VersaRobot";
}

VersaOpenGLModel::~VersaOpenGLModel()
{
	// 不能释放这个
	pVersaScene = nullptr;
}

/*!
 * 加载模型,支持添加多个模型
 * @param filePath
 * @param pathType
 * @return
 */
bool VersaOpenGLModel::importer(QString filePath)
{
	if (pVersaScene == nullptr)
	{
		return false;
	}
	Assimp::Importer AssimpImporter;
	// 加载模型
	const aiScene *pScene = AssimpImporter.ReadFile(filePath.toStdString(),
	                                                aiProcess_GenSmoothNormals |
	                                                aiProcess_CalcTangentSpace |
	                                                aiProcess_Triangulate |
	                                                aiProcess_JoinIdenticalVertices |
	                                                aiProcess_SortByPType
	);
	
	if (!pScene)
	{
		qDebug() << "Error loading file: (assimp:) " << AssimpImporter.GetErrorString();
		return false;
	}
	
	int mIndexMeshes = 0, mIndexMaterial = 0;
	mIndexMaterial = pVersaScene->mMaterials.size();
	mIndexMeshes = pVersaScene->mMeshes.size();
	
	// 获取材质
	if (pScene->HasMaterials())
	{
		for (unsigned int imateral = 0; imateral < pScene->mNumMaterials; ++imateral)
		{
			// 追加到 mMaterials
			processMaterial(pVersaScene, pScene->mMaterials[imateral]);
		}
	}
	else
	{
		qDebug() << "Error: No materal found";
		return false;
	}
	
	// 获取网格
	if (pScene->HasMeshes())
	{
		for (unsigned int imesh = 0; imesh < pScene->mNumMeshes; ++imesh)
		{
			// 追加到 mMeshes
			processMesh(pVersaScene, pScene->mMeshes[imesh], &mIndexMaterial);
		}
	}
	else
	{
		qDebug() << "Error: No meshe found";
		return false;
	}
	
	// 有灯
	if (pScene->HasLights())
	{
		qDebug() << "Has Lights";
	}
	
	if (pScene->mRootNode != NULL)
	{
		// 流程节点
		VersaNode mCurrentNode;
		processNode(pVersaScene, pScene, pScene->mRootNode, 0, &mCurrentNode, &mIndexMeshes);
		pVersaScene->rootNode.mNodes.push_back(mCurrentNode);
		// m 根节点
	}
	else
	{
		qDebug() << "Error loading model";
		return false;
	}
	
	if (mTransformToUnit)
	{
		// 使用 mRelativeMatrix 进行转换，其它矩阵此时应该为单位矩阵，转换为单位矩阵
		transformToUnitCoordinates(pVersaScene, &pVersaScene->rootNode.mNodes.back());
	}
	
	return true;
}

/*!
 * 处理工艺材质
 * @param versaScene
 * @param pMaterial
 */
void VersaOpenGLModel::processMaterial(VersaScene *versaScene, const aiMaterial *pMaterial)
{
	QSharedPointer<VersaMaterial> material(new VersaMaterial);
	aiString mName;
	pMaterial->Get(AI_MATKEY_NAME, mName);
	if (mName.length > 0)
	{
		material->mName = mName.C_Str();
	}
	int shadingModel;
	pMaterial->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
	if (shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud)
	{
		qDebug() << "This mesh's shading model is not implemented in this loader, setting to default pMaterial";
		material->mName = "DefaultMaterial";
	}
	else
	{
		aiColor3D dif(0.f, 0.f, 0.f);
		aiColor3D amb(0.f, 0.f, 0.f);
		aiColor3D spec(0.f, 0.f, 0.f);
		float shine = 0.0;
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, amb);    // 颜色环境
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, dif);    // 色彩漫反射
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, spec);  // 颜色镜面
		pMaterial->Get(AI_MATKEY_SHININESS, shine);      // 匹配闪亮度
		
		material->mAmbient = QVector3D(amb.r, amb.g, amb.b);
		material->mDiffuse = QVector3D(dif.r, dif.g, dif.b);
		material->mSpecular = QVector3D(spec.r, spec.g, spec.b);
		material->mShinines = shine;
		
		material->mAmbient *= .2f;
		if (material->mShinines == 0.0)
		{
			material->mShinines = 30;
		}
	}
	
	versaScene->mMaterials.push_back(material);
	return;
}

/*!
 * 处理网格信息
 * @param versaScene
 * @param pMesh
 * @param mIndexOffsetMaterial
 */
void VersaOpenGLModel::processMesh(VersaScene *versaScene, const aiMesh *pMesh, int *mIndexOffsetMaterial)
{
	// 创建一个mesh
	QSharedPointer<VersaMesh> newMesh(new VersaMesh);
	newMesh->mName = pMesh->mName.length != 0 ? pMesh->mName.C_Str() : "";
	// 标定此mesh在所有mesh中的偏移
	newMesh->mIndexOffset = versaScene->mIndices.size();
	// 添加新mesh前的大小
	unsigned int indexCountBefore = versaScene->mIndices.size();
	int vertindexoffset = versaScene->mVertices.size() / 3;
	// 遍历该网格中的顶点
	for (uint ii = 0; ii < pMesh->mNumVertices; ++ii)
	{
		// 获取顶点
		if (pMesh->mNumVertices > 0)
		{
			versaScene->mVertices.push_back(pMesh->mVertices[ii].x);
			versaScene->mVertices.push_back(pMesh->mVertices[ii].y);
			versaScene->mVertices.push_back(pMesh->mVertices[ii].z);
		}
		// 获取法线
		if (pMesh->HasNormals())
		{
			versaScene->mNormals.push_back(pMesh->mNormals[ii].x);
			versaScene->mNormals.push_back(pMesh->mNormals[ii].y);
			versaScene->mNormals.push_back(pMesh->mNormals[ii].z);
		}
		
		// 获取切线和双切线
		if (pMesh->HasTangentsAndBitangents())
		{
			versaScene->mTangents.push_back(pMesh->mTangents[ii].x);
			versaScene->mTangents.push_back(pMesh->mTangents[ii].y);
			versaScene->mTangents.push_back(pMesh->mTangents[ii].z);
			
			versaScene->mTangents.push_back(pMesh->mBitangents[ii].x);
			versaScene->mTangents.push_back(pMesh->mBitangents[ii].y);
			versaScene->mTangents.push_back(pMesh->mBitangents[ii].z);
		}
	}
	// 获取纹理坐标
	if (pMesh->GetNumUVChannels() > 0)
	{
		// 注意，假设该模型中的所有网格具有相同数量的 uv 通道
		if ((unsigned int) versaScene->mTextureUv.size() < pMesh->GetNumUVChannels())
		{
			versaScene->mTextureUv.resize(pMesh->GetNumUVChannels());
			versaScene->mTextureUvComponents.resize(pMesh->GetNumUVChannels());
		}
		for (unsigned int ich = 0; ich < pMesh->GetNumUVChannels(); ++ich)
		{
			versaScene->mTextureUvComponents[ich] = pMesh->mNumUVComponents[ich];
			
			for (uint iind = 0; iind < pMesh->mNumVertices; ++iind)
			{
				versaScene->mTextureUv[ich].push_back(pMesh->mTextureCoords[ich][iind].x);
				if (pMesh->mNumUVComponents[ich] > 1)
				{
					versaScene->mTextureUv[ich].push_back(pMesh->mTextureCoords[ich][iind].y);
					if (pMesh->mNumUVComponents[ich] > 2)
					{
						versaScene->mTextureUv[ich].push_back(pMesh->mTextureCoords[ich][iind].z);
					}
				}
			}
		}
	}
	
	// 获取 pMesh 索引
	for (uint t = 0; t < pMesh->mNumFaces; ++t)
	{
		aiFace *face = &pMesh->mFaces[t];
		if (face->mNumIndices != 3)
		{
			qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive." << face->mNumIndices;
			continue;
		}
		// 添加新的索引，指向新 mesh 的顶点
		versaScene->mIndices.push_back(face->mIndices[0] + vertindexoffset);
		versaScene->mIndices.push_back(face->mIndices[1] + vertindexoffset);
		versaScene->mIndices.push_back(face->mIndices[2] + vertindexoffset);
	}
	
	// newMesh 的索引大小
	newMesh->mIndexCount = versaScene->mIndices.size() - indexCountBefore;
	// mMaterials 是所有材质的合集，而此处是使用指针引用的的某一个材质
	newMesh->material = versaScene->mMaterials.at(pMesh->mMaterialIndex + *mIndexOffsetMaterial);
	
	versaScene->mMeshes.push_back(newMesh);
	return;
}

/*!
 * 解析节点，具有父子关系
 * @param versaScene
 * @param pScene
 * @param pNode
 * @param parentNode
 * @param newNode
 * @param mIndexOffsetMeshes
 */
void VersaOpenGLModel::processNode(VersaScene *versaScene,
                                   const aiScene *pScene,
                                   const aiNode *pNode,
                                   VersaNode *parentNode,
                                   VersaNode *newNode,
                                   int *mIndexOffsetMeshes)
{
	QVector3D initQVector3D(0, 0, 0);
	newNode->mName = pNode->mName.length != 0 ? pNode->mName.C_Str() : "";
	// 获取位置信息
	newNode->mRelativeMatrix = QMatrix4x4(pNode->mTransformation[0]);
	newNode->mScaleMatrix.setToIdentity();
	newNode->mTranslateMatrix.setToIdentity();
	newNode->mRotationMatrix.setToIdentity();
	newNode->mRotation3DCoordinates = initQVector3D;
	newNode->mRotation3DAxisEnable = initQVector3D;
	newNode->mTranslate3D = initQVector3D;
	newNode->mRotation3DAngle = initQVector3D;
	updataTranslateMatrix(newNode);
	// 获取 Node 的 Mesh
	newNode->mMeshes.resize(pNode->mNumMeshes);
	for (uint imesh = 0; imesh < pNode->mNumMeshes; ++imesh)
	{
		// 支持添加多个 模型
		QSharedPointer<VersaMesh> mesh = versaScene->mMeshes[*mIndexOffsetMeshes + pNode->mMeshes[imesh]];
		newNode->mMeshes[imesh] = mesh;
	}
	
	// 递归获取子节点
	for (uint ich = 0; ich < pNode->mNumChildren; ++ich)
	{
		newNode->mNodes.push_back(VersaNode());
		processNode(versaScene, pScene,
		            pNode->mChildren[ich], parentNode,
		            &newNode->mNodes[ich], mIndexOffsetMeshes);
		
	}
}

/*!
 * 转换为单位坐标,缩放并且平移
 * @param versaScene
 * @param pNode
 */
void VersaOpenGLModel::transformToUnitCoordinates(VersaScene *versaScene, VersaNode *pNode)
{
	// 这会将模型转换为单位坐标，因此任何尺寸或形状的模型都适合屏幕
	double amin = std::numeric_limits<double>::max();
	double amax = std::numeric_limits<double>::min();
	QVector3D minDimension(amin, amin, amin);
	QVector3D maxDimension(amax, amax, amax);
	
	minDimension = QVector3D(amin, amin, amin);
	maxDimension = QVector3D(amax, amax, amax);
	// 获取模型的最小和最大 x,y,z 值
	checkObjectDimensions(versaScene, pNode, QMatrix4x4(), &minDimension, &maxDimension);
	
	// 计算居中并适合屏幕所需的比例和平移
	float dist = qMax((maxDimension.x() - minDimension.x()),
	                  qMax((maxDimension.y() - minDimension.y()), (maxDimension.z() - minDimension.z())));
	
	// 缩放比例
	float scalingRatio = 0.9 / dist;
	QVector3D center = (maxDimension - minDimension) / 2;
	QVector3D trans = -(maxDimension - center);
	// // 将缩放和平移应用于矩阵
	QMatrix4x4 transformation;
	transformation.setToIdentity();           // 将其设置为单位矩阵
	transformation.scale(scalingRatio); // 将矩阵沿着x、y和z轴进行缩放
	transformation.translate(trans);    // 将矩阵沿着x、y和z轴进行平移
	
	pNode->mRelativeMatrix = transformation * pNode->mRelativeMatrix;
	updataTranslateMatrix(pNode);
}

/*!
 * 获取对象最大和最小坐标
 * @param versaScene
 * @param pNode
 * @param transformation
 * @param minDimension
 * @param maxDimension
 */
void VersaOpenGLModel::checkObjectDimensions(VersaScene *versaScene,
                                             VersaNode *pNode,
                                             QMatrix4x4 transformation,
                                             QVector3D *minDimension,
                                             QVector3D *maxDimension)
{
	// 需要保证其它矩阵目前还是单位矩阵
	transformation *= pNode->mRelativeMatrix;
	for (int ii = 0; ii < pNode->mMeshes.size(); ++ii)
	{
		// 开始
		int start = pNode->mMeshes[ii]->mIndexOffset;
		int end = start + pNode->mMeshes[ii]->mIndexCount;
		for (int ii = start; ii < end; ++ii)
		{
			int ind = versaScene->mIndices[ii] * 3;
			QVector4D vec(versaScene->mVertices[ind], versaScene->mVertices[ind + 1], versaScene->mVertices[ind + 2],
			              1.0);
			vec = transformation * vec;
			
			{
				if (vec.x() < minDimension->x())
				{
					minDimension->setX(vec.x());
				}
				if (vec.y() < minDimension->y())
				{
					minDimension->setY(vec.y());
				}
				if (vec.z() < minDimension->z())
				{
					minDimension->setZ(vec.z());
				}
				if (vec.x() > maxDimension->x())
				{
					maxDimension->setX(vec.x());
				}
				if (vec.y() > maxDimension->y())
				{
					maxDimension->setY(vec.y());
				}
				if (vec.z() > maxDimension->z())
				{
					maxDimension->setZ(vec.z());
				}
			}
		}
	}
	// 递归检查子节点
	for (int ii = 0; ii < pNode->mNodes.size(); ++ii)
	{
		checkObjectDimensions(versaScene, &(pNode->mNodes[ii]), transformation, minDimension, maxDimension);
	}
}

/*!
 * 更新变换矩阵信息
 * @param pNode
 */
void VersaOpenGLModel::updataTranslateMatrix(VersaNode *pNode)
{
	pNode->mTranslateMatrix.setToIdentity();
	pNode->mTranslateMatrix.translate(pNode->mTranslate3D);
	
	pNode->mRotationMatrix.setToIdentity();
	pNode->mRotationMatrix.translate(pNode->mRotation3DCoordinates);
	QQuaternion pitch = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), pNode->mRotation3DAngle.x());
	QQuaternion yaw = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), pNode->mRotation3DAngle.y());
	QQuaternion roll = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), pNode->mRotation3DAngle.z());
	QQuaternion rotation = yaw * pitch * roll;
	pNode->mRotationMatrix.translate(-pNode->mRotation3DCoordinates);
	pNode->mRotationMatrix.rotate(rotation);
	
	pNode->mScaleMatrix.setToIdentity();
	pNode->mScaleMatrix.scale(pNode->mScale);
	
	pNode->mTransformMatrix =
			pNode->mTranslateMatrix *
			pNode->mRotationMatrix *
			pNode->mScaleMatrix *
			pNode->mRelativeMatrix;
}