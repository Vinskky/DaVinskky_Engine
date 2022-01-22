#include "C_Material.h"
#include "R_Material.h"
#include "R_Texture.h"
#include "R_Shader.h"

#include "External/mmgr/include/mmgr.h"

using json = nlohmann::ordered_json;

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

bool C_Material::CleanUp()
{
	RELEASE(rmaterial);
	rmaterial = nullptr;
	RELEASE(rtexture);
	rtexture = nullptr;
	RELEASE(rshader);
	rshader = nullptr;

	return true;
}

void C_Material::Save(json& jsonComp)
{
	jsonComp["type"] = "Material";
	jsonComp["color"] = { GetMaterialColour().r, GetMaterialColour().g, GetMaterialColour().b, GetMaterialColour().a };
	jsonComp["texture_path"] = GetTexturePath();
	jsonComp["shader_path"] = GetShaderPath();
}

void C_Material::Load(json& jsonComp)
{
	rmaterial = new R_Material();

	jsonComp.at("color")[0].get_to(rmaterial->diffuseColor.r);
	jsonComp.at("color")[1].get_to(rmaterial->diffuseColor.g);
	jsonComp.at("color")[2].get_to(rmaterial->diffuseColor.b);
	jsonComp.at("color")[3].get_to(rmaterial->diffuseColor.a);

	jsonComp.at("texture_path").get_to(texturePath);
	jsonComp.at("shader_path").get_to(shaderPath);

	rtexture = new R_Texture();
	rshader = new R_Shader();
}

void C_Material::SetMaterial(R_Material* rmaterial)
{
	this->rmaterial = rmaterial;
}

void C_Material::SetTexture(R_Texture* rtexture)
{
	this->rtexture = rtexture;
}

void C_Material::SetShader(R_Shader* rshader)
{
	this->rshader = rshader;
}

R_Texture* C_Material::GetTexture() const
{
	return rtexture;
}

uint C_Material::GetTextureID()
{
	if (rtexture != nullptr)
	{
		return rtexture->GetTextureID();
	}
	return 0;
}

const char* C_Material::GetTexturePath() const
{
	return texturePath.c_str();
}

void C_Material::SetTexturePath(const char* path)
{
	texturePath = path;
}

R_Shader* C_Material::GetShader() const
{
	return rshader;
}

const char* C_Material::GetShaderPath() const
{
	return shaderPath.c_str();
}

void C_Material::SetShaderPath(const char* path)
{
	shaderPath = path;
}

void C_Material::SetDefaultTexture(bool text)
{
	debugTextEnabled = text;
}

bool C_Material::UseDefaultTexture()
{
	return debugTextEnabled;
}

Color C_Material::GetMaterialColour()
{
	if (rmaterial != nullptr)
	{
		return rmaterial->diffuseColor;
	}
	return Color();
}