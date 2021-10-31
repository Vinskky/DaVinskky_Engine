#include "C_Transform.h"

C_Transform::C_Transform()
{
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

float3 C_Transform::GetPosition() const
{
	return position;
}

float3 C_Transform::GetRotation() const
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

void C_Transform::SetRotation(float x, float y, float z)
{
	rotation.Set(x, y, z);
}

void C_Transform::SetScale(float x, float y, float z)
{
	scale.Set(x, y, z);
}
