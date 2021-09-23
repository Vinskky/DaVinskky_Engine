#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"
#include "Globals.h"
#include "imgui.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:
	bool InitImGui()const;

public:
	
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

	float f;
	int counter;
};


#endif // !_MODULE_EDITOR_H_
