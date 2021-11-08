#ifndef _E_CONFIGURATION_H_
#define _E_CONFIGURATION_H_

#include "Editor.h"

#define MAX_HISTOGRAM_SIZE 100

class E_Configuration : public Editor
{
public:
	E_Configuration( const char* name, bool isActive = true);
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

	//Histogram Variables
	float fpsData[MAX_HISTOGRAM_SIZE];
	float msData[MAX_HISTOGRAM_SIZE];
	int fps;
	
	//Windows variables
	bool checkActive = true;
	char* iconStr = "*default*";
	float brightness = 1.000f;
	int width;
	int height;
	bool checkFullscreen;
	bool checkResizable;
	bool checkBorderless;
	bool checkFullDesktop;
	
	//FileSystem Variables
	bool ckActiveFS = true;
	char* basePath = "C:\...";
	char* readPath = ".";
	char* writePath = ".";

	//Input variables
	bool ckActiveInput = true;
	ImGuiTextBuffer logInputText;

};

#endif // !_E_CONFIGURATION_H_

