#ifndef _E_TEST_PANEL_H_
#define _E_TEST_PANEL_H_

#include "Editor.h"
#include <vector>

class ETestPanel : public Editor
{
public:
	ETestPanel(const char* name, bool isActive = true);
	~ETestPanel();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;
private:
	//Specific functions to edit each panel information
	bool TestMenuBar();
	bool TestExamplesBar();

	bool TestHelpMenu();
	bool TestWindowOptionsMenu();
	bool TestWidgetsMenu();

	bool TestButtonsColors();
	bool TestInputMenu();

private:
	bool checkBox = false;
	bool checkBoxA = false;
	bool checkBoxB = false;
	bool checkBoxC = false;
};


#endif // !_E_TEST_PANEL_H_

