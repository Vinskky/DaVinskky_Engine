#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "E_MainMenuBar.h"

E_MainMenuBar::E_MainMenuBar(Application* app, const char* name, bool isActive): Editor(app,"Main Menu")
{
}

E_MainMenuBar::~E_MainMenuBar()
{
}

bool E_MainMenuBar::Draw(ImGuiIO& io)
{
	bool ret = true;
	
	ImGui::BeginMainMenuBar();

	FileMenu();
	ViewMenu();
	HelpMenu();

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

bool E_MainMenuBar::FileMenu()
{
	bool ret = true;
	
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit", "ESC"))
		{
			
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
		if (ImGui::MenuItem("Console", "1", nullptr, true))
		{
			
		}
		if (ImGui::MenuItem("Configuration", "4", nullptr, true))
		{

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
			App->RequestBrowser("https://github.com/Vinskky/DaVinskky_Engine");
		}
		if (ImGui::MenuItem("Download Latest", "", nullptr, true))
		{
			App->RequestBrowser("https://github.com/Vinskky/DaVinskky_Engine/releases");
		}
		if (ImGui::MenuItem("Report bug", "", nullptr, true))
		{
			App->RequestBrowser("https://github.com/Vinskky/DaVinskky_Engine/issues");
		}
		if (ImGui::MenuItem("About", "", nullptr, true))
		{

		}
		ImGui::EndMenu();
	}
	return ret;
}
