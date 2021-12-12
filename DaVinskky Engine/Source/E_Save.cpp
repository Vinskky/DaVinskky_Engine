#include "Application.h"
#include "Globals.h"
#include "GameObject.h"
#include "E_Save.h"
#include "E_MainMenuBar.h"
#include "I_Scene.h"

#include "External/mmgr/include/mmgr.h"

E_Save::E_Save(const char* name, bool isActive) : Editor("Save Window")
{

}
E_Save::~E_Save()
{
}

bool E_Save::Draw(ImGuiIO& io)
{
	bool ret = true;
	if (app->editor->mainMenuPanel->GetSaveWindowState() == true)
	{
		ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
		SaveWindow();
		ImGui::End();
	}

	return ret;
}

bool E_Save::CleanUp()
{
	bool ret = true;

	return ret;
}

bool E_Save::SaveWindow()
{
	
		std::string sceneName;
		char buffer[128];
		strcpy_s(buffer, app->sceneIntro->sceneGameObjects[0]->GetName());
		if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			sceneName = buffer;

			Importer::Scene::Save(sceneName.c_str(), app->sceneIntro->sceneGameObjects);

		}


	return true;
}
