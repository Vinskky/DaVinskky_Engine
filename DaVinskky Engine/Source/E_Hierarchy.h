#ifndef _E_HIERACHY_H_
#define _E_HIERACHY_H_

#include "Editor.h"

class GameObject;

class E_Hierarchy : public Editor
{
public:
	E_Hierarchy(const char* name, bool isActive = true);
	~E_Hierarchy();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;


private:
	//Functionality for herarchy
	void DrawGameObjects(GameObject* rootGO);
	void ProcessRoot(GameObject* objs);
};


#endif