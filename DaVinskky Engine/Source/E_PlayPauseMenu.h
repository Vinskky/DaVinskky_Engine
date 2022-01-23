#pragma once
#ifndef _E_PLAYPAUSEMENU_H_
#define _E_PLAYPAUSEMENU_H_

#include "Editor.h"
#include "ModuleSceneIntro.h"
#include <vector>


class E_PlayPauseMenu : public Editor
{
public:
	E_PlayPauseMenu(const char* name, bool isActive = true);
	~E_PlayPauseMenu();
	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;
	

public:


};



#endif // !_E_PLAYPAUSEMENU_H_