#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <vector>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* sceneIntro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;

private:

	//CAP FPS
	int maxFPS = 0;
	float framMsCap = 0.0f;

	Timer	msTimer;
	float	dt;

	//FPS
	Timer secondCount;
	int frameCount;
	int lastFps;

	std::vector<Module*> modules;
	

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* url)const;

	void UpdateFrameData(float frames, float ms);

	void GetSDLVer(int& major, int& minor, int& patch);
	void GetCPU(int& count, int& size);
	float GetRAM();
	void GetCaps(bool& threeD, bool& altiVec, bool& avx, bool& avx2, bool& mmx, bool& rdtsc, bool& sse, bool& sse2, bool& sse3, bool& sse41, bool& sse42);
	void GetGPU();
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	
};

#endif // !_APPLICATION_H_

