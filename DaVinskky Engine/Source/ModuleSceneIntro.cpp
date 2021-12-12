#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "I_Scene.h"
#include "I_Texture.h"
#include "GameObject.h"

ModuleSceneIntro::ModuleSceneIntro()
{
	SetName("Scene");
	sceneRoot = new GameObject();
	sceneRoot->SetName("sceneRoot");
	sceneRoot->SetParentUUID(sceneRoot->GetUUID());
	selectedGameObj = sceneRoot;
	sceneGameObjects.push_back(sceneRoot);
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	bool ret = false;

	Importer::Scene::Import("Assets/Models/BakerHouse.fbx", sceneGameObjects);
	//ret = Importer::Scene::Save("SampleScene", sceneGameObjects);
	//ret = Importer::Scene::Load("SampleScene", sceneGameObjects, sceneGameObjects[0]);
	//Importer::Scene::Import("Assets/Models/warrior.fbx", sceneGameObjects);

	LOG("Loading Intro assets");

	app->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	app->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	//Plane p(0, 1, 0, 0);
	//p.axis = true;
	//p.Render();

	for (auto gameobjs = sceneGameObjects.begin(); gameobjs != sceneGameObjects.end(); ++gameobjs)
	{
		if ((*gameobjs)->IsActive())
		{
			(*gameobjs)->Update();
		}
	}

	return UPDATE_CONTINUE;
}

