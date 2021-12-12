#ifndef _C_CAMERA_H_
#define _C_CAMERA_H_

#include "Component.h"
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "External/MathGeoLib/include/Geometry/Frustum.h"
#include "External/MathGeoLib/include/Geometry/Plane.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* owner);
	~C_Camera();

	void Update()override;
	bool CleanUp()override;
	
	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::CAMERA; }

public:
	void InitFrustum();
	void UpdateFrustumTransform();
	Frustum GetFrustum()const;

	float* GetOpenGLViewMatrix();
	float* GetOpenGLProjectionMatrix();

public:
	//Camera details
	void PointAt(const float3& position, const float3& target);
	void LookAt(const float3& target);

	void Rotate(const float3x3& rotationMatrix);
	
	void SetPosition(const float3& position);

public:
	//FRUSTUM CULLING
	void UpdateFrustumPlanes();
	void UpdateFrustumVertices();

	Plane* GetFrustumPlanes()const;
	float3* GetFrustumVertices()const;

	bool FrustumContainsAABB(const AABB& aabb)const;
	bool FrustumIntersectsAABB(const AABB& aabb)const;

public:
	//FRUSTUM SETTINGS
	float GetAspectRatio()const;
	void SetAspectRatio(float aspectRatio);

	float GetNearPlaneDistance()const;
	float GetFarPlaneDistance()const;
	float GetHorizontalFOV()const;
	float GetVerticalFOV()const;

	void SetNearPlaneDistance(float nearDistance);
	void SetFarPlaneDistance(float farDistance);
	void SetHorizontalFOV(float horizontalFOV);
	void SetVerticalFOV(float verticalFOV);

	void GetMinMaxFOV(uint& minFOV, uint& maxFOV)const;
	void SetMinMaxFOV(uint minFOV, uint maxFOV);
public:
	//FLAGS
	bool IsCulling()const;
	bool FrustumIsHidden()const;

	void SetIsCulling(bool setTo);
	void SetFrustumIsHidden(bool setTo);

private:
	Frustum frustum;

	Plane* frustumPlanes;
	float3* frustumVertices;

	uint minFOV;
	uint maxFOV;

	bool isCulling;
	bool hideFrustum;

public:
	bool updateMatrix;
};

#endif // !_C_CAMERA_H_

