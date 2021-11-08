#include "C_Material.h"
#include "R_Material.h"
#include "R_Texture.h"

C_Material::C_Material(GameObject* owner, bool debugTextEnabled): Component(owner,COMPONENT_TYPE::MATERIAL)
{
	this->debugTextEnabled = debugTextEnabled;
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


R_Texture* C_Material::GetTexture() const
{
	return rtexture;
}

bool C_Material::UseDefaultTexture()
{
	return debugTextEnabled;
}

void C_Material::SetDefaultTexture(bool text)
{
	debugTextEnabled = text;
}

Color C_Material::GetMaterialColour()
{
	if (rmaterial != nullptr)
	{
		return rmaterial->diffuseColor;
	}
	return Color();
}

uint C_Material::GetTextureID()
{
	if (rtexture != nullptr)
	{
		return rtexture->GetTextureID();
	}
	
	return 0;
}
