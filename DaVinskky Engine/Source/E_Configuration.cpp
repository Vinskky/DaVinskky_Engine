#include "Application.h"
#include "Globals.h"
#include "imgui.h"
#include "E_Configuration.h"

E_Configuration::E_Configuration(Application* app, const char* name, bool isActive): Editor(app,"Configuration")
{
	appName = TITLE;
	orgName = "Organization";
	fps = 0;
}

E_Configuration::~E_Configuration()
{
}

bool E_Configuration::Draw(ImGuiIO& io)
{
	bool ret = true;
	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_MenuBar);
	OptionsPanel();
	ApplicationHeader();

	ImGui::End();
	return ret;
}

bool E_Configuration::CleanUp()
{
	bool ret = true;

	return ret;
}

void E_Configuration::UpdateFrameData(int frames, int ms)
{
	for (uint i = 0; i < (MAX_HISTOGRAM_SIZE - 1); ++i)
	{
		fpsData[i] = fpsData[i + 1];
		msData[i] = msData[i + 1];
	}
	fpsData[MAX_HISTOGRAM_SIZE - 1] = (float)frames;
	msData[MAX_HISTOGRAM_SIZE - 1] = (float)ms;
}

bool E_Configuration::OptionsPanel()
{
	bool ret = true;
	
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("Default Settings"))
		{
			//Reset to default values
		}
		if (ImGui::MenuItem("Load"))
		{
			//Parser + JSON
		}	
		if (ImGui::MenuItem("Save"))
		{
			//Parser + JSON
		}	
		ImGui::EndMenu();
	}
	

	return ret;
}

bool E_Configuration::ApplicationHeader()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Application"))
	{
		char buffer[128];
		strcpy_s(buffer, appName);
		if (ImGui::InputText("Engine Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//modify variable
		}
		//ImGui::SameLine();
		//ImGui::Text("App Name");

		char bufferOrg[128];
		strcpy_s(bufferOrg, orgName);
		if (ImGui::InputText("Organization", bufferOrg, IM_ARRAYSIZE(bufferOrg), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//modify variable
		}

		ImGui::SliderInt("Max FPS", &fps, 0, 144);
		ImGui::Text("Limit Framerate: %d", fps);
		//Plots
		PlotFrameHistogram();
		
	}

	return ret;
}

bool E_Configuration::PlotFrameHistogram()
{
	ImGui::PlotHistogram("FPS", fpsData, IM_ARRAYSIZE(fpsData), 0, NULL, 0.0f, 144.0f, ImVec2(0, 80));
	ImGui::PlotHistogram("MS", msData, IM_ARRAYSIZE(msData), 0, NULL, 0.0f, 40.0f, ImVec2(0, 80));
	return false;
}

bool E_Configuration::WindowHeader()
{
	bool ret = true;

	return ret;
}

bool E_Configuration::FileSystemHeader()
{
	bool ret = true;

	return ret;
}

bool E_Configuration::InputHeader()
{
	bool ret = true;

	return ret;
}

bool E_Configuration::HardwareHeader()
{
	bool ret = true;

	return ret;
}
