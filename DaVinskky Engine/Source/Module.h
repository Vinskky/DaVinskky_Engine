#ifndef _MODULE_H_
#define _MODULE_H_

#include "Globals.h"
#include <string>
class Config;
class Application;

class Module
{
private :

	bool enabled;
	char* name;
public:

	Module(): enabled(false)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		enabled = true;
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

	virtual void SetName(char* str)
	{
		name = str;
	}
	virtual char* GetName()const
	{
		return name;
	}
};

#endif // !_MODULE_H_