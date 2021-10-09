#include "Application.h"
#include "Globals.h"
#include "E_About.h"
#include "E_MainMenuBar.h"

E_About::E_About(const char* name, bool isActive) : Editor("About Window")
{

}
E_About::~E_About()
{
}

bool E_About::Draw(ImGuiIO& io)
{
	bool ret = true;
	if (app->editor->mainMenuPanel->GetAboutWindowState() == true)
	{
		ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
		AboutWindow();
		ImGui::End();
	}
	
	return ret;
}

bool E_About::CleanUp()
{
	bool ret = true;

	return ret;
}

bool E_About::AboutWindow()
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

	return true;
}
