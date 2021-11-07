#include "I_Scene.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "AssimpInc.h"
#include "MathGeoInc.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "I_Mesh.h"
#include "I_Material.h"
#include "I_Texture.h"
#include "R_Mesh.h"
#include "R_Material.h"
#include "R_Texture.h"

#include <string>

void Importer::Scene::Import(const char* path, std::vector<GameObject*>& gameObjects)
{
	if (path == nullptr)
		return;

	char* buffer = nullptr;
	uint read = app->fileSystem->Load(path, &buffer);

	if (read == 0)
		return;

	if (buffer == nullptr)
		return;

	const aiScene* scene = aiImportFileFromMemory((const char*)buffer, read, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return;

	Importer::Scene::Private::ProcessNode(scene, scene->mRootNode, gameObjects);

}

void Importer::Scene::Private::ProcessNode(const aiScene* aiscene, const aiNode* node, std::vector<GameObject*>& gameObjects)
{
	GameObject* gameObj = new GameObject();

	//Import Transform with dummies
	node = Private::ImportTransform(node, gameObj);
	Private::ImportMeshesAndMaterial(aiscene, node, gameObj);

	gameObj->SetName(node->mName.C_Str());
	LOG("ProcessNode node name: %s", gameObj->GetName());
	gameObjects.push_back(gameObj);

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		Importer::Scene::Private::ProcessNode(aiscene, node->mChildren[i], gameObjects);
	}
}

const aiNode* Importer::Scene::Private::ImportTransform(const aiNode* ainode, GameObject* gameObj)
{
	aiTransform aiT;
	mathTransform maT;

	ainode->mTransformation.Decompose(aiT.scale, aiT.rotation, aiT.position);

	maT.position = { aiT.position.x,aiT.position.y, aiT.position.z };
	maT.rotation = { aiT.rotation.x,aiT.rotation.y, aiT.rotation.z, aiT.rotation.w };
	maT.scale = { aiT.scale.x,aiT.scale.y, aiT.scale.z };

	while (IsDummyNode(*ainode))
	{
		//If node is dummy we add this transform to next dummy until arrives to a node that's not a dummy
		ainode = ainode->mChildren[0];

		ainode->mTransformation.Decompose(aiT.scale, aiT.rotation, aiT.position);

		mathTransform dummy;
		dummy.position = { aiT.position.x,aiT.position.y, aiT.position.z };
		dummy.rotation = { aiT.rotation.x,aiT.rotation.y, aiT.rotation.z, aiT.rotation.w };
		dummy.scale = { aiT.scale.x,aiT.scale.y, aiT.scale.z };

		maT.position += dummy.position;
		maT.rotation = maT.rotation * dummy.rotation;
		maT.scale = { maT.scale.x * aiT.scale.x,maT.scale.y * aiT.scale.y ,maT.scale.z * aiT.scale.z };
	}
	
	gameObj->transform->SetPosition(maT.position.x, maT.position.y, maT.position.z);
	gameObj->transform->SetRotation(maT.rotation.x, maT.rotation.y, maT.rotation.z, maT.rotation.w);
	gameObj->transform->SetScale(maT.position.x, maT.position.y, maT.position.z);


	return ainode;
}

void Importer::Scene::Private::ImportMeshesAndMaterial(const aiScene* aiscene, const aiNode* node, GameObject* gameObj)
{
	if (aiscene == nullptr || node == nullptr)
		return;

	if (!aiscene->HasMeshes())
	{
		return;
	}

	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* aimesh = aiscene->mMeshes[node->mMeshes[i]];

		if (aimesh != nullptr && aimesh->HasFaces())
		{
			ImportMesh(aimesh, gameObj);

			ImportMaterial(aimesh, aiscene, gameObj);
		}
	}
}

void Importer::Scene::Private::ImportMesh(const aiMesh* aimesh, GameObject* gameObj)
{
	R_Mesh* rmesh = new R_Mesh();
	bool res = Importer::Mesh::Import(aimesh, rmesh);

	if (res)
	{
		C_Mesh* compMesh = (C_Mesh*)gameObj->CreateComponent(COMPONENT_TYPE::MESH);
		compMesh->SetMesh(rmesh);
	}
}

void Importer::Scene::Private::ImportMaterial(const aiMesh* aimesh, const aiScene* aiscene, GameObject* gameObj)
{
	if (aimesh->mMaterialIndex >= 0)
	{
		R_Material* rmat = new R_Material();
		aiMaterial* aimat = aiscene->mMaterials[aimesh->mMaterialIndex];
		bool res = Importer::Material::Import(aimat, rmat);
		if (res)
		{
			C_Material* compMaterial = (C_Material*)gameObj->CreateComponent(COMPONENT_TYPE::MATERIAL);
			compMaterial->SetMaterial(rmat);

			//Componet Material Has materials and textures inside.
			ImportTexture(aimat, compMaterial);
		}
	}
}

void Importer::Scene::Private::ImportTexture(const aiMaterial* aimaterial, C_Material* compMaterial)
{
	aiString textPath;
	if (aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textPath) == AI_SUCCESS)
	{
		R_Texture* rtexture = new R_Texture();

		std::string finalPath = ASSET_TEXTURES_PATH;
		finalPath += textPath.C_Str();
		bool ret = Importer::Texture::Import(finalPath.c_str(), rtexture);
		
		if (ret)
		{
			compMaterial->SetTexture(rtexture);
		}
	}

}



bool Importer::Scene::Private::IsDummyNode(const aiNode& node)
{
	return (strstr(node.mName.C_Str(), "_$AssimpFbx$_") != nullptr && node.mNumChildren == 1);
}
