#include "Application.h"
#include "Globals.h"
#include "E_About.h"
#include "E_MainMenuBar.h"

#include "version.h"

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
	ImGui::Text("DaVinskky Engine V0.1");
	ImGui::Spacing();

	ImGui::Bullet();
	if (ImGui::Button("Check our Repository"))
		ShellExecuteA(NULL, "open", "https://github.com/Vinskky/DaVinskky_Engine", NULL, NULL, SW_SHOWNORMAL);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Developed by:");
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_BLUE, "Sebastian Delgado:", "");
	ImGui::SameLine();
	if (ImGui::Button("Vinskky"))
		ShellExecuteA(NULL, "open", "https://github.com/Vinskky", NULL, NULL, SW_SHOWNORMAL);
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_BLUE, "Abraham Diaz:", "");
	ImGui::SameLine();
	if (ImGui::Button("Theran1"))
		ShellExecuteA(NULL, "open", "https://github.com/Theran1", NULL, NULL, SW_SHOWNORMAL);
	ImGui::Spacing();

	IMGUI_BULLET(IMGUI_BLUE, "Yeray Tarifa:", "");
	ImGui::SameLine();
	if (ImGui::Button("yeraytm"))
		ShellExecuteA(NULL, "open", "https://github.com/yeraytm", NULL, NULL, SW_SHOWNORMAL);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("3rd Party Libraries used:");
	ImGui::Spacing();

	SDL_version sdlVersion;
	SDL_GetVersion(&sdlVersion);
	ImGui::BulletText("SDL v%i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
	ImGui::Spacing();

	ImGui::BulletText("Open GL v3.1");
	ImGui::Spacing();

	ImGui::BulletText("Glew v2.1.0");
	ImGui::Spacing();

	ImGui::BulletText("ImGui v%s (Docking)", ImGui::GetVersion());
	ImGui::Spacing();

	ImGui::BulletText("Assimp v%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
	ImGui::Spacing();

	ImGui::BulletText("DevIL v1.7.8");
	ImGui::Spacing();

	ImGui::BulletText("MathGeoLib v2.0");
	ImGui::Spacing();

	ImGui::BulletText("PhysFS v3.0.2");
	ImGui::Spacing();

	ImGui::BulletText("nlohmann json v3.10.4");

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::TextColored(IMGUI_BLUE, "MIT License");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Copyright (c) 2021 Vinskky yeraytm Theran1");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: ");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::TextWrapped("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

	return true;
}
