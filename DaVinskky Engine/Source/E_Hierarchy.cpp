#include "E_Hierarchy.h"
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include <vector>

E_Hierarchy::E_Hierarchy(const char* name, bool isActive): Editor("Hierachy")
{
	payload = "DraggedNode";
}

E_Hierarchy::~E_Hierarchy()
{
}

bool E_Hierarchy::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
	
	//Checks hover to avoid unnecesary inputs
	if (app->editor->hoveringEditor == false)
		app->editor->hoveringEditor = ImGui::IsWindowHovered();
	
	DrawGameObjects(app->sceneIntro->sceneRoot);

	ImGui::End();
	return ret;
}

bool E_Hierarchy::CleanUp()
{
	bool ret = true;


	return ret;
}

void E_Hierarchy::DrawGameObjects(GameObject* rootGO)
{
	if (rootGO == nullptr)
	{
		LOG("Error Drawing herarchy gameObjects. Root Game Object is null!");
		return;
	}
	ProcessRoot(rootGO);
}

void E_Hierarchy::ProcessRoot(GameObject* objs)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;;
	
	if (objs->children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (app->sceneIntro->selectedGameObj != nullptr && objs == app->sceneIntro->selectedGameObj)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (app->sceneIntro->sceneRoot != nullptr && objs == app->sceneIntro->sceneRoot)
	{
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}

	if (ImGui::TreeNodeEx(objs->GetName(), flags))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			app->sceneIntro->selectedGameObj = objs;
		}

		DragAndDrop(objs);

		if (!objs->children.empty())
		{
			for (uint i = 0; i < objs->children.size(); ++i)
			{
				ProcessRoot(objs->children[i]);
			}
		}
		ImGui::TreePop();
	}
}

void E_Hierarchy::DragAndDrop(GameObject* objs)
{
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload(payload, objs, sizeof(GameObject));
		ImGui::Text("Dragging %s",objs->GetName());
		draggedObj = objs;
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* tmp = ImGui::AcceptDragDropPayload(payload, ImGuiDragDropFlags_None))
		{
			if (draggedObj != nullptr)
			{
				bool ret = false;

				//deleting from child list dragged object 
				draggedObj->parent->DeleteChild(draggedObj);

				//reasign parent to dragged object 
				//SetParent already adds to its child list the new object added.
				draggedObj->SetParent(objs);
				draggedObj = nullptr;
			}
		}
		ImGui::EndDragDropTarget();
	}
}
