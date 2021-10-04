#include "Application.h"
#include <shellapi.h>
#include <string>
#include <time.h>

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	sceneIntro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	// Scenes
	AddModule(sceneIntro);

	// Renderer last!
	AddModule(editor);
	AddModule(renderer3D);
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
	/* Whenever I want to fix frame rate acording config slider
	float frameMs = msTimer.Read();
	if (frameMs > 0 && frameMs < framMsCap)
	{
		SDL_Delay(framMsCap - frameMs);
	}*/

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

