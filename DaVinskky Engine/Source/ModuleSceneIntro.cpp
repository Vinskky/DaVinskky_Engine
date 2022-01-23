#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "I_Scene.h"
#include "I_Texture.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "GameObject.h"

#include "External/mmgr/include/mmgr.h"

ModuleSceneIntro::ModuleSceneIntro():Module(),
cullingCamera(nullptr)
{
	SetName("Scene");
	sceneRoot = new GameObject(0, true);
	sceneRoot->SetName("MainScene");
	sceneRoot->SetParentUUID(sceneRoot->GetUUID());
	selectedGameObj = sceneRoot;
	sceneGameObjects.push_back(sceneRoot);
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	sceneRoot->Clear();
	RELEASE(sceneRoot);
	sceneGameObjects.clear();
}

// Load assets
bool ModuleSceneIntro::Start()
{
	bool ret = true;

	Importer::Scene::Import("Assets/Models/street2.FBX", sceneGameObjects);
	//Importer::Scene::Import("Assets/Models/warrior.fbx", sceneGameObjects);

	LOG("Loading Intro assets");

	//app->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//app->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent)
{
	if (sceneGameObjects.empty())
	{
		sceneRoot = new GameObject();
		sceneRoot->SetName("sceneRoot");
		sceneRoot->SetParentUUID(sceneRoot->GetUUID());
		selectedGameObj = sceneRoot;
		sceneGameObjects.push_back(sceneRoot);
		return sceneRoot;
	}

	GameObject* gameObject = new GameObject();
	gameObject->SetName(name);

	if (gameObject != nullptr)
	{
		if (parent != nullptr)
		{
			gameObject->SetParent(parent);
		}

		sceneGameObjects.push_back(gameObject);
	}

	return gameObject;
}

void ModuleSceneIntro::CreateSceneCamera(const char* cameraName)
{
	int winWidth;
	int winHeight;

	GameObject* sceneCamera = CreateGameObject(cameraName, sceneRoot);
	sceneCamera->CreateComponent(COMPONENT_TYPE::CAMERA);

	app->window->GetWindowsSize(app->window->window, winWidth, winHeight);
	sceneCamera->GetComponent<C_Camera>()->SetAspectRatio((float)winWidth / (float)winHeight);
	sceneCamera->GetComponent<C_Transform>()->SetPosition(0.0f, 5.0f, 25.0f);
}

C_Camera* ModuleSceneIntro::GetCullingCamera() const
{
	return cullingCamera;
}

void ModuleSceneIntro::SetCullingCamera(C_Camera* cullingCamera)
{
	C_Camera* prev_cull_cam = this->cullingCamera;

	this->cullingCamera = cullingCamera;

	if (prev_cull_cam != nullptr)
	{
		if (prev_cull_cam != cullingCamera)
		{
			prev_cull_cam->SetIsCulling(false);
		}
	}
}

bool ModuleSceneIntro::GameObjectIsInsideCullingCamera(GameObject* obj)
{
	if (cullingCamera == nullptr)
	{
		//LOG("[ERROR] Scene: There is currently no camera with culling activated!");
		return true;
	}

	bool intersects = cullingCamera->FrustumIntersectsAABB(obj->aabb);

	return intersects;
}

void ModuleSceneIntro::DeleteGameObjectFromVector(GameObject* toDelete)
{
	for (uint k = 0; k < sceneGameObjects.size(); ++k)
	{
		if (toDelete == sceneGameObjects[k])
		{
			sceneGameObjects.erase(sceneGameObjects.begin() + k);
			break;
		}
	}
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	for (auto gameobjs = sceneGameObjects.begin(); gameobjs != sceneGameObjects.end(); ++gameobjs)
	{
		if ((*gameobjs)->IsActive())
		{
			
			if (GameObjectIsInsideCullingCamera((*gameobjs)) || (*gameobjs) == cullingCamera->GetOwner())
			{
				//Take into consideration that if the gameobject should keep being updated it wont update if it's not 
				//inside the Frustum Culling
				(*gameobjs)->Update();

			}
		}
	}

	return UPDATE_CONTINUE;
}

