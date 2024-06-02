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
