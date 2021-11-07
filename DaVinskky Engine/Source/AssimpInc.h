#ifndef _ASSIMP_INC_H_
#define _ASSIMP_INC_H_

#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"
		
#include "External/Assimp/include/mesh.h"
#include "External/Assimp/include/material.h"
#include "External/Assimp/include/texture.h"
#include "External/Assimp/include/matrix4x4.h"
#include "External/Assimp/include/vector3.h"
#include "External/Assimp/include/quaternion.h"

struct aiTransform
{
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;
};

#endif // !_ASSIMP_H_

