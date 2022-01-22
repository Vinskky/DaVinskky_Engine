#ifndef _R_MESH_H_
#define _R_MESH_H_

#include "Globals.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "External/MathGeoLib/include/Geometry/AABB.h"

#include <vector>

class R_Mesh
{
public:
	R_Mesh();
	~R_Mesh();

	void CleanUp();

	// LoadBuffer to been able to draw
	void LoadBuffers();

	void SetAABB();

public:
	
	unsigned vertexSizeBytes;
	unsigned indexSizeBytes;
	unsigned normalSizeBytes;
	unsigned textCoordSizeBytes;

	std::vector<float> mVertices;
	std::vector<uint> mIndex;
	std::vector<float> mNormals;
	std::vector<float> mTextureCoords;

	// OpenGL buffers parameters
	uint VAO;						// Vertex Array Object
	uint VBO;						// Vertex Buffer Object
	uint IBO;						// Index Buffer Object
	uint NBO;						// Normal Buffer Object
	uint TBO;						// Texture Buffer Object

	// AABB
	AABB aabb;
};

#endif // !_R_MESH_H_