#include "Application.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "R_Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"


C_Mesh::C_Mesh(GameObject* owner): Component(owner, COMPONENT_TYPE::MESH)
{
}

C_Mesh::~C_Mesh()
{
}

void C_Mesh::Enable()
{
}

void C_Mesh::Update()
{
	app->renderer3D->DrawMesh(this, this->GetOwner()->GetComponent<C_Material>());
}

void C_Mesh::Disable()
{
}

bool C_Mesh::CleanUp()
{
	RELEASE(rmesh);
	rmesh = nullptr;
	return true;
}

void C_Mesh::SetMesh(R_Mesh* rmesh)
{
	this->rmesh = rmesh;
}

R_Mesh* C_Mesh::GetMesh() const
{
	return rmesh;
}

bool C_Mesh::GetDrawNormals() const
{
	return drawNormals;
}

void C_Mesh::SetDrawNormals(bool debugDraw)
{
	drawNormals = debugDraw;
}

void C_Mesh::GetMeshData(uint& numVertices, uint& numNormals, uint& numTexCoords, uint& numIndices)
{
	numVertices = rmesh->mVertices.size();
	numNormals = rmesh->mNormals.size();
	numTexCoords = rmesh->mTextureCoords.size();
	numIndices = rmesh->mIndex.size();
}

void C_Mesh::SetMeshPath(const char* path)
{
	_path = path;
}

const char* C_Mesh::GetMeshPath() const
{
	return _path.c_str();
}