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
}

void C_Mesh::Disable()
{
}

void C_Mesh::SetMesh(R_Mesh* rmesh)
{

}
