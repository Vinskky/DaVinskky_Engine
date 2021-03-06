#include "E_Inspector.h"
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "I_Shader.h"
#include "E_TextEditor.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "R_Shader.h"

#include "External/mmgr/include/mmgr.h"

E_Inspector::E_Inspector(const char* name, bool isActive):Editor("Inspector"),
componentType(0)
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
			case COMPONENT_TYPE::CAMERA: {InspectorCamera((C_Camera*)tComp); }break;
			default:
				break;
			}
		}
		
		ImGui::Separator();

		AddComponentComboBox(tmp);
	}

	ImGui::End();
	return ret;
}

bool E_Inspector::CleanUp()
{
	bool ret = true;
	return ret;
}

void E_Inspector::FillShaderList()
{
	if (shaderList.empty())
	{
		app->fileSystem->GetAllFilesWithExtension(ASSETS_SHADERS_PATH, "DaVShader", shaderList);
	}
	else
	{
		shaderList.clear();
		app->fileSystem->GetAllFilesWithExtension(ASSETS_SHADERS_PATH, "DaVShader", shaderList);
	}
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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
		if (comp->GetOwner() != app->sceneIntro->sceneRoot)
		{
			if (ImGui::Button("Delete"))
			{
				GameObject* go = comp->GetOwner();

				app->sceneIntro->selectedGameObj = app->sceneIntro->sceneRoot;

				go->Clear();
				app->sceneIntro->DeleteGameObjectFromVector(go);

				RELEASE(go);
			}
			
		}
		
		ImGui::PopStyleColor();

		if (ImGui::Button("Add Children"))
		{
			comp->GetOwner()->CreateEmptyChild(comp->GetOwner());
		}

		float3 position = comp->GetPosition();
		if (ImGui::DragFloat3("Position", (float*)&position, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
		{
			comp->SetPosition(position.x, position.y, position.z);
		}

		float3 rotation = RadToDeg(comp->GetLocalEulerRotation());
		if (ImGui::DragFloat3("Rotation", (float*)&rotation, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
		{
			comp->SetLocalEulerRotation(rotation);
		}

		float3 scale = comp->GetScale();
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
		{
			comp->SetScale(scale.x, scale.y, scale.z);
		}
	}
}

void E_Inspector::InspectorMesh(C_Mesh* comp)
{
	if (comp->GetMesh() != nullptr)
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

			ImGui::Text("Triangles: %i", nVertices / 3);
			ImGui::Text("Indices: %i", nIndices);
			ImGui::Text("Vertices: %i", nVertices);

			ImGui::Separator();

			//checkers: show vertex && normals.
			bool debug = comp->GetDrawNormals();
			if (ImGui::Checkbox("Draw Normals", &debug))
			{
				comp->SetDrawNormals(debug);
			}

			ImGui::Separator();
			bool drawBoundingBox = comp->GetOwner()->showBoundingBoxes;
			if (ImGui::Checkbox("Draw BoundingBox", &drawBoundingBox))
			{
				comp->GetOwner()->showBoundingBoxes = drawBoundingBox;
			}
		}
	}
}

void E_Inspector::InspectorMaterialTexture(C_Material* comp)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (comp->GetTexture() != nullptr)
		{
			ImGui::Text("Texture Path: %s", comp->GetTexturePath());
		}

		//check default texture
		bool dftText = comp->UseDefaultTexture();
		if(ImGui::Checkbox("Default Text", &dftText))
		{
			comp->SetDefaultTexture(dftText);
		}

		if (comp->GetShader() != nullptr)
		{
			ImGui::Separator();

			FillShaderList();

			if (ImGui::BeginCombo("Shaders", comp->GetShaderPath(), ImGuiComboFlags_PopupAlignLeft))
			{
				for (int i = 0; i < shaderList.size(); ++i)
				{
					if (ImGui::Selectable(shaderList[i].c_str()))
					{
						//Reset path of shader and recompile it
						std::string path(ASSETS_SHADERS_PATH + shaderList[i]);
						comp->SetShaderPath(path.c_str());
						Importer::Shader::Recompile(comp->GetShader());
					}
				}
				ImGui::EndCombo();
			}
			
			if (ImGui::Button("Edit Shader"))
			{
				app->editor->textEdit->InitTextEditor(comp, false);
			}
			if (ImGui::Button("Create Shader"))
			{
				app->editor->textEdit->InitTextEditor(comp, true);
			}

			ImGui::Separator();
			R_Shader* rshader = comp->GetShader();
			for (uint i = 0; i < rshader->uniforms.size(); i++)
			{
				switch (rshader->uniforms[i].uniformType)
				{
				case	UNIFORM_TYPE::INT:			ImGui::DragInt(rshader->uniforms[i].name.c_str(), &rshader->uniforms[i].integer, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::FLOAT:		ImGui::DragFloat(rshader->uniforms[i].name.c_str(), &rshader->uniforms[i].floatNumber, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::INT_VEC2:		ImGui::DragInt2(rshader->uniforms[i].name.c_str(), (int*)&rshader->uniforms[i].vec2, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::INT_VEC3:		ImGui::DragInt3(rshader->uniforms[i].name.c_str(), (int*)&rshader->uniforms[i].vec3, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::INT_VEC4:		ImGui::DragInt4(rshader->uniforms[i].name.c_str(), (int*)&rshader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::FLOAT_VEC2:	ImGui::DragFloat2(rshader->uniforms[i].name.c_str(), (float*)&rshader->uniforms[i].vec2, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::FLOAT_VEC3:	ImGui::DragFloat3(rshader->uniforms[i].name.c_str(), (float*)&rshader->uniforms[i].vec3, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::FLOAT_VEC4:	ImGui::DragFloat4(rshader->uniforms[i].name.c_str(), (float*)&rshader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				case	UNIFORM_TYPE::MATRIX4:		ImGui::DragFloat4(rshader->uniforms[i].name.c_str(), rshader->uniforms[i].matrix4.ToEulerXYZ().ptr(), 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
				}
			}
		}
	}
}

void E_Inspector::InspectorCamera(C_Camera* comp)
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (comp != nullptr)
		{
			bool cameraIsActive = comp->IsActive();
			if (ImGui::Checkbox("Active", &cameraIsActive))
			{
				comp->SetIsActive(cameraIsActive);
			}

			ImGui::Separator();

			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Camera Flags:");

			bool cameraIsCulling = comp->IsCulling();
			if (ImGui::Checkbox("Culling", &cameraIsCulling))
			{
				comp->SetIsCulling(cameraIsCulling);
			}

			bool frustumIsHidden = comp->FrustumIsHidden();
			if (ImGui::Checkbox("Show Frustum", &frustumIsHidden))
			{
				comp->SetFrustumIsHidden(frustumIsHidden);
			}

			ImGui::Separator();

			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Frustum Settings:");

			float nearPlaneDistance = comp->GetNearPlaneDistance();
			if (ImGui::SliderFloat("Near Plane", &nearPlaneDistance, 0.1f, 1000.0f, "%.3f", 0))
			{
				comp->SetNearPlaneDistance(nearPlaneDistance);
			}

			float farPlaneDistance = comp->GetFarPlaneDistance();
			if (ImGui::SliderFloat("Far Plane", &farPlaneDistance, 0.1f, 1000.0f, "%.3f", 0))
			{
				comp->SetFarPlaneDistance(farPlaneDistance);
			}

			int fov = (int)comp->GetVerticalFOV();
			uint minFov = 0;
			uint maxFov = 0;
			comp->GetMinMaxFOV(minFov, maxFov);
			if (ImGui::SliderInt("FOV", &fov, minFov, maxFov, "%d"))
			{
				comp->SetVerticalFOV((float)fov);
			}

			ImGui::Separator();

			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Camera Selection:");

			if (ImGui::Button("Set as Current Camera"))
			{
				app->camera->SetCurrentCamera(comp);
			}

			if (ImGui::Button("Return to Master Camera"))
			{
				app->camera->SetMasterAsCurrentCam();
			}

		}
	}
}

void E_Inspector::AddComponentComboBox(GameObject* selectedObj)
{
	ImGui::Combo("##", &componentType, "Add Component\0Transform\0Mesh\0Material\0Camera");

	ImGui::SameLine();

	if ((ImGui::Button("ADD")))
	{
		if (componentType != (int)COMPONENT_TYPE::NONE && componentType != (int)COMPONENT_TYPE::MESH)
		{
			selectedObj->CreateComponent((COMPONENT_TYPE)componentType);
		}
	}
}
