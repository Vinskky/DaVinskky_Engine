#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject;

enum COMPONENT_TYPE 
{
	C_TRANSFORM = 1,
	C_MESH,
	C_MATERIAL
};

class Component
{
public:
	Component():active(false)
	{}
	virtual ~Component()
	{}

	virtual void Enable()
	{}
	virtual void Update()
	{}
	virtual void Disable()
	{}

private:
	COMPONENT_TYPE type;
	bool active;
	GameObject* owner;
};

#endif // !_COMPONENT_H_

