#ifndef _I_SCENE_H_
#define _I_SCENE_H_

#include "Globals.h"
#include <vector>


struct aiScene;
struct aiMesh;
struct aiNode;
struct aiMaterial;

class R_Mesh;
class C_Material;
class GameObject;

namespace Importer
{
	namespace Scene
	{
		void Import(const char* path, std::vector<GameObject*>& gameObjects);

		bool Save(const char* name, std::vector<GameObject*> gameObjects);
		bool Load(const char* name, std::vector<GameObject*>& gameObjects);

		namespace Private
		{
			void ProcessNode(const aiScene* aiscene, const aiNode* node, std::vector<GameObject*>& gameObjects, GameObject* parent);

			const aiNode* ImportTransform(const aiNode* ainode, GameObject* gameObj);
			void ImportMeshesAndMaterial(const aiScene* aiscene, const aiNode* node, GameObject* gameObj);

			void ImportMesh(const aiMesh* aimesh, GameObject* gameObj, const char* name);
			void ImportMaterial(const aiMesh* aimesh, const aiScene* aiscene, GameObject* gameObj);
			void ImportTexture(const aiMaterial* aimaterial, C_Material* compMaterial);

			bool IsDummyNode(const aiNode& node);
		}
	}
}


#endif // !_I_SCENE_H_

