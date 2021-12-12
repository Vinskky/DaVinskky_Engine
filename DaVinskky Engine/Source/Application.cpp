#include "Application.h"
#include "Config.h"
#include <shellapi.h>
#include <string>
#include <time.h>

#include "JsonFile.h"

Application::Application(int argc, char** argv) : argc(argc), argv(argv)
{
	fileSystem = new M_FileSystem();
	window = new ModuleWindow();
	input = new ModuleInput();
	sceneIntro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new ModuleEditor();
	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(fileSystem);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	// Scenes
	AddModule(sceneIntro);

	// Renderer last!
	AddModule(editor);
	AddModule(renderer3D);

	title = TITLE;
	organization = ORGANIZATION;
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend())
	{
		delete *item;
		++item;
	}
}

bool Application::Init()
{
	bool ret = true;
	//char* buffer = nullptr;

	//uint size = fileSystem->Load("Engine/Settings.JSON", &buffer);

	//if (size <= 0)
	//{
	//	uint defSize = fileSystem->Load("Engine/DefaultSettings.JSON", &buffer);
	//	if (defSize <= 0)
	//	{
	//		LOG("[error] failed to load project settings");
	//		return false;
	//	}
	//}

	//Config config(buffer);
	//Config node = config.GetNode("EditorState");
	
	// Call Init() in all modules
	std::vector<Module*>::iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}

	msTimer.Start();
	return ret;
}

void Application::UpdateFrameData(float frames, float ms)
{
	editor->UpdateFrameData(frames, ms);
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (maxFPS > 0)
		framMsCap = 1000 / (float)maxFPS;

	float frameMs = msTimer.Read();
	if (frameMs > 0 && frameMs < framMsCap && maxFPS != 0)
	{
		SDL_Delay(framMsCap - frameMs);
	}

	frameCount++;
	if (secondCount.Read() >= 1000)
	{
		secondCount.Start();
		lastFps = frameCount;
		frameCount = 0;
	}

	UpdateFrameData(lastFps, (float)msTimer.Read());
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (quit)
	{
		ret = UPDATE_STOP;
	}

	PrepareUpdate();
	std::vector<Module*>::iterator item = modules.begin();

	while(item != modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		++item;
	}

	item = modules.begin();

	while(item != modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = modules.begin();

	while(item != modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		++item;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::vector<Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

void Application::RequestBrowser(const char* url)const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::GetSDLVer(int& major, int& minor, int& patch)
{
	SDL_version v;
	SDL_GetVersion(&v);
	major = v.major;
	minor = v.minor;
	patch = v.patch;
}

void Application::GetCPU(int& count, int& size)
{
	count = SDL_GetCPUCount();
	size = SDL_GetCPUCacheLineSize();
}


float Application::GetRAM()
{
	return (float)SDL_GetSystemRAM() / 1000.0f;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}

void Application::GetCaps(bool& threeD, bool& altiVec, bool& avx, bool& avx2, bool& mmx, bool& rdtsc, bool& sse, bool& sse2, bool& sse3, bool& sse41, bool& sse42)
{
	threeD = SDL_Has3DNow();
	altiVec = SDL_HasAltiVec();
	avx = SDL_HasAVX();
	avx2 = SDL_HasAVX2();
	mmx = SDL_HasMMX();
	rdtsc = SDL_HasRDTSC();
	sse = SDL_HasSSE();
	sse2 = SDL_HasSSE2();
	sse3 = SDL_HasSSE3();
	sse41 = SDL_HasSSE41();
	sse42 = SDL_HasSSE42();
}

void Application::GetGPU()
{
	
}

void Application::AddLogFromApp(const char* str)
{
	if (editor != nullptr)
	{
		std::string full_log = str;                                                    // TMP. Switch to normalize later.

		uint log_start = full_log.find_last_of("\\") + 1;                            // Gets the position of the last "" in the log string.
		uint log_end = full_log.size();                                            // The last position of the log will be equal to the size of it.

		std::string short_log = full_log.substr(log_start, log_end);                // Returns the string that is within the given positions.

		editor->AddLogFromEditor(short_log.c_str());
	}
}

const char* Application::GetTitleName() const
{
	return title.c_str();
}

const char* Application::GetOrganizationName() const
{
	return organization.c_str();
}

void Application::SetTitleName(char* str)
{
	title = str;
}

void Application::SetOrganizationName(char* str)
{
	organization = str;
}

int Application::GetMaxFPS() const
{
	return maxFPS;
}

void Application::SetMaxFPS(int value)
{
	maxFPS = value;
}

