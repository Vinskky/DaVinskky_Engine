#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "OpenGL.h"
#include "Globals.h"

#include "Editor.h"
#include "E_TestPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


ModuleEditor::ModuleEditor(Application* app, bool startEnabled) : Module(app, startEnabled),
testPanel(new ETestPanel("ImGui Test"))
{
    AddEditorPanel(testPanel);
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

    //iterate all different editor panels stored in vector of editorPanels to be able to draw them.
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    //Start Dear ImGui's frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

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

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
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
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    //This line was making the window render bad
    //SDL_GL_SwapWindow(App->window->window); 

    return true;
}

