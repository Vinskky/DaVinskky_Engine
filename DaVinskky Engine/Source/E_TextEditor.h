#ifndef _E_TEXT_EDITOR_H_
#define _E_TEXT_EDITOR_H_

#include "Editor.h"
#include <string>
#include "External/ImGui/ColorTextEditor/TextEditor.h"

class C_Material;

class E_TextEditor : public Editor
{
public:
	E_TextEditor(const char* name, bool isActive = false);
	~E_TextEditor();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;

	void InitTextEditor(C_Material* cmaterial, bool newShader);

	bool TextEditorWindow();

public:
	bool show_texteditor_window; 
	bool IsNewShader;
	TextEditor editor;
	std::string fileToEdit;

	C_Material* cmaterial;
};


#endif // !_E_TEXT_EDITOR_H_

