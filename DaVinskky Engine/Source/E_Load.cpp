#include "Application.h"
#include "Globals.h"
#include "GameObject.h"
#include "E_Load.h"
#include "E_MainMenuBar.h"
#include "I_Scene.h"

E_Load::E_Load(const char* name, bool isActive) : Editor("Load Window")
{

}
E_Load::~E_Load()
{
}

bool E_Load::Draw(ImGuiIO& io)
{
	bool ret = true;
	if (app->editor->mainMenuPanel->GetLoadWindowState() == true)
	{
		ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
		LoadWindow();
		ImGui::End();
	}

	return ret;
}

bool E_Load::CleanUp()
{
	bool ret = true;

	return ret;
}

bool E_Load::LoadWindow()
{

	std::string sceneName;
	char buffer[128];
	strcpy_s(buffer, app->sceneIntro->sceneGameObjects[0]->GetName());
	if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		sceneName = buffer;

		Importer::Scene::Load(sceneName.c_str(), app->sceneIntro->sceneGameObjects);

	}


	return true;
}
