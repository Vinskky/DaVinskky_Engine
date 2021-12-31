#include "C_Material.h"
#include "R_Material.h"
#include "R_Texture.h"

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
	return true;
}

void C_Material::Save(json& jsonComp)
{
	jsonComp["type"] = "Material";
	jsonComp["color"] = { GetMaterialColour().r, GetMaterialColour().g, GetMaterialColour().b, GetMaterialColour().a };
	jsonComp["path"] = GetTexturePath();
}

void C_Material::Load(json& jsonComp)
{
	rmaterial = new R_Material();

	jsonComp.at("color")[0].get_to(rmaterial->diffuseColor.r);
	jsonComp.at("color")[1].get_to(rmaterial->diffuseColor.g);
	jsonComp.at("color")[2].get_to(rmaterial->diffuseColor.b);
	jsonComp.at("color")[3].get_to(rmaterial->diffuseColor.a);

	jsonComp.at("path").get_to(_path);

	rtexture = new R_Texture();
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

void C_Material::SetTexturePath(const char* path)
{
	_path = path;
}

const char* C_Material::GetTexturePath() const
{
	return _path.c_str();
}