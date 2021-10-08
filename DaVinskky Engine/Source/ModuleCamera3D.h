#ifndef _MODULE_CAMERA_3D_H_
#define _MODULE_CAMERA_3D_H_

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D();
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3& position, const vec3& reference, bool rotateAroundReference = false);
	void LookAt(const vec3& spot);
	void Move(const vec3& movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:

	vec3 x, y, z, position, reference;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};

#endif // !_MODULE_CAMERA_3D_H_

