#ifndef _E_CONFIGURATION_H_
#define _E_CONFIGURATION_H_

#include "Editor.h"

class E_Configuration : public Editor
{
public:
	E_Configuration(Application* app, const char* name, bool isActive = true);
	~E_Configuration();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

private:
	bool OptionsPanel();
	bool ApplicationHeader();
	bool WindowHeader();
	bool FileSystemHeader();
	bool InputHeader();
	bool HardwareHeader();

private:

	char* appName = nullptr;
	char* orgName = nullptr;
	static char str0[128];
	static char str1[128];
};

#endif // !_E_CONFIGURATION_H_

