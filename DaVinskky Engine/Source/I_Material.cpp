#include "Globals.h"
#include "AssimpInc.h"
#include "I_Material.h"
#include "R_Material.h"

bool Importer::Material::Import(const aiMaterial* aimat, R_Material* rmat)
{
	if (aimat == nullptr)
	{
		LOG("ERROR importing Materials, aiMaterial is null!!");
		return false;
	}
	if (rmat == nullptr)
	{
		LOG("ERROR importing Materials, R_Material is null!!");
		return false;
	}

	aiColor4D color;
	if (aimat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		rmat->diffuseColor = Color(color.r, color.g, color.b, color.a);
	}

	return true;
}
