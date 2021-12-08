#include "Application.h"
#include "ModuleFileSystem.h"
#include "R_Mesh.h"
#include "Globals.h"
#include "OpenGL.h"

R_Mesh::R_Mesh()
{
	VBO = 0;
	IBO = 0;
	NBO = 0;
	TBO = 0;

	vertexSizeBytes = 0;
	normalSizeBytes = 0;
	indexSizeBytes = 0;
	textCoordSizeBytes = 0;
}

R_Mesh::~R_Mesh()
{
}


void R_Mesh::CleanUp()
{
	// Deleting buffers
	glDeleteBuffers(1, (GLuint*)&TBO);
	glDeleteBuffers(1, (GLuint*)&VBO);
	glDeleteBuffers(1, (GLuint*)&NBO);
	glDeleteBuffers(1, (GLuint*)&IBO);

	//Clear all vectors
	mVertices.clear();
	mNormals.clear();
	mTextureCoords.clear();
	mIndex.clear();

}

void R_Mesh::LoadBuffers()
{
	if (!mVertices.empty())
	{
		glGenBuffers(1, (GLuint*)&VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW);
	}

	if (!mNormals.empty())
	{
		glGenBuffers(1, (GLuint*)&NBO);
		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNormals.size(), &mNormals[0], GL_STATIC_DRAW);
	}

	if (!mTextureCoords.empty())
	{
		glGenBuffers(1, (GLuint*)&TBO);
		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mTextureCoords.size(), &mTextureCoords[0], GL_STATIC_DRAW);
	}

	if (!mIndex.empty())
	{
		glGenBuffers(1, (GLuint*)&IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mIndex.size(), &mIndex[0], GL_STATIC_DRAW);
	}


}
