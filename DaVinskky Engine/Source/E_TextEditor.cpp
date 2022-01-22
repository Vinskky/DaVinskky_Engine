#include "E_TextEditor.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleEditor.h"
#include "E_MainMenuBar.h"


E_TextEditor::E_TextEditor(const char* name, bool isActive): Editor("Text Editor"),
show_texteditor_window(true)
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

	TextEditorWindow();

	return ret;
}

bool E_TextEditor::CleanUp()
{
	return false;
}

bool E_TextEditor::TextEditorWindow()
{
	if (app->editor->mainMenuPanel->GetTextEditorState() == true)
	{
		if (ImGui::Begin("Text Editor", &show_texteditor_window, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
		{
			//Update
			auto cpos = editor.GetCursorPosition();
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save"))
					{
						std::string textToSave = editor.GetText();

						app->fileSystem->Remove(fileToEdit.c_str());
						app->fileSystem->Save(fileToEdit.c_str(), textToSave.c_str(), editor.GetText().size());

						//glDetachShader(shaderToRecompile->shaderProgramID, shaderToRecompile->vertexID);
						//glDetachShader(shaderToRecompile->shaderProgramID, shaderToRecompile->fragmentID);
						//glDeleteProgram(shaderToRecompile->shaderProgramID);
						//
						//Importer::ShaderImporter::Import(shaderToRecompile->assetsFile.c_str(), shaderToRecompile);
					}

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					bool ro = editor.IsReadOnly();
					if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
						editor.SetReadOnly(ro);
					ImGui::Separator();

					if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
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
	}

	return true;
}
