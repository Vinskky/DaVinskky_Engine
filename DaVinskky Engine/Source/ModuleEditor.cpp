#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "OpenGL.h"
#include "Globals.h"

#include "Editor.h"
#include "E_TestPanel.h"
#include "E_MainMenuBar.h"
#include "E_Configuration.h"
#include "E_Console.h"
#include "E_About.h"
#include "E_Hierarchy.h"
#include "E_Inspector.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


ModuleEditor::ModuleEditor() : Module(),
//testPanel(new ETestPanel(app,"ImGui Test")),
mainMenuPanel(new E_MainMenuBar("Main Menu")),
configPanel(new E_Configuration("Configuration")),
consolePanel(new E_Console("Configuration")),
aboutPanel(new E_About("About")),
hierarchy(new E_Hierarchy("Hierarchy")),
inspector(new E_Inspector("Inspector"))
{
    SetName("Editor");
    //AddEditorPanel(testPanel);
    AddEditorPanel(mainMenuPanel);
    AddEditorPanel(configPanel);
    AddEditorPanel(consolePanel);
    AddEditorPanel(aboutPanel);
    AddEditorPanel(hierarchy);
    AddEditorPanel(inspector);
}

ModuleEditor::~ModuleEditor()
{
    std::vector<Editor*>::reverse_iterator item = editorPanels.rbegin();
    while (item != editorPanels.rend())
    {
        (*item)->CleanUp();
        RELEASE(*item);
        ++item;
    }
}

bool ModuleEditor::Start()
{
	bool ret = true;

	InitImGui();

	return ret;
}



update_status ModuleEditor::Update(float dt)
{
    update_status ret = update_status::UPDATE_CONTINUE;


    return ret;
}

update_status ModuleEditor::PostUpdate(float dt)
{
    update_status ret = update_status::UPDATE_CONTINUE;
    
    hoveringEditor = false;

    //iterate all different editor panels stored in vector of editorPanels to be able to draw them.
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
   
    //Start Dear ImGui's frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (BeginRootWindow(io,"rootWindow",true,ImGuiWindowFlags_MenuBar))
    {
        bool draw = true;
        for (uint i = 0; i < editorPanels.size(); ++i)
        {
            if (editorPanels[i]->IsActive())
            {
                draw = editorPanels[i]->Draw(io);

                if (!draw)
                {
                    ret = update_status::UPDATE_STOP;
                    LOG("[EDITOR] Exited through %s Panel", editorPanels[i]->GetName());
                    break;
                }
            }
        }
        ImGui::End();
    }

    RenderEditorPanels();
    
    return ret;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}

bool ModuleEditor::InitImGui() const
{
	bool ret = true;

	// ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;											// Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;                                         // Enable Multi-Viewport / Platform Windows
    
    // Setup Dear ImGui style
	ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

	ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->renderer3D->context);
    ImGui_ImplOpenGL3_Init(0);


	return ret;
}

void ModuleEditor::AddEditorPanel(Editor* panel)
{
    editorPanels.push_back(panel);
}


bool ModuleEditor::RenderEditorPanels() const
{
    // Rendering
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
    
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    return true;
}

void ModuleEditor::UpdateFrameData(float frames, float ms)
{
    configPanel->UpdateFrameData(frames, ms);
}

void ModuleEditor::AddLogFromEditor(const char* str)
{
    if (app->quit == false && app->editor != nullptr)
    {
        if (consolePanel != nullptr)
        {
            consolePanel->AddLogConsole(str);
        }
    }
    
}

bool ModuleEditor::BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags)
{
    bool ret = true;

    ImGuiViewport* viewport = ImGui::GetWindowViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ret = ImGui::Begin(window_id, &ret, window_flags);
    ImGui::PopStyleVar(3);

    if (docking)
    {
        BeginDockspace(io, window_id, ImGuiDockNodeFlags_PassthruCentralNode);
    }

    return ret;
}

void ModuleEditor::BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags, ImVec2 size)
{
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dckspace_id = ImGui::GetID(dockspace_id);
        ImGui::DockSpace(dckspace_id, size, docking_flags);
    }
}

bool ModuleEditor::GetEvent(SDL_Event* event)
{
    return ImGui_ImplSDL2_ProcessEvent(event);
}
