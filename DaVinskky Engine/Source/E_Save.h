#pragma once
#ifndef _E_SAVE_H_
#define _E_SAVE_H_

#include "Editor.h"

class E_Save : public Editor
{
public:
	E_Save(const char* name, bool isActive = true);
	~E_Save();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

public:
	bool SaveWindow();
};

#endif // !_E_SAVE_H_
