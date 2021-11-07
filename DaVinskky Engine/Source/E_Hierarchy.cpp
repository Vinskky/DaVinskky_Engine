#include "E_Hierarchy.h"
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"


E_Hierarchy::E_Hierarchy(const char* name, bool isActive): Editor("Hierachy")
{
}

E_Hierarchy::~E_Hierarchy()
{
}

bool E_Hierarchy::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);

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
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen || ImGuiTreeNodeFlags_OpenOnArrow || ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx(objs->GetName(), flags))
	{
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
