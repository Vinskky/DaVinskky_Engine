#include "Application.h"
#include "E_Console.h"

#include "External/mmgr/include/mmgr.h"

E_Console::E_Console( const char* name, bool isActive): Editor("Console")
{
}

E_Console::~E_Console()
{
}

bool E_Console::Draw(ImGuiIO& io)
{
	bool ret = true;

	//Checks hover to avoid unnecesary inputs
	

	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
	
	if (app->editor->hoveringEditor == false)
		app->editor->hoveringEditor = ImGui::IsWindowHovered();

	for (uint i = 0; i < logs.size(); ++i)
	{
		ImGui::TextUnformatted(logs[i]);
	}
	ImGui::End();
	return ret;
}

bool E_Console::CleanUp()
{
	return true;
}

void E_Console::AddLogConsole(const char* logStr)
{
	if (logs.size() + 1 > THRESHOLD_LOGS)
	{
		ClearLog();
	}

	char* tmp = _strdup(logStr);

	logs.push_back(tmp);
}

void E_Console::ClearLog()
{
	for (uint i = 0; i < logs.size(); ++i)
	{
		free(logs[i]);
	}

	logs.clear();
}
