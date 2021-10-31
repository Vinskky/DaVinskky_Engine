#ifndef _C_TRANSFORM_H_
#define _C_TRANSFORM_H_

#include "Component.h"
#include "External/MathGeoLib/include/MathGeoLib.h"

//component transform class
class C_Transform: public Component
{
public:
	C_Transform();
	~C_Transform();

	void Enable()override;
	void Update()override;
	void Disable()override;

	float3 GetPosition()const;
	float3 GetRotation()const;
	float3 GetScale()const;

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

private:
	float3 position;
	float3 rotation;
	float3 scale;

};


#endif // !_C_TRANSFORM_H_

