#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "E_MainMenuBar.h"

E_MainMenuBar::E_MainMenuBar(const char* name, bool isActive): Editor("Main Menu")
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

update_status E_MainMenuBar::FileMenu()
{
	update_status ret = update_status::UPDATE_CONTINUE;
	
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit", "ESC"))
		{
			ret = update_status::UPDATE_STOP;
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
		if (ImGui::MenuItem("About", "", &aboutWindow, true))
		{
			//AboutPopUp();
			if (ImGui::Begin("About", &aboutWindow, ImGuiWindowFlags_None))
			{
				ImGui::Text("DaVinskky Engine V0.1\n");
				ImGui::Text("Created by Sebastián Delgado\n");
				ImGui::Separator();
				ImGui::Text("3rd Party Libraries used:\n");
				ImGui::BulletText("SDL 2.06\n");
				ImGui::BulletText("Glew 2.0.0\n");
				ImGui::BulletText("ImGui\n");
				ImGui::BulletText("MathGeoLib\n");

				ImGui::Separator();
				ImGui::Text("License:\n");
				ImGui::Text("MIT License\n");
				ImGui::Text("Copyright (c) 2021 DaVinskky Engine\n"
					"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
					"of this software and associated documentation files(the ""Software""), to deal\n"
					"in the Software without restriction, including without limitation the rights\n"
					"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n"
					"copies of the Software, and to permit persons to whom the Software is\n"
					"furnished to do so, subject to the following conditions :\n\n"
					"The above copyright noticeand this permission notice shall be included in all\n"
					"copies or substantial portions of the Software.\n\n"
					"THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
					"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
					"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n"
					"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
					"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
					"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
					"SOFTWARE.");
			}
			ImGui::End();
		}
		ImGui::EndMenu();
	}
	
	return ret;
}

bool E_MainMenuBar::AboutPopUp()
{
	
	return true;
}
