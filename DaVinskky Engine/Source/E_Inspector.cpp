#include "E_Inspector.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

E_Inspector::E_Inspector(const char* name, bool isActive):Editor("Inspector")
{
	position = float3::zero;
	rotation = float3::zero;
	scale = float3::one;
}

E_Inspector::~E_Inspector()
{
}

bool E_Inspector::Draw(ImGuiIO& io)
{
	bool ret = true;

	

	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_None);
	
	//Checks hover to avoid unnecesary inputs
	if (app->editor->hoveringEditor == false)
		app->editor->hoveringEditor = ImGui::IsWindowHovered();

	if (app->sceneIntro->selectedGameObj != nullptr)
	{
		GameObject* tmp = app->sceneIntro->selectedGameObj;

		for (uint i = 0; i < tmp->components.size(); ++i)
		{
			Component* tComp = tmp->components[i];
			switch (tComp->GetType())
			{
			case COMPONENT_TYPE::TRANSFORM: { InspectorTransform((C_Transform*)tComp); }break;
			case COMPONENT_TYPE::MESH: { InspectorMesh((C_Mesh*)tComp); }break;
			case COMPONENT_TYPE::MATERIAL: {InspectorMaterialTexture((C_Material*)tComp); }break;
			default:
				break;
			}
		}
	}

	ImGui::End();
	return ret;
}

bool E_Inspector::CleanUp()
{
	bool ret = true;
	return ret;
}

void E_Inspector::InspectorTransform(C_Transform* comp)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char buffer[128];
		strcpy_s(buffer, comp->GetOwner()->GetName());
		if (ImGui::InputText("Name",buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			comp->GetOwner()->SetName(buffer);
		}
		if (ImGui::DragFloat3("position",(float*) &comp->GetPosition()))
		{

		}
		if (ImGui::DragFloat3("rotation", (float*)&comp->GetRotation()))
		{

		}
		if (ImGui::DragFloat3("scale", (float*)&comp->GetScale()))
		{

		}
	}
}

void E_Inspector::InspectorMesh(C_Mesh* comp)
{
	uint nVertices;
	uint nNormals;
	uint nTexCoords;
	uint nIndices;
	comp->GetMeshData(nVertices, nNormals, nTexCoords, nIndices);

	if (ImGui::CollapsingHeader("Geometry", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool active = comp->IsActive(); 
		if (ImGui::Checkbox("Enable Mesh", &active))
		{
			comp->SetIsActive(active);
		}
		ImGui::Separator();
		ImGui::Text("Triangles: %i",nVertices/3);
		ImGui::Text("Indices: %i",nIndices);
		ImGui::Text("Vertices: %i",nVertices);
		ImGui::Separator();
		//checkers: show vertex && normals.
		bool debug = comp->GetDrawNormals();
		if (ImGui::Checkbox("Draw Normals", &debug))
		{
			comp->SetDrawNormals(debug);
		}
	}
}

void E_Inspector::InspectorMaterialTexture(C_Material* comp)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//check default texture
		bool dftText = comp->UseDefaultTexture();
		if(ImGui::Checkbox("Default Text", &dftText))
		{
			comp->SetDefaultTexture(dftText);
		}
		
	}
}
