#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
 
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

	//create component

private:
	std::string name;
	//vector component
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

};


#endif //_GAME_OBJECT_H_
