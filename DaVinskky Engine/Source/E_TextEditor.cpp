#include "E_TextEditor.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleEditor.h"
#include "E_Inspector.h"
#include "C_Material.h"
#include "I_Shader.h"

#include <fstream>


E_TextEditor::E_TextEditor(const char* name, bool isActive) : Editor("Text Editor"),
show_texteditor_window(false), IsNewShader(false)
{
}

E_TextEditor::~E_TextEditor()
{
}

bool E_TextEditor::Draw(ImGuiIO& io)
{
	bool ret = true;

	if (app->editor->hoveringEditor == false)
		app->editor->hoveringEditor = ImGui::IsWindowHovered();

	if (show_texteditor_window)
		TextEditorWindow();

	return ret;
}

bool E_TextEditor::CleanUp()
{
	return false;
}

void E_TextEditor::InitTextEditor(C_Material* cmaterial, bool newShader)
{
	this->cmaterial = cmaterial;
	IsNewShader = newShader;

	if (IsNewShader)
	{
		fileToEdit = std::string(ASSETS_SHADERS_PATH + std::string("newShader.DaVShader"));
		editor.SetText("");
	}
	else
	{
		fileToEdit = this->cmaterial->GetShaderPath();

		// Only Handles GLSL
		TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::GLSL();
		editor.SetShowWhitespaces(false);

		std::ifstream t(fileToEdit.c_str());
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str);
		}
	}

	show_texteditor_window = true;
}

bool E_TextEditor::TextEditorWindow()
{
	if (ImGui::Begin("Text Editor", &show_texteditor_window, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::Button("COMPILE", ImVec2(100,50)))
		{
			std::string textToSave = editor.GetText();
			if (Importer::Shader::CompileText(textToSave))
			{
				app->fileSystem->Save(fileToEdit.c_str(), textToSave.c_str(), editor.GetText().size());

				Importer::Shader::Recompile(cmaterial->GetShader());
			}
			else
				LOG("There are some errors with the Shader! %s", fileToEdit.c_str());
		}
		if (IsNewShader)
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();

			std::string shaderName;
			char buffer[128];
			strcpy_s(buffer, fileToEdit.c_str());
			if (ImGui::InputText("Name (.DaVShader)", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				shaderName = buffer;
				fileToEdit = shaderName;
			}
			ImGui::Spacing();
			ImGui::Spacing();
		}
		// Update
		auto cpos = editor.GetCursorPosition();
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr, !ro && editor.CanUndo()))
					editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
					editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
					editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
					editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
					editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) //Might not be necessary? 
			{
				if (ImGui::MenuItem("Dark palette"))
					editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					editor.SetPalette(TextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), fileToEdit.c_str());

		editor.Render("TextEditor");
		ImGui::End();
	}

	return true;
}
