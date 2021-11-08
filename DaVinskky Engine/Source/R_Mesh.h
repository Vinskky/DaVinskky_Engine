#ifndef _R_MESH_H_
#define _R_MESH_H_

#include "Globals.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"

#include <vector>

class R_Mesh
{
public:
	R_Mesh();
	~R_Mesh();

	void CleanUp();

	//LoadBuffer to been able to draw
	void LoadBuffers();

public:
	
	std::vector<float> mVertices;
	std::vector<float> mNormals;
	std::vector<float> mTextureCoords;
	std::vector<uint> mIndex;

	//OpenGL buffers parameters
	uint VBO;						//Vertix Buffer Object	
	uint NBO;						//Normal Buffer Object
	uint TBO;						//Texture Buffer Object
	uint IBO;						//Index Buffer Object

};

#endif // !_I_MESH_H_

