#include "I_Mesh.h"
#include "Globals.h"
#include "AssimpInc.h"
#include "R_Mesh.h"
#include <fstream>

bool Importer::Mesh::Import(const aiMesh* aimesh, R_Mesh* rmesh)
{
	bool ret = true;
	if (aimesh == nullptr || rmesh == nullptr)
		return false;

	Private::GetVertices(aimesh,rmesh);
	Private::GetNormals(aimesh,rmesh);
	Private::GetTextCoords(aimesh,rmesh);
	Private::GetIndices(aimesh,rmesh);

	rmesh->LoadBuffers();

	return ret;
}

bool Importer::Mesh::Save(const R_Mesh* rmesh, const char* path)
{
	bool ret = false;

	if (rmesh == nullptr || path == nullptr)
		return false;

	// Our header size is 4 (or NUM_HEADER_CATEGORIES) * sizeof(unsigned) = 16 bytes

	std::ofstream myMeshFile;
	myMeshFile.open(path, std::ios::in | std::ios::app | std::ios::binary);

	if (myMeshFile.is_open())
	{
		myMeshFile.write((char*)rmesh, NUM_HEADER_CATEGORIES * sizeof(unsigned));

		myMeshFile.write((char*)rmesh->mVertices.data(), rmesh->vertexSizeBytes);
		myMeshFile.write((char*)rmesh->mNormals.data(), rmesh->normalSizeBytes);
		myMeshFile.write((char*)rmesh->mTextureCoords.data(), rmesh->textCoordSizeBytes);
		myMeshFile.write((char*)rmesh->mIndex.data(), rmesh->indexSizeBytes);

		myMeshFile.close();
		ret = true;
	}
	else
		ret = false;

	return ret;
}

bool Importer::Mesh::Load(const char* path, R_Mesh* rmesh)
{
	bool ret = false;

	if (rmesh == nullptr || path == nullptr)
		return false;

	std::ifstream myMeshFile;
	myMeshFile.open(path, std::ios::binary);

	if (myMeshFile.is_open())
	{
		// Reads Header
		myMeshFile.read((char*)rmesh, NUM_HEADER_CATEGORIES * sizeof(unsigned));

		size_t bufferSize = rmesh->vertexSizeBytes / sizeof(float);
		rmesh->mVertices.resize(bufferSize);
		myMeshFile.read((char*)&rmesh->mVertices[0], rmesh->vertexSizeBytes);

		bufferSize = rmesh->normalSizeBytes / sizeof(float);
		rmesh->mNormals.resize(bufferSize);
		myMeshFile.read((char*)&rmesh->mNormals[0], rmesh->normalSizeBytes);

		bufferSize = rmesh->textCoordSizeBytes / sizeof(float);
		rmesh->mTextureCoords.resize(bufferSize);
		myMeshFile.read((char*)&rmesh->mTextureCoords[0], rmesh->textCoordSizeBytes);

		bufferSize = rmesh->indexSizeBytes / sizeof(uint);
		rmesh->mIndex.resize(bufferSize);
		myMeshFile.read((char*)&rmesh->mIndex[0], rmesh->indexSizeBytes);

		myMeshFile.close();
		ret = true;
	}
	else
		ret = false;

	rmesh->LoadBuffers();

	return ret;
}

void Importer::Mesh::Private::GetVertices(const aiMesh* aimesh, R_Mesh* rmesh)
{
	if (!aimesh->HasPositions())
	{
		LOG("ERROR: Imported mesh has no position vertices!!");
		return;
	}

	rmesh->vertexSizeBytes = aimesh->mNumVertices * sizeof(float) * 3;

	rmesh->mVertices.resize(aimesh->mNumVertices * 3);
	memcpy(&rmesh->mVertices[0], aimesh->mVertices, sizeof(float) * aimesh->mNumVertices * 3);

	LOG("New mesh with %d vertices", rmesh->mVertices.size());
}

void Importer::Mesh::Private::GetNormals(const aiMesh* aimesh, R_Mesh* rmesh)
{
	if (!aimesh->HasNormals())
	{
		LOG("ERROR: Imported mesh has no normals!!");
		return;
	}

	rmesh->normalSizeBytes = aimesh->mNumVertices * sizeof(float) * 3;

	rmesh->mNormals.resize(aimesh->mNumVertices * 3);
	memcpy(&rmesh->mNormals[0], aimesh->mNormals, sizeof(float) * aimesh->mNumVertices * 3);

	LOG("New mesh with %d vertices", rmesh->mNormals.size());
}

void Importer::Mesh::Private::GetTextCoords(const aiMesh* aimesh, R_Mesh* rmesh)
{
	if (!aimesh->HasTextureCoords(0))
	{
		LOG("ERROR: Imported mesh has no Texturecoords!");
		return;
	}

	rmesh->textCoordSizeBytes = aimesh->mNumVertices * sizeof(float) * 2;

	rmesh->mTextureCoords.resize(aimesh->mNumVertices * 2);
	for (uint i = 0; i < aimesh->mNumVertices; ++i)
	{
		rmesh->mTextureCoords[i * 2] = aimesh->mTextureCoords[0][i].x;
		rmesh->mTextureCoords[i * 2 + 1] = aimesh->mTextureCoords[0][i].y;
	}

	LOG("Correctly imported textures coordinates");
}

void Importer::Mesh::Private::GetIndices(const aiMesh* aimesh, R_Mesh* rmesh)
{
	if (!aimesh->HasFaces())
	{
		LOG("ERROR: Imported mesh has no Index!");
		return;
	}

	rmesh->indexSizeBytes = aimesh->mNumFaces * sizeof(uint) * 3;

	rmesh->mIndex.resize(aimesh->mNumFaces * 3);
	for (uint i = 0; i < aimesh->mNumFaces; ++i)
	{

		if (aimesh->mFaces[i].mNumIndices != 3)
		{
			LOG("WARNING, geometry face with != 3 indices!");
		}
		else
		{
			memcpy(&rmesh->mIndex[i * 3], aimesh->mFaces[i].mIndices, sizeof(uint) * 3);
		}
	}

	LOG("Imported Index: %d", rmesh->mIndex.size());
}