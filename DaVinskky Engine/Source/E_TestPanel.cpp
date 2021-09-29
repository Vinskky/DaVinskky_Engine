#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "ModuleSceneIntro.h"

#include "E_TestPanel.h"

ETestPanel::ETestPanel(Application* app,const char* name, bool isActive): Editor(app, "Test")
{
}

ETestPanel::~ETestPanel()
{
}

bool ETestPanel::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_MenuBar);

	TestMenuBar();
	TestExamplesBar();
	ImGui::Text("ImGui says hello.");
	TestHelpMenu();
	TestWindowOptionsMenu();
	TestWidgetsMenu();
	//Buttons and checkboxes
	
	if (ImGui::Button("Button")){}
	if (ImGui::Checkbox("checkbox",&checkBox)) {}
	ImGui::Separator();
	ImGui::SameLine();
	if (ImGui::Checkbox("radio a", &checkBoxA)) {}
	ImGui::SameLine();
	if (ImGui::Checkbox("radio b", &checkBoxB)) {}
	ImGui::SameLine();
	if (ImGui::Checkbox("radio c", &checkBoxC)) {}
	ImGui::Separator();
	ImGui::SameLine();
	if (ImGui::Button("Click")) {}
	ImGui::SameLine();
	if (ImGui::Button("Click")) {}	
	ImGui::SameLine();
	if (ImGui::Button("Click")) {}	
	ImGui::SameLine();
	if (ImGui::Button("Click")) {}
	ImGui::SameLine();
	if (ImGui::Button("Click")) {}
	ImGui::SameLine();
	if (ImGui::Button("Click")) {}
	ImGui::Separator();

	

	ImGui::End();

	return ret;
}

bool ETestPanel::CleanUp()
{
	bool ret = true;


	return ret;
}

bool ETestPanel::TestMenuBar()
{
	bool ret = true;
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("Menu"))
	{
		ImGui::MenuItem("New", "Ctrl+N", nullptr, false);
		ImGui::Separator();
		ImGui::MenuItem("Open", "Ctrl+o", nullptr, false);
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	return ret;
}

bool ETestPanel::TestExamplesBar()
{
	bool ret = true;
	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("Examples"))
	{
		ImGui::MenuItem("Example 1", "Ctrl+E", nullptr, false);
		ImGui::Separator();
		ImGui::MenuItem("Example 2", "Ctrl+E", nullptr, false);
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	return ret;
}

bool ETestPanel::TestHelpMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Help"))
	{
	}

	return ret;
}

bool ETestPanel::TestWindowOptionsMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("window options"))
	{
	}

	return ret;
}

bool ETestPanel::TestWidgetsMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("widgets"))
	{
		if (ImGui::CollapsingHeader("Tree"))
		{
			
		}

		if (ImGui::CollapsingHeader("Bullets"))
		{

		}

		if (ImGui::CollapsingHeader("Colored Text"))
		{

		}
		if (ImGui::CollapsingHeader("World Wrapping"))
		{

		}
		if (ImGui::CollapsingHeader("UTF-8 Text"))
		{

		}
		if (ImGui::CollapsingHeader("Images"))
		{

		}
		if (ImGui::CollapsingHeader("Selectables"))
		{

		}
		if (ImGui::CollapsingHeader("Filtered Text Input"))
		{

		}
		if (ImGui::CollapsingHeader("Multi-line Text Input"))
		{

		}
	}

	return ret;
}

bool ETestPanel::TestButtonsColors()
{
	return true;
}

bool ETestPanel::TestInputMenu()
{
	return true;
}
