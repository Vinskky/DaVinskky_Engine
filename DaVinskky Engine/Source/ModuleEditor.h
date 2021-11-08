#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include <vector>
#include "SDL.h"

class Editor;
class ETestPanel;
class E_MainMenuBar;
class E_Configuration;
class E_Console;
class E_About;
class E_Hierarchy;
class E_Inspector;


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

	bool BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags = ImGuiWindowFlags_None);                            // Generates a root window for docking.
	void BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags = ImGuiDockNodeFlags_None, ImVec2 size = { 0.0f, 0.0f });    // Generates a new dockspace.

	bool GetEvent(SDL_Event* event);

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
	E_Inspector* inspector = nullptr;

	bool hoveringEditor = false;
};


#endif // !_MODULE_EDITOR_H_
