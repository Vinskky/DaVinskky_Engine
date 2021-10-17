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

	bool LoadMesh();

	bool LoadVertex(const aiMesh* mesh);
	bool LoadIndex(const aiMesh* mesh);
	bool LoadTextCoords(const aiMesh* mesh);
	bool LoadNormals(const aiMesh* mesh);

	void CleanUp();

	//LoadBuffer to been able to draw
	void LoadBuffers();

public:
	struct aiLogStream stream;

	std::vector<float> mVertices;
	std::vector<float> mNormals;
	std::vector<float> mTextureCoords;
	std::vector<uint> mIndex;

	uint mMaterialIndex;

	char* filePath = nullptr;

	//OpenGL buffers parameters
	uint VBO;						//Vertix Buffer Object	
	uint NBO;						//Normal Buffer Object
	uint TBO;						//Texture Buffer Object
	uint IBO;						//Index Buffer Object

};

#endif // !_I_MESH_H_

