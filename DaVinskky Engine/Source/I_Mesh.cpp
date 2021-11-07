#include "I_Mesh.h"
#include "Globals.h"
#include "AssimpInc.h"
#include "R_Mesh.h"

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

void Importer::Mesh::Private::GetVertices(const aiMesh* aimesh, R_Mesh* rmesh)
{
	if (!aimesh->HasPositions())
	{
		LOG("ERROR: Imported mesh has no position vertices!!");
		return;
	}

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
