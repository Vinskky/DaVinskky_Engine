#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include <vector>

class Editor;
class ETestPanel;
class E_MainMenuBar;
class E_Configuration;
class E_Console;
class E_About;
class E_Hierarchy;


class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:
	bool InitImGui()const;
	void AddEditorPanel(Editor* panel);

	bool RenderEditorPanels()const;

	void UpdateFrameData(float frames, float ms);

	//Log Console editor function
	void AddLogFromEditor(const char* str);

public:
	
	bool showDemoWindow;
	bool showAnotherWindow;
	ImVec4 clearColor;

	float f;
	int counter;

public:
	std::vector<Editor*> editorPanels;

	//ETestPanel* testPanel = nullptr;
	E_MainMenuBar* mainMenuPanel = nullptr;
	E_Configuration* configPanel = nullptr;
	E_Console* consolePanel = nullptr;
	E_About* aboutPanel = nullptr;
	E_Hierarchy* hierarchy = nullptr;
};


#endif // !_MODULE_EDITOR_H_
