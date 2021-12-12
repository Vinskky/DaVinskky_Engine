#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleSceneIntro.h"
#include "E_Console.h"
#include "E_MainMenuBar.h"
#include "E_Configuration.h"
#include "E_About.h"
#include "E_Save.h"
#include "E_Load.h"
#include "GameObject.h"
#include "I_Scene.h"

#include "External/mmgr/include/mmgr.h"

E_MainMenuBar::E_MainMenuBar(const char* name, bool isActive): Editor("Main Menu")
{
	saveWindow = false;
	loadWindow = false;

	showcase = false;
	aboutWindow = false;
	showConsole = true;
	showConfig = true;
}

E_MainMenuBar::~E_MainMenuBar()
{
}

bool E_MainMenuBar::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::BeginMainMenuBar();

	//Checks hover to avoid unnecesary inputs
	if (app->editor->hoveringEditor == false)
		app->editor->hoveringEditor = ImGui::IsWindowHovered();

	FileMenu();
	ViewMenu();
	HelpMenu();
	CreateMenu();

	ImGui::EndMainMenuBar();
	
	if (showcase)
		ImGui::ShowDemoWindow();

	return ret;
}

bool E_MainMenuBar::CleanUp()
{
	bool ret = true;

	return ret;
}

bool E_MainMenuBar::GetAboutWindowState() const
{
	return aboutWindow;
}

bool E_MainMenuBar::GetSaveWindowState() const
{
	return saveWindow;
}

bool E_MainMenuBar::GetLoadWindowState() const
{
	return loadWindow;
}

bool E_MainMenuBar::FileMenu()
{
	bool ret = true;
	
	if (ImGui::BeginMenu("File"))
	{

		if (ImGui::MenuItem("Save Scene", "", saveWindow, true))
		{
			if (saveWindow)
			{
				app->editor->savePanel->Disable();
			}
			else
			{
				app->editor->savePanel->Enable();

			}
			saveWindow = !saveWindow;
		}
		if (ImGui::MenuItem("Load Scene", "", loadWindow, true))
		{
			if (loadWindow)
			{
				app->editor->loadPanel->Disable();
			}
			else
			{
				app->editor->loadPanel->Enable();

			}
			loadWindow = !loadWindow;
		}
		if (ImGui::MenuItem("Quit", "ESC"))
		{
			app->quit = true;
		}
			
		ImGui::EndMenu();
	}
	

	return ret;
}

bool E_MainMenuBar::ViewMenu()
{
	bool ret = true;
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Console", "1", showConsole, true))
		{
			if (showConsole)
			{
				app->editor->consolePanel->Disable();
			}
			else
			{
				app->editor->consolePanel->Enable();
			}
			showConsole = !showConsole;
		}
		if (ImGui::MenuItem("Configuration", "4", showConfig, true))
		{
			if (showConfig)
			{
				app->editor->configPanel->Disable();
			}
			else
			{
				app->editor->configPanel->Enable();
			}
			showConfig = !showConfig;
		}
		ImGui::EndMenu();
	}
	return ret;
}

bool E_MainMenuBar::HelpMenu()
{
	bool ret = true;
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("GuiDemo"))
			showcase = !showcase;

		if (ImGui::MenuItem("Documentation", "", nullptr, true))
		{
			app->RequestBrowser("https://github.com/Vinskky/DaVinskky_Engine");
		}
		if (ImGui::MenuItem("Download Latest", "", nullptr, true))
		{
			app->RequestBrowser("https://github.com/Vinskky/DaVinskky_Engine/releases");
		}
		if (ImGui::MenuItem("Report bug", "", nullptr, true))
		{
			app->RequestBrowser("https://github.com/Vinskky/DaVinskky_Engine/issues");
		}
		if (ImGui::MenuItem("About", "", aboutWindow, true))
		{
			if (aboutWindow)
			{
				app->editor->aboutPanel->Disable();
			}
			else
			{
				app->editor->aboutPanel->Enable();
				
			}
			aboutWindow = !aboutWindow;
		}
		ImGui::EndMenu();
	}
	
	return ret;
}

bool E_MainMenuBar::CreateMenu()
{
	bool ret = true;

	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Empty Game Object"))
		{
			//Creating a game object empty means that only will have Component transform.
			//We should keep in mind the parent. (rootscene or game object selected?)
			app->sceneIntro->CreateGameObject("Empty", app->sceneIntro->sceneRoot);
		}
		ImGui::EndMenu();
	}

	return ret;
}