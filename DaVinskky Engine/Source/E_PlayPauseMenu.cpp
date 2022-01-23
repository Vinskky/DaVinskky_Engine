#include "E_PlayPauseMenu.h"
#include "Application.h"
#include "GameTime.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "E_Save.h"
#include "E_Load.h"
#include "I_Scene.h"


E_PlayPauseMenu::E_PlayPauseMenu(const char* name, bool isActive) : Editor("Play Pause")
{
}

E_PlayPauseMenu::~E_PlayPauseMenu()
{
}



bool E_PlayPauseMenu::Draw(ImGuiIO& io)
{
	ImGui::Begin("Play Pause");
	std::string name = app->sceneIntro->msGameTimer.running ? "Pause" : "Play";

	//std::string name = GameTimer::gameTimer ? "Stop" : "Play";

	if (ImGui::Button(name.c_str()))
	{
		//App->scene->GameTime.running = !App->scene->GameTime.running;

		if (app->sceneIntro->msGameTimer.running == false)
		{
			//Save
			
			Importer::Scene::Save("TimeScene", app->sceneIntro->sceneGameObjects);
			app->sceneIntro->msGameTimer.Resume();
		}
		else if (app->sceneIntro->msGameTimer.running == true)
		{
			app->sceneIntro->msGameTimer.Pause();
		}
	}


	ImGui::SameLine();

	if (app->sceneIntro->msGameTimer.Read() > 0) {

		if (ImGui::Button("Stop"))
		{
			//Load
			Importer::Scene::Load("TimeScene", app->sceneIntro->sceneGameObjects);

			app->sceneIntro->msGameTimer.Restart();
		}
	}

	ImGui::SameLine(100);

	ImGui::Text("GameTime:");

	ImGui::SameLine();

	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%.2f", (app->sceneIntro->msGameTimer.Read() / 1000.0f));

	ImGui::End();


	return true;
}



bool E_PlayPauseMenu::CleanUp()
{
	bool ret = true;


	return ret;
}