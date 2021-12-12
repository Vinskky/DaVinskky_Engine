#pragma once
#ifndef _E_LOAD_H_
#define _E_LOAD_H_

#include "Editor.h"

class E_Load : public Editor
{
public:
	E_Load(const char* name, bool isActive = true);
	~E_Load();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

public:
	bool LoadWindow();
};

#endif // !_E_LOAD_H_
