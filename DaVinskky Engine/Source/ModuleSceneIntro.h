#ifndef _MODULE_SCENE_INTRO_H_
#define _MODULE_SCENE_INTRO_H_

#include "Module.h"
#include "Globals.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro();
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
};

#endif // !_MODULE_SCENE_INTRO_H_


