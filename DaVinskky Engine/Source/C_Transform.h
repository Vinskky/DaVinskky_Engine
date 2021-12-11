#ifndef _C_TRANSFORM_H_
#define _C_TRANSFORM_H_

#include "Component.h"
#include "External/MathGeoLib/include/MathGeoLib.h"

//component transform class
class C_Transform: public Component
{
public:
	C_Transform(GameObject* owner);
	~C_Transform();

	void Enable()override;
	void Update()override;
	void Disable()override;
	bool CleanUp()override;

	float3 GetPosition()const;
	Quat GetRotation()const;
	float3 GetScale()const;

	//World transforms
	float3 GetWorldPosition();
	Quat GetWorldRotation();
	float3 GetWorldEulerRotation();
	float3 GetWorldScale();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z, float w);
	void SetScale(float x, float y, float z);

	float4x4 GetWorldTransform();
	void SetWorldTransform(const float4x4& worldTransform);

	float4x4 GetLocalTransform()const;
	void SetLocalTransform(const float4x4& localTransform);

	float3 GetLocalEulerRotation()const;
	void SetLocalEulerRotation(float3 euler);
	
	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::TRANSFORM; }

	void SetChildsAsDirty();

private:
	void UpdateLocalTransform();
	void UpdateWorldTransform();
	void SyncLocalToWorld();


private:
	float3 position;
	Quat rotation;
	float3 scale;

	float4x4 localTransform;
	float4x4 worldTransform;

	float3 localEulerRotation;

	bool updateWorld; //dirty flag to update transforms.

};


#endif // !_C_TRANSFORM_H_

