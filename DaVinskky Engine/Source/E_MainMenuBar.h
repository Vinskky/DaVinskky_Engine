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

	bool GetAboutWindowState()const;
	bool GetSaveWindowState()const;
	bool GetLoadWindowState()const;


private:
	bool FileMenu();
	bool ViewMenu();
	bool HelpMenu();
	bool CreateMenu();

public:
	bool saveWindow = false;
	bool loadWindow = false;

private:
	bool showcase = false;
	bool aboutWindow = false;
	bool showConsole = true;
	bool showConfig = true;
};


#endif // !_E_MAIN_MENU_BAR_H_

