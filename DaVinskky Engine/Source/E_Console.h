#ifndef _E_CONSOLE_H_
#define _E_CONSOLE_H_

#include "Editor.h"
#include <vector>

#define THRESHOLD_LOGS 1000

class E_Console : public Editor
{
public:
	E_Console(const char* name, bool isActive = true);
	~E_Console();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

	void AddLogConsole(const char* logStr);
	void ClearLog();

public:
	std::vector<char*> logs;

};



#endif // !_E_CONSOLE_H_

