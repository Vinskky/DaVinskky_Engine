#ifndef _E_CONFIGURATION_H_
#define _E_CONFIGURATION_H_

#include "Editor.h"

#define MAX_HISTOGRAM_SIZE 100

class E_Configuration : public Editor
{
public:
	E_Configuration(Application* app, const char* name, bool isActive = true);
	~E_Configuration();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

	void UpdateFrameData(int frames, int ms);

private:
	bool OptionsPanel();
	bool ApplicationHeader();
	bool PlotFrameHistogram();
	bool WindowHeader();
	bool FileSystemHeader();
	bool InputHeader();
	bool HardwareHeader();

private:

	float fpsData[MAX_HISTOGRAM_SIZE];
	float msData[MAX_HISTOGRAM_SIZE];

	char* appName = nullptr;
	char* orgName = nullptr;
	int fps;
};

#endif // !_E_CONFIGURATION_H_

