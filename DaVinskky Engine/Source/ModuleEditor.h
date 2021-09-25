#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"
#include "Globals.h"
#include "imgui.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool startEnabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:
	bool InitImGui()const;

public:
	
	bool showDemoWindow;
	bool showAnotherWindow;
	ImVec4 clearColor;

	float f;
	int counter;
};


#endif // !_MODULE_EDITOR_H_
