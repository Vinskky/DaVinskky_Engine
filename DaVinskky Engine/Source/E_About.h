#ifndef _E_ABOUT_H_
#define _E_ABOUT_H_

#include "Editor.h"

class E_About : public Editor
{
public:
	E_About(const char* name, bool isActive = true);
	~E_About();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

public:
	bool AboutWindow();
};

#endif // !_E_ABOUT_H_
