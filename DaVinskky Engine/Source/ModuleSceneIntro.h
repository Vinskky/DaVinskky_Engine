#ifndef _MODULE_SCENE_INTRO_H_
#define _MODULE_SCENE_INTRO_H_

#include "Module.h"
#include "Globals.h"

#include <vector>
#include <External/MathGeoLib/include/Math/Reinterpret.h>

class GameObject;
class C_Camera;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro();
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(const char* name = nullptr, GameObject* parent = nullptr);
	
	//void SelectGameObjectThroughRaycast(const LineSegment& ray);

public:
	void CreateSceneCamera(const char* cameraName);
	C_Camera* GetCullingCamera() const;
	void SetCullingCamera(C_Camera* cullingCamera);
	bool GameObjectIsInsideCullingCamera(GameObject* obj);


public:
	std::vector<GameObject*> sceneGameObjects;
	GameObject* sceneRoot;
	GameObject* selectedGameObj;

	C_Camera* cullingCamera;
};

#endif // !_MODULE_SCENE_INTRO_H_


