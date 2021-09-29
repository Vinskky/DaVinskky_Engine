#ifndef _E_MAIN_MENU_BAR_H_
#define _E_MAIN_MENU_BAR_H_

#include "Editor.h"

class E_MainMenuBar : public Editor
{
public:
	E_MainMenuBar(Application* app, const char* name, bool isActive = true);
	~E_MainMenuBar();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

private:
	bool FileMenu();
	bool ViewMenu();
	bool HelpMenu();

private:
	bool showcase = false;
};


#endif // !_E_MAIN_MENU_BAR_H_

