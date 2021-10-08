#include "Application.h"
#include "E_Console.h"

E_Console::E_Console( const char* name, bool isActive): Editor("Console")
{
}

E_Console::~E_Console()
{
}

bool E_Console::Draw(ImGuiIO& io)
{
	bool ret = true;
	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);

	ImGui::End();
	return ret;
}

bool E_Console::CleanUp()
{
	return true;
}
