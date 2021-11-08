#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D()
{
	SetName("Camera");
	CalculateViewMatrix();

	x = vec3(1.0f, 0.0f, 0.0f);
	y = vec3(0.0f, 1.0f, 0.0f);
	z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(0.0f, 0.0f, 5.0f);
	reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);
	float speed = 10.0f * dt;
	if(app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 20.0f * dt;

	if(app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= z * speed;
	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += z * speed;

	if (app->input->GetMouseZ() != 0)
	{
		newPos -= z * speed * app->input->GetMouseZ();
	}
	if(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= x * speed;
	if(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += x * speed;

	position += newPos;
	reference += newPos;

	// Mouse motion ----------------
	if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int dx = -app->input->GetMouseXMotion();
			int dy = -app->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			position -= reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				x = rotate(x, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				y = rotate(y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				z = rotate(z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float deltaY = (float)dy * Sensitivity;

				y = rotate(y, deltaY, x);
				z = rotate(z, deltaY, x);

				if (y.y < 0.0f)
				{
					z = vec3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					y = cross(z, x);
				}
			}

			position = reference + z * length(position);
		}
	}
	

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &position, const vec3 &reference, bool rotateAroundReference)
{
	this->position = position;
	this->reference = reference;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	if(!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &spot)
{
	reference = spot;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &movement)
{
	position += movement;
	reference += movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	viewMatrix = mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -dot(x, position), -dot(y, position), -dot(z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}
