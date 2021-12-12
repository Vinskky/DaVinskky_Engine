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
#include "JsonFile.h"

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

	Importer::Scene::Private::ProcessNode(scene, scene->mRootNode, gameObjects, gameObjects[0]);

}

bool Importer::Scene::Save(const char* name, std::vector<GameObject*> gameObjects)
{
	bool ret = false;

	JsonFile sceneJson;
	// We start setting gameobjects, including the object for the root scene:
	sceneJson.file["Game Objects"] = json::array(); // Start filling the array for it (as the slides explain)
	for (std::vector<GameObject*>::iterator goIt = gameObjects.begin() + 1; goIt != gameObjects.end(); goIt++)
	{
		json jsonGO;
		jsonGO["name"] = (*goIt)->GetName();
		const char* name = (*goIt)->GetName();
		jsonGO["active"] = (*goIt)->IsActive();
		jsonGO["uuid"] = (*goIt)->GetUUID();
		jsonGO["parentUUID"] = (*goIt)->GetParentUUID();

		// We start setting its components in another array:
		jsonGO["Components"] = json::array();
		for (std::vector<Component*>::iterator componentIt = (*goIt)->components.begin(); componentIt != (*goIt)->components.end(); componentIt++)
		{
			Component* component = (*componentIt);

			json jsonComp;
			jsonComp["active"] = (*componentIt)->IsActive();
			switch ((*componentIt)->GetType())
			{
			case COMPONENT_TYPE::NONE:
				jsonComp["type"] = "NONE";
				break;
			case COMPONENT_TYPE::TRANSFORM:
			{
				C_Transform transformComp = *(C_Transform*)component;
				jsonComp["type"] = "Transform";
				jsonComp["position"] = { transformComp.GetPosition().x, transformComp.GetPosition().y, transformComp.GetPosition().z };
				jsonComp["rotation"] = { transformComp.GetRotation().x, transformComp.GetRotation().y, transformComp.GetRotation().z, transformComp.GetRotation().w };
				jsonComp["scale"] = { transformComp.GetScale().x, transformComp.GetScale().y, transformComp.GetScale().z };
			}
			break;
			case COMPONENT_TYPE::MESH:
			{
				C_Mesh meshComp = *(C_Mesh*)component;
				jsonComp["type"] = "Mesh";
				jsonComp["path"] = meshComp.GetMeshPath();

				std::string path(MESHES_PATH + std::string(name) + ".DaVMesh");
				Importer::Mesh::Save(meshComp.GetMesh(), path.c_str());
			}
			break;
			case COMPONENT_TYPE::MATERIAL:
			{
				C_Material materialComp = *(C_Material*)component;
				jsonComp["type"] = "Material";
				jsonComp["color"] = { materialComp.GetMaterialColour().r, materialComp.GetMaterialColour().g, materialComp.GetMaterialColour().b, materialComp.GetMaterialColour().a };
				jsonComp["path"] = materialComp.GetTexturePath();
			}
			break;
			default:
				break;
			}
			jsonGO["Components"].push_back(jsonComp);
		}
		sceneJson.file["Game Objects"].push_back(jsonGO);
	}

	std::string path = SCENES_PATH + std::string(name) + ".json";
	ret = sceneJson.Save(path.c_str());

	return ret;
}

bool Importer::Scene::Load(const char* name, std::vector<GameObject*>& gameObjects)
{
	bool ret = false;

	JsonFile sceneJson;
	std::string path = SCENES_PATH + std::string(name) + ".json";
	sceneJson.Load(path.c_str());

	if (!sceneJson.file.is_null())
	{
		json jsonGameObjects = sceneJson.file["Game Objects"];

		for (auto goIt = jsonGameObjects.begin(); goIt != jsonGameObjects.end(); ++goIt)
		{
			// We store all values of the different keys in variables to then set the gameobject
			std::string name = (*goIt)["name"];
			bool active = (*goIt)["active"];
			UINT32 uuid = (*goIt)["uuid"];
			UINT32 parentUUID = (*goIt)["parentUUID"];

			GameObject* gameObj = new GameObject(uuid, active);
			gameObj->SetName(name.c_str());
			gameObj->SetParentUUID(parentUUID);

			json jsonComp = (*goIt)["Components"];
			for (auto componentIt = jsonComp.begin(); componentIt != jsonComp.end(); ++componentIt)
			{
				// We store all values of the different keys as we set the components
				bool active = (*componentIt)["active"];
				gameObj->transform->SetIsActive(active);

				std::string strType = (*componentIt)["type"];

				if (strType == "Transform")
				{
					float posX = (*componentIt)["position"][0];
					float posY = (*componentIt)["position"][1];
					float posZ = (*componentIt)["position"][2];
					gameObj->transform->SetPosition(posX, posY, posZ);
					float rotX = (*componentIt)["rotation"][0];
					float rotY = (*componentIt)["rotation"][1];
					float rotZ = (*componentIt)["rotation"][2];
					float rotW = (*componentIt)["rotation"][3];
					gameObj->transform->SetRotation(rotX, rotY, rotZ, rotW);
					float scaleX = (*componentIt)["scale"][0];
					float scaleY = (*componentIt)["scale"][1];
					float scaleZ = (*componentIt)["scale"][2];
					gameObj->transform->SetScale(scaleX, scaleY, scaleZ);
				}
				if (strType == "Mesh")
				{
					std::string path = (*componentIt)["path"];

					R_Mesh* rmesh = new R_Mesh();

					bool res = Importer::Mesh::Load(path.c_str(), rmesh);

					if (res)
					{
						C_Mesh* compMesh = (C_Mesh*)gameObj->CreateComponent(COMPONENT_TYPE::MESH);
						compMesh->SetMesh(rmesh);
						compMesh->SetMeshPath(path.c_str());
					}
				}
				if (strType == "Material")
				{
					R_Material* rmat = new R_Material();

					float r = (*componentIt)["color"][0];
					float g = (*componentIt)["color"][1];
					float b = (*componentIt)["color"][2];
					float a = (*componentIt)["color"][3];
					rmat->diffuseColor = { r,g, b, a };

					std::string path = (*componentIt)["path"];

					C_Material* compMaterial = (C_Material*)gameObj->CreateComponent(COMPONENT_TYPE::MATERIAL);
					compMaterial->SetMaterial(rmat);

					R_Texture* rtexture = new R_Texture();
					bool ret = Importer::Texture::Import(path.c_str(), rtexture);

					if (ret)
					{
						compMaterial->SetTexture(rtexture);
						compMaterial->SetTexturePath(path.c_str());
					}

				}
			}
			gameObjects.push_back(gameObj);
		}
		ret = true;
	}
	else
		ret = false;

	for (std::vector<GameObject*>::iterator goIt = gameObjects.begin(); goIt != gameObjects.end(); goIt++)
	{
		for (std::vector<GameObject*>::iterator childrengoIt = gameObjects.begin() + 1; childrengoIt != gameObjects.end(); childrengoIt++)
		{
			if ((*childrengoIt)->GetParentUUID() == (*goIt)->GetUUID())
			{
				(*goIt)->AddChild((*childrengoIt));
			}
		}
	}

	return ret;
}

void Importer::Scene::Private::ProcessNode(const aiScene* aiscene, const aiNode* node, std::vector<GameObject*>& gameObjects, GameObject* parent)
{
	GameObject* gameObj = new GameObject();
	gameObj->SetParent(parent);
	//Import Transform with dummies
	node = Private::ImportTransform(node, gameObj);
	Private::ImportMeshesAndMaterial(aiscene, node, gameObj);

	gameObj->SetName(node->mName.C_Str());
	LOG("ProcessNode node name: %s", gameObj->GetName());
	gameObjects.push_back(gameObj);

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		Importer::Scene::Private::ProcessNode(aiscene, node->mChildren[i], gameObjects, gameObj);
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
			ImportMesh(aimesh, gameObj, node->mName.C_Str());

			ImportMaterial(aimesh, aiscene, gameObj);
		}
	}
}

void Importer::Scene::Private::ImportMesh(const aiMesh* aimesh, GameObject* gameObj, const char* name)
{
	R_Mesh* rmesh = new R_Mesh();

	bool res = Importer::Mesh::Import(aimesh, rmesh);

	std::string path(MESHES_PATH + std::string(name) + ".DaVMesh");

	if (res)
	{
		C_Mesh* compMesh = (C_Mesh*)gameObj->CreateComponent(COMPONENT_TYPE::MESH);
		compMesh->SetMesh(rmesh);
		compMesh->SetMeshPath(path.c_str());
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

		std::string finalPath = ASSETS_TEXTURES_PATH;
		finalPath += textPath.C_Str();
		bool ret = Importer::Texture::Import(finalPath.c_str(), rtexture);
		
		if (ret)
		{
			compMaterial->SetTexture(rtexture);
			compMaterial->SetTexturePath(finalPath.c_str());
		}
	}
}

bool Importer::Scene::Private::IsDummyNode(const aiNode& node)
{
	return (strstr(node.mName.C_Str(), "_$AssimpFbx$_") != nullptr && node.mNumChildren == 1);
}
