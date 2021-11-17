#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <string>
#include <vector>
#include "Component.h"

class C_Transform;

class GameObject
{
public:
	GameObject(bool active = true);
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

	void SetName(const char* str);
	const char* GetName()const;
	
	bool IsActive()const;

	void SetParent(GameObject* parent);
	void AddChild(GameObject* child);

	void Clear();

	void DeleteComponents();
	void DeleteChildren();
	bool DeleteChild(GameObject* obj);

private:
	std::string name;
	bool active;

public:
	C_Transform* transform = nullptr;
	GameObject* parent = nullptr;
	//vector component
	std::vector<Component*> components;

	std::vector<GameObject*> children;

};


#endif //_GAME_OBJECT_H_
