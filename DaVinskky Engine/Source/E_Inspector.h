#ifndef _E_INSPECTOR_H_
#define _E_INSPECTOR_H_
#include "Editor.h"
#include "External/MathGeoLib/include/Math/float3.h"

class C_Transform;
class C_Mesh;
class C_Material;
class C_Camera;
class GameObject;

class E_Inspector : public Editor
{
public:
	E_Inspector(const char* name, bool isActive = true);
	~E_Inspector();

	bool Draw(ImGuiIO& io)override;
	bool CleanUp()override;



private:
	void InspectorTransform(C_Transform* comp);
	void InspectorMesh(C_Mesh* comp);
	void InspectorMaterialTexture(C_Material* comp);
	void InspectorCamera(C_Camera* comp);

	void AddComponentComboBox(GameObject* selectedObj);

public:
	float3 position;
	float3 rotation;
	float3 scale;

private:
	int componentType;
};

#endif
