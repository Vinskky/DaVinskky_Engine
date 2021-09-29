#include "Application.h"
#include "Globals.h"
#include "imgui.h"
#include "E_Configuration.h"

E_Configuration::E_Configuration(Application* app, const char* name, bool isActive): Editor(app,"Configuration")
{
	appName = TITLE;
	orgName = "Organization";
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
		strcpy_s(buffer, TITLE);
		if (ImGui::InputText("Engine Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//modify variable
		}
		//ImGui::SameLine();
		//ImGui::Text("App Name");

		ImGui::InputText("", orgName, IM_ARRAYSIZE(str1));
		ImGui::SameLine();
		ImGui::Text("Organization");
	}

	return ret;
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
