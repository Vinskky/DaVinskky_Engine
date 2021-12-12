#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <string>
#include <vector>
#include "Component.h"
#include "External/MathGeoLib/include/Geometry/AABB.h"
#include "External/MathGeoLib/include/Geometry/OBB.h"

class C_Transform;

class GameObject
{
public:
	GameObject(bool active = true);
	GameObject(UINT32 uuid, bool active = true);
	~GameObject();

	void Update();

	//create component
	Component* CreateComponent(COMPONENT_TYPE type);

public:

	template<typename T>
	T* GetComponent()const
	{
		COMPONENT_TYPE type = T::GetType();

		if (type == COMPONENT_TYPE::TRANSFORM)
			return (T*)transform;

		for (uint i = 0; i < components.size(); ++i)
		{
			if (components[i]->GetType() == type)
			{
				return (T*)components[i];
			}
		}
		return nullptr;
	}

	template <typename T>
	bool GetComponents(std::vector<T*>& componentType)
	{
		COMPONENT_TYPE type = T::GetType();
		for (uint i = 0; i < components.size(); ++i)
		{
			if (components[i]->GetType() == type)
			{
				componentType.push_back((T*)components[i]);
			}
		}

		return  (componentType.empty()) ? false : true;
	}

	void SetName(const char* str);
	const char* GetName()const;

	UINT32 GetUUID() const;
	UINT32 GetParentUUID() const;
	void SetParentUUID(UINT32 uuid);

	bool IsActive()const;

	void SetParent(GameObject* parent);
	void AddChild(GameObject* child);

	void Clear();

	void DeleteComponents();
	void DeleteChildren();
	bool DeleteChild(GameObject* obj);

	void CreateEmptyChild(GameObject* parent);

	void UpdateBoundingBoxes();

private:
	std::string name;
	bool active;
	UINT32 uuid;
	UINT32 parentUUID;

public:
	C_Transform* transform = nullptr;
	GameObject* parent = nullptr;
	//vector component
	std::vector<Component*> components;

	std::vector<GameObject*> children;
	//Bounding boxes
	AABB aabb;
	OBB obb;
	bool showBoundingBoxes;
	float3* obbVertices;
	float3* aabbVertices;

};


#endif //_GAME_OBJECT_H_
