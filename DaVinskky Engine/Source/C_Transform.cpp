#include "C_Transform.h"
#include "C_Camera.h"
#include "Globals.h"
#include "GameObject.h"

#include "External/mmgr/include/mmgr.h"

C_Transform::C_Transform(GameObject* owner): Component(owner, COMPONENT_TYPE::TRANSFORM)
{
	updateWorld = false;
	localTransform = float4x4::identity;
	worldTransform = float4x4::identity;

	localTransform.Decompose(position, rotation, scale);

	localEulerRotation = rotation.ToEulerXYZ();
}

C_Transform::~C_Transform()
{
}

void C_Transform::Enable()
{
}

void C_Transform::Update()
{
}

void C_Transform::Disable()
{
}

bool C_Transform::CleanUp()
{
	return true;
}

float3 C_Transform::GetPosition() const
{
	return position;
}

Quat C_Transform::GetRotation() const
{
	return rotation;
}

float3 C_Transform::GetScale() const
{
	return scale;
}

float3 C_Transform::GetWorldPosition()
{
	if (updateWorld)
		UpdateWorldTransform();

	return worldTransform.TranslatePart();
}

Quat C_Transform::GetWorldRotation() 
{
	if (updateWorld)
		UpdateWorldTransform();
		
	return Quat(worldTransform.RotatePart());
}

float3 C_Transform::GetWorldEulerRotation() 
{
	if (updateWorld)
		UpdateWorldTransform();

	return RadToDeg(worldTransform.RotatePart().ToEulerXYZ());
}

float3 C_Transform::GetWorldScale() 
{
	if (updateWorld)
		UpdateWorldTransform();

	return worldTransform.GetScale();
}

float4x4 C_Transform::GetWorldTransform() 
{
	if (updateWorld)
		UpdateWorldTransform();

	return worldTransform;
}

void C_Transform::SetWorldTransform(const float4x4& worldTransform)
{
	this->worldTransform = worldTransform;

	SyncLocalToWorld();
}

float4x4 C_Transform::GetLocalTransform() const
{
	return localTransform;
}

void C_Transform::SetLocalTransform(const float4x4& localTransform)
{
	this->localTransform = localTransform;
	localTransform.Decompose(position, rotation, scale);
	localEulerRotation = localTransform.RotatePart().ToEulerXYZ();

	UpdateWorldTransform();
}

float3 C_Transform::GetLocalEulerRotation() const
{
	return localEulerRotation;
}

void C_Transform::SetLocalEulerRotation(float3 euler)
{
	this->localEulerRotation =  DegToRad(euler);
	rotation = Quat::FromEulerXYZ(localEulerRotation.x, localEulerRotation.y, localEulerRotation.z);

	UpdateLocalTransform();
}

void C_Transform::SetChildsAsDirty()
{
	GameObject* owner = GetOwner();

	if (owner->children.empty())
		return;

	for (uint i = 0; i < owner->children.size(); ++i)
	{
		C_Transform* childTransform = owner->children[i]->GetComponent<C_Transform>();

		if (childTransform != nullptr)
		{
			childTransform->updateWorld = true;
			childTransform->SetChildsAsDirty();
		}
	}
}

void C_Transform::SetWorldPosition(const float3& newPosition)
{
	worldTransform.SetTranslatePart(newPosition);

	SyncLocalToWorld();
}

void C_Transform::SetWorldRotation(const Quat& newRotation)
{
	worldTransform.SetRotatePart(newRotation);

	SyncLocalToWorld();
}

void C_Transform::SetWorldRotation(const float3& new_rotation)
{
	worldTransform.SetRotatePart(Quat::FromEulerXYZ(new_rotation.x, new_rotation.y, new_rotation.z));

	SyncLocalToWorld();
}

void C_Transform::SetWorldScale(const float3& newScale)
{
	worldTransform.Scale(worldTransform.GetScale().Neg());

	if (newScale.x == 0.0f || newScale.y == 0.0f || newScale.z == 0.0f)
	{
		worldTransform.Scale(float3(0.1f, 0.1f, 0.1f));
	}
	else
	{
		worldTransform.Scale(newScale);
	}

	SyncLocalToWorld();
}

void C_Transform::UpdateLocalTransform()
{
	localTransform = float4x4::FromTRS(position, rotation, scale);

	updateWorld = true;
}

void C_Transform::UpdateWorldTransform()
{
	GameObject* owner = GetOwner();

	worldTransform = (owner->parent != nullptr) ? owner->parent->GetComponent<C_Transform>()->worldTransform * localTransform : localTransform;

	SetChildsAsDirty();

	updateWorld = false;

	C_Camera* c_camera = owner->GetComponent<C_Camera>();
	if (c_camera != nullptr)
	{
		c_camera->UpdateFrustumTransform();
	}
}

void C_Transform::SyncLocalToWorld()
{
	GameObject* owner = GetOwner();

	localTransform = (owner->parent != nullptr) ? owner->parent->GetComponent<C_Transform>()->worldTransform.Inverted() * worldTransform : worldTransform;

	SetLocalTransform(localTransform);

	SetChildsAsDirty();

	C_Camera* cCamera = owner->GetComponent<C_Camera>();
	if (cCamera != nullptr)
	{
		cCamera->UpdateFrustumTransform();
	}

}

void C_Transform::SetPosition(float x, float y, float z)
{
	position.Set(x, y, z);
	UpdateLocalTransform();
}

void C_Transform::SetRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);
	localEulerRotation = rotation.ToEulerXYZ();

	UpdateLocalTransform();
}

void C_Transform::SetScale(float x, float y, float z)
{
	scale.x = (x != 0.0f) ? x : 0.5f;
	scale.y = (y != 0.0f) ? y : 0.5f;
	scale.z = (z != 0.0f) ? z : 0.5f;
	
	UpdateLocalTransform();
}
