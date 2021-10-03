#ifndef _E_CONSOLE_H_
#define _E_CONSOLE_H_

#include "Editor.h"

class E_Console : public Editor
{
public:
	E_Console(Application* app, const char* name, bool isActive = true);
	~E_Console();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;


};



#endif // !_E_CONSOLE_H_

