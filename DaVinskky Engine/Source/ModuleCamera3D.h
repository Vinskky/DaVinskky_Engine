#ifndef _MODULE_CAMERA_3D_H_
#define _MODULE_CAMERA_3D_H_

#include "Module.h"
#include "Globals.h"
#include "External/MathGeoLib/include/Geometry/LineSegment.h"
#include "glmath.h"

class GameObject;
class C_Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D();
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	void CreateMasterCamera();
	C_Camera* GetCurrentCamera()const;
	void SetCurrentCamera(C_Camera* cam);
	void SetMasterAsCurrentCam();

public:
	void PointAt(const float3& position, const float3& reference, bool orbit = false);
	void LookAt(const float3& spot);

	void ReturnToWorldOrigin();

public:																				// Camera3D Getters and Setters.
	float3	GetPosition() const;
	float3	GetReference() const;
	void SetPosition(const float3& position);
	void SetReference(const float3& reference);

	float GetMovementSpeed() const;
	float GetRotationSpeed() const;
	float GetZoomSpeed() const;
	void SetMovementSpeed(const float& movementSpeed);
	void SetRotationSpeed(const float& rotationSpeed);
	void SetZoomSpeed(const float& zoomSpeed);

	float3 GetMasterCameraPosition() const;
	float3 GetMasterCameraRotation() const;
	float3 GetMasterCameraScale() const;
	void SetMasterCameraPosition(const float3& position);
	void SetMasterCameraRotation(const float3& rotation);
	void SetMasterCameraScale(const float3& scale);

public:
	void CastRay();
	bool DrawLastRaycast()const;
	void SetDrawLastRaycast(bool setTo);

private:
	void WASDMovement(float dt);

	void Orbit(float dt);
	void PanCamera(float dt);
	void Zoom(float dt);

public:
	GameObject* masterCamera;
	C_Camera* currentCamera;

	float3 reference;

	float3 positionOrigin;
	float3 referenceOrigin;

	float movementSpeed;
	float rotationSpeed;
	float zoomSpeed;

public:
	LineSegment lastRaycast;
	bool drawRay;


};

#endif // !_MODULE_CAMERA_3D_H_

