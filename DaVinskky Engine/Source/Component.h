#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject;

enum class COMPONENT_TYPE 
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
public:
	Component(GameObject* owner,COMPONENT_TYPE type, bool active = true): owner(owner), type(type),active(active)
	{}
	virtual ~Component()
	{}

	virtual void Enable()
	{}
	virtual void Update()
	{}
	virtual void Disable()
	{}
	
	virtual inline COMPONENT_TYPE GetType()const
	{
		return type;
	}

	bool IsActive()const { return active; }

	GameObject* GetOwner()const { return owner; }

private:
	COMPONENT_TYPE type;
	bool active;
	GameObject* owner;
};

#endif // !_COMPONENT_H_

