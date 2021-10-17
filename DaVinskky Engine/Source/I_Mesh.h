#ifndef _I_MESH_H_
#define _I_MESH_H_

#include "Globals.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"


#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void InitMesh();

	void CleanUp();

public:
	struct aiLogStream stream;

	std::vector<float> mVertices;
	std::vector<float> mNormals;
	std::vector<float> mTextureCoords;
	std::vector<uint> mIndex;

	uint mMaterialIndex;
};

#endif // !_I_MESH_H_

