#ifndef _E_MAIN_MENU_BAR_H_
#define _E_MAIN_MENU_BAR_H_

#include "Editor.h"

class E_MainMenuBar : public Editor
{
public:
	E_MainMenuBar(const char* name, bool isActive = true);
	~E_MainMenuBar();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

private:
	update_status FileMenu();
	bool ViewMenu();
	bool HelpMenu();
	bool AboutPopUp();

private:
	bool showcase = false;
	bool aboutWindow = true;
};


#endif // !_E_MAIN_MENU_BAR_H_

