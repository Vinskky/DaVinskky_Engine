#include "Application.h"
#include "ModuleRenderer3D.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "GameObject.h"

#include "External/mmgr/include/mmgr.h"

#define NUM_FRUSTUM_PLANES 6
#define NUM_FRUSTUM_VERTICES 8

#define MIN_FOV 40
#define MAX_FOV 120


C_Camera::C_Camera(GameObject* owner):Component(owner, COMPONENT_TYPE::CAMERA),
frustumPlanes(nullptr),
frustumVertices(nullptr),
minFOV(MIN_FOV),
maxFOV(MAX_FOV),
isCulling(false),
hideFrustum(false),
updateMatrix(false)
{
	frustumPlanes = new Plane[NUM_FRUSTUM_PLANES];
	frustumVertices = new float3[NUM_FRUSTUM_VERTICES];
	InitFrustum();
}

C_Camera::~C_Camera()
{
	RELEASE_ARRAY(frustumPlanes);
	RELEASE_ARRAY(frustumVertices);
}

void C_Camera::Update()
{
	this->GetOwner()->transform->GetWorldTransform();
	if (hideFrustum)
		app->renderer3D->DrawCuboid(this->frustumVertices, Color(0.0f, 0.0f, 1.0f, 1.0f));
}

bool C_Camera::CleanUp()
{
	bool ret = true;

	if (isCulling)
	{
		//App->Scene... Remove culling camera
	}

	return ret;
}

void C_Camera::InitFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(1.0f, 300.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();
}

void C_Camera::UpdateFrustumTransform()
{
	float4x4 worldTransform = this->GetOwner()->GetComponent<C_Transform>()->GetWorldTransform();
	float3x4 worldMatrix = float3x4::identity;

	worldMatrix.SetTranslatePart(worldTransform.TranslatePart());
	worldMatrix.SetRotatePart(worldTransform.RotatePart());
	worldMatrix.Scale(worldTransform.GetScale());

	frustum.SetWorldMatrix(worldMatrix);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	updateMatrix = true;
}

Frustum C_Camera::GetFrustum() const
{
	return frustum;
}

float* C_Camera::GetOpenGLViewMatrix()
{
	static float4x4 viewMatrix;

	viewMatrix = frustum.ViewMatrix();
	viewMatrix.Transpose();

	return (float*)viewMatrix.v;
}

float* C_Camera::GetOpenGLProjectionMatrix()
{
	static float4x4 projectionMatrix;

	projectionMatrix = frustum.ProjectionMatrix().Transposed();

	return (float*)projectionMatrix.v;
}

void C_Camera::PointAt(const float3& position, const float3& target)
{
	SetPosition(position);
	LookAt(target);
}

void C_Camera::LookAt(const float3& target)
{
	float3 newZ = float3(target - frustum.Pos()).Normalized();											// Constructing the new forward vector of the camera.
	float3x3 lookAtMatrix = float3x3::LookAt(frustum.Front(), newZ, frustum.Up(), float3::unitY);				// Using the LookAt() method built in MathGeoLib to generate the mat.

	frustum.SetFront(lookAtMatrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(lookAtMatrix.MulDir(frustum.Up()).Normalized());

	float4x4 worldMatrix = frustum.WorldMatrix();
	this->GetOwner()->GetComponent<C_Transform>()->SetWorldTransform(worldMatrix);
}

void C_Camera::Rotate(const float3x3& rotationMatrix)
{
	float4x4 worldMatrix = frustum.WorldMatrix();

	worldMatrix.SetRotatePart(rotationMatrix);

	this->GetOwner()->GetComponent<C_Transform>()->SetWorldTransform(worldMatrix);
}

void C_Camera::SetPosition(const float3& position)
{
	this->GetOwner()->GetComponent<C_Transform>()->SetWorldPosition(position);
}

bool C_Camera::IsCulling() const
{
	return this->isCulling;
}

bool C_Camera::FrustumIsHidden() const
{
	return hideFrustum;
}

void C_Camera::SetIsCulling(bool setTo)
{
	this->isCulling = setTo;
}

void C_Camera::SetFrustumIsHidden(bool setTo)
{
	this->hideFrustum = setTo;
}

void C_Camera::UpdateFrustumPlanes()
{
	frustum.GetPlanes(frustumPlanes);
}

void C_Camera::UpdateFrustumVertices()
{
	frustum.GetCornerPoints(frustumVertices);
}

Plane* C_Camera::GetFrustumPlanes() const
{
	return frustumPlanes;
}

float3* C_Camera::GetFrustumVertices() const
{
	return frustumVertices;
}

bool C_Camera::FrustumContainsAABB(const AABB& aabb) const
{
	float3 aabbVertices[8];
	aabb.GetCornerPoints(aabbVertices);

	uint planesContainingAllVerts = 0;                                                                    // Can be used to check whether or not the geometric figure is intersected.

	for (uint p = 0; p < 6; ++p)
	{
		bool allVertsAreIn = true;
		uint vertsInPlane = 8;

		for (uint v = 0; v < 8; ++v)
		{
			if (frustumPlanes[p].IsOnPositiveSide(aabbVertices[v]))                                        // IsOnPositiveSide() returns true if the given point is behind the frustum plane.
			{
				allVertsAreIn = false;
				--vertsInPlane;
			}
		}

		if (vertsInPlane == 0)
		{
			return false;
		}

		if (allVertsAreIn)
		{
			++planesContainingAllVerts;
		}
	}

	if (planesContainingAllVerts == 6)
	{
		return true;
	}

	return false;
}

bool C_Camera::FrustumIntersectsAABB(const AABB& aabb) const
{
	float3 aabbVertices[8];
	aabb.GetCornerPoints(aabbVertices);

	for (uint p = 0; p < 6; ++p)
	{
		uint vertsInPlane = 8;

		for (uint v = 0; v < 8; ++v)
		{
			if (frustumPlanes[p].IsOnPositiveSide(aabbVertices[v]))
			{
				--vertsInPlane;
			}
		}

		if (vertsInPlane == 0)
		{
			return false;
		}
	}

	return true;
}

float C_Camera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void C_Camera::SetAspectRatio(float aspectRatio)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectRatio);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	updateMatrix = true;
}

float C_Camera::GetNearPlaneDistance() const
{
	return frustum.NearPlaneDistance();
}

float C_Camera::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

float C_Camera::GetHorizontalFOV() const
{
	return RadToDeg(frustum.HorizontalFov());
}

float C_Camera::GetVerticalFOV() const
{
	return RadToDeg(frustum.VerticalFov());
}

void C_Camera::SetNearPlaneDistance(float nearDistance)
{
	if (nearDistance < 0)
	{
		LOG("Component Camera: Cannot set the new distance of the near plane! Error: New near distance < 0.");
		return;
	}

	if (nearDistance > frustum.FarPlaneDistance())
	{
		LOG("Component Camera: Cannot set the new distance of the near plane! Error: New near distance > far distance.");
		return;
	}

	frustum.SetViewPlaneDistances(nearDistance, frustum.FarPlaneDistance());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	updateMatrix = true;
}

void C_Camera::SetFarPlaneDistance(float farDistance)
{
	if (farDistance < 0)
	{
		LOG("Component Camera: Cannot set the new distance of the far plane! Error: New far distance < 0.");
		return;
	}

	if (farDistance < frustum.NearPlaneDistance())
	{
		LOG("Component Camera: Cannot set the new distance of the far plane! Error: New far distance < near distance");
		return;
	}

	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), farDistance);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	updateMatrix = true;
}

void C_Camera::SetHorizontalFOV(float horizontalFOV)
{
	frustum.SetHorizontalFovAndAspectRatio((DegToRad(horizontalFOV)), frustum.AspectRatio());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	updateMatrix = true;
}

void C_Camera::SetVerticalFOV(float verticalFOV)
{
	frustum.SetVerticalFovAndAspectRatio((DegToRad(verticalFOV)), frustum.AspectRatio());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	updateMatrix = true;
}

void C_Camera::GetMinMaxFOV(uint& minFOV, uint& maxFOV) const
{
	minFOV = this->minFOV;
	maxFOV = this->maxFOV;
}

void C_Camera::SetMinMaxFOV(uint minFOV, uint maxFOV)
{
	this->minFOV = minFOV;
	this->maxFOV = maxFOV;
}
