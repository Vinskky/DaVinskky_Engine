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

	float3 GetPosition()const;
	Quat GetRotation()const;
	float3 GetScale()const;

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z, float w);
	void SetScale(float x, float y, float z);

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::TRANSFORM; }

private:
	float3 position;
	Quat rotation;
	float3 scale;

};


#endif // !_C_TRANSFORM_H_

