#include "Application.h"
#include "ModuleFileSystem.h"
#include "R_Mesh.h"
#include "Globals.h"
#include "OpenGL.h"

R_Mesh::R_Mesh()
{
}

R_Mesh::~R_Mesh()
{
}

void R_Mesh::InitMesh()
{
	VBO = 0;
	IBO = 0;
	NBO = 0;
	TBO = 0;

	filePath = "Assets/Models/BakerHouse.fbx";
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	LOG("%s", stream.user);
}

bool R_Mesh::LoadMesh()
{
	bool ret = true;

	char* buffer = nullptr;
	uint sizeBuffer = app->fileSystem->Load(filePath, &buffer);

	if (sizeBuffer == 0)
	{
		LOG("load %s does not work properly", filePath);
		return false;
	}

	const aiScene* scene = aiImportFileFromMemory(buffer, sizeBuffer, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	if (scene != nullptr && scene->HasMeshes())
	{
		//aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", filePath);
		ret = false;
	}

	//copy vertices
	if (scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];
			
			ret = LoadVertex(mesh);
			ret = LoadNormals(mesh);
			ret = LoadTextCoords(mesh);
			ret = LoadIndex(mesh);

			LoadBuffers();
		}
	}
	else
	{
		LOG("Error copying vertices from scene %s", filePath);
		ret = false;
	}
	


	return ret;
}

bool R_Mesh::LoadVertex(const aiMesh* mesh)
{
	if (!mesh->HasPositions())
	{
		LOG("Error copying vertices from scene %s", filePath);
		return false;
	}

	mVertices.resize(mesh->mNumVertices * 3);
	memcpy(&mVertices[0], mesh->mVertices, sizeof(float) * mesh->mNumVertices * 3);
	LOG("New mesh with %d vertices", mVertices.size());

	return true;
}

bool R_Mesh::LoadIndex(const aiMesh* mesh)
{
	if (!mesh->HasFaces())
	{
		LOG("Error finding Index from scene %s", filePath);
		return false;
	}

	mIndex.resize(mesh->mNumFaces * 3);
	for (uint i = 0; i < mesh->mNumFaces; ++i)
	{
		
		if (mesh->mFaces[i].mNumIndices != 3)
		{
			LOG("WARNING, geometry face with != 3 indices!");
		}
		else
		{
			memcpy(&mIndex[i*3], mesh->mFaces[i].mIndices,sizeof(uint) * 3);
		}
	}
	LOG("Imported Index: %d", mIndex.size());
	return true;
}

bool R_Mesh::LoadTextCoords(const aiMesh* mesh)
{
	if (!mesh->HasTextureCoords(0))
	{
		LOG("Error finding TextCoords from scene %s", filePath);
		return false;
	}

	mTextureCoords.resize(mesh->mNumVertices * 2);
	for (uint i = 0; i < mesh->mNumVertices; ++i)
	{
		mTextureCoords[i * 2] = mesh->mTextureCoords[0][i].x;
		mTextureCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
	}

	LOG("Correctly imported textures coordinates");

	return true;
}

bool R_Mesh::LoadNormals(const aiMesh* mesh)
{
	if (!mesh->HasNormals())
	{
		LOG("Error finding Normals from scene %s", filePath);
		return false;
	}

	mNormals.resize(mesh->mNumVertices * 3);
	memcpy(&mNormals[0], mesh->mNormals, sizeof(float) * mesh->mNumVertices * 3);
	LOG("New mesh with %d normals", mNormals.size());

	return true;
}

void R_Mesh::CleanUp()
{
	aiDetachAllLogStreams();
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
