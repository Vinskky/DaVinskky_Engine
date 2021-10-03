#ifndef _MODULE_H_
#define _MODULE_H_

#include "Globals.h"

class Config;
class Application;

class Module
{
private :
	bool enabled;

public:
	Application* App;

	Module(Application* parent, bool startEnabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void SaveConfig(Config& root) const
	{}

	virtual void LoadConfig(Config& root)
	{}

};

#endif // !_MODULE_H_