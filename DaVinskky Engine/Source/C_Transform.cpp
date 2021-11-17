#include "C_Transform.h"


C_Transform::C_Transform(GameObject* owner): Component(owner, COMPONENT_TYPE::TRANSFORM)
{
	position = float3::zero;
	scale = float3::one;
	rotation = Quat::identity;
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

void C_Transform::SetPosition(float x, float y, float z)
{
	position.Set(x, y, z);
}

void C_Transform::SetRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);
}

void C_Transform::SetScale(float x, float y, float z)
{
	scale.Set(x, y, z);
}
