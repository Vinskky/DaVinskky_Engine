#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	bool Init()override;
	bool CleanUp();

	void SetTitle(const char* title);
	//Imgui Values

	float GetBrightness()const;
	void SetBrightness(float value);

	void GetWindowsSize(SDL_Window* window, int& width, int& height);
	void SetWindowSize(uint width, uint height);


public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;
};

#endif // __ModuleWindow_H__