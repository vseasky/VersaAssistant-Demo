/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\VersaOpenGL\VersaOpenGLModel.h
 * @Description  : 
 */
#ifndef VERSAOPENGLMODEL_H
#define VERSAOPENGLMODEL_H

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "VersaScene.h"

class VersaOpenGLModel
{
public:
	VersaOpenGLModel(VersaScene *versaScene, bool transformToUnit = true);
	~VersaOpenGLModel();
	bool importer(QString filePath);

private:
	void processMaterial(VersaScene *versaScene, const aiMaterial *pMaterial);
	void processMesh(VersaScene *versaScene, const aiMesh *pMesh, int *mIndexOffsetMaterial);
	void processNode(VersaScene *versaScene,
	                 const aiScene *pScene,
	                 const aiNode *pNode,
	                 VersaNode *parentNode,
	                 VersaNode *newNode,
	                 int *mIndexOffsetMeshes);
	
	void transformToUnitCoordinates(VersaScene *versaScene, VersaNode *pNode);
	void checkObjectDimensions(VersaScene *versaScene,
	                           VersaNode *pNode,
	                           QMatrix4x4 transformation,
	                           QVector3D *minDimension,
	                           QVector3D *maxDimension);
	void updataTranslateMatrix(VersaNode *pNode);
	bool mTransformToUnit;
	// 一定不要使用这个释放
	VersaScene *pVersaScene = nullptr;
};

#endif // VERSAOPENGLMODEL_H
