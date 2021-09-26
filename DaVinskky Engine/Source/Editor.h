#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <string>
#include <vector>

//Imgui
#include "imgui.h"
#include "imconfig.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


class Editor
{
public:

	Editor(const char* name, bool isActive = true): name(name),isActive(isActive)
	{}

	virtual ~Editor()
	{}

	virtual bool Draw(ImGuiIO& io)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

public:
	//Fuctions that can be usefull

	void Enable() 
	{
		if (!isActive)
			isActive = true;
	}

	void Disable() 
	{
		if (isActive)
			isActive = false;
	}

	const char* GetName()const 
	{
		return name;
	}

	bool IsActive()const
	{
		return isActive;
	}

	bool IsHover()const
	{
		return isHover;
	}

	bool IsClicked()const
	{
		return isClicked;
	}


private:
	const char* name;
	bool isActive;
	bool isHover;
	bool isClicked;
};

#endif // !_EDITOR_H_
