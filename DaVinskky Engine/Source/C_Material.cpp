#include "C_Material.h"

C_Material::C_Material(GameObject* owner): Component(owner,COMPONENT_TYPE::MATERIAL)
{
}

C_Material::~C_Material()
{
}

void C_Material::Enable()
{
}

void C_Material::Update()
{
}

void C_Material::Disable()
{
}

void C_Material::SetMaterial(R_Material* rmaterial)
{
	this->rmaterial = rmaterial;
}

void C_Material::SetTexture(R_Texture* rtexture)
{
	this->rtexture = rtexture;
}
