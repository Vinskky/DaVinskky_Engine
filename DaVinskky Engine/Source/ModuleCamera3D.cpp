#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "GameObject.h"

#define MOVEMENT_SPEED 24.0f
#define ROTATION_SPEED 0.05f
#define ZOOM_SPEED 300.0f

ModuleCamera3D::ModuleCamera3D():Module(),
masterCamera(nullptr),
currentCamera(nullptr),
drawRay(false)
{
	SetName("Camera");

	CreateMasterCamera();
	positionOrigin = float3(10.0f, 40.0f, 10.0f);
	referenceOrigin = float3(0.0f, 0.0f, 0.0f);
	reference = referenceOrigin;
	movementSpeed = MOVEMENT_SPEED;
	rotationSpeed = ROTATION_SPEED;
	zoomSpeed = ZOOM_SPEED;

}

ModuleCamera3D::~ModuleCamera3D()
{
	currentCamera = nullptr;
	masterCamera->Clear();
	RELEASE(masterCamera);
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	masterCamera->GetComponent<C_Transform>()->SetPosition(10.0f,8.0f, 10.0f);
	masterCamera->GetComponent<C_Camera>()->UpdateFrustumTransform();
	LookAt(reference);
	return ret;
}
// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if(app->editor->hoveringEditor)
		return UPDATE_CONTINUE;

	masterCamera->GetComponent<C_Transform>()->GetWorldTransform();

	if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN)
	{
		CastRay();
	}

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		WASDMovement(dt);

	}

	if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_STATE::KEY_REPEAT)
	{
		if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
		{
			Orbit(dt);
			/*if (app->sceneIntro->selectedGameObj != nullptr)
			{
				if (app->sceneIntro->selectedGameObj->GetComponent<C_Camera>() != currentCamera)
				{
					reference = app->sceneIntro->selectedGameObj->GetComponent<C_Transform>()->GetWorldPosition();
				}
			}
			else
			{
				reference = float3::zero;
			}*/
			
		}
	}

	if (app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
	{
		PanCamera(dt);
	}

	if (app->input->GetMouseZ() != 0)
	{
		Zoom(dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_O) == KEY_STATE::KEY_DOWN)
		{
			ReturnToWorldOrigin();
		}
	}

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

void ModuleCamera3D::CreateMasterCamera()
{
	masterCamera = new GameObject();
	masterCamera->SetName("MasterCamera");
	masterCamera->CreateComponent(COMPONENT_TYPE::CAMERA);
	
	C_Camera* cam = masterCamera->GetComponent<C_Camera>();
	cam->SetFarPlaneDistance(1000.0f);
	cam->SetIsCulling(false);
	SetCurrentCamera(cam);

	if (app != nullptr)
	{
		int winWidth;
		int winHeight;
		app->window->GetWindowsSize(app->window->window, winWidth, winHeight);

		cam->SetAspectRatio((float)winWidth / (float)winHeight);
	}
}

C_Camera* ModuleCamera3D::GetCurrentCamera() const
{
	return currentCamera;
}

void ModuleCamera3D::SetCurrentCamera(C_Camera* cam)
{
	if (cam == nullptr)
	{
		LOG("[ERROR] Camera: Could not set a new current camera! Error: Given Camera Component was nullptr.");
		return;
	}

	if (cam->GetOwner() == nullptr)																						// Highly unlikely case. Gets checked just to make sure.
	{
		LOG("[ERROR] Camera: Could not set a new current camera! Error: Given Camera Component's owner was nullptr.");
		return;
	}

	if (cam != nullptr)
	{
		cam->SetFrustumIsHidden(false);
	}

	cam->SetFrustumIsHidden(true);
	currentCamera = cam;
	currentCamera->updateMatrix = true;

	if (app != nullptr)																										// TMP (?)
	{
		int winWidth;
		int winHeight;
		app->window->GetWindowsSize(app->window->window, winWidth, winHeight);

		currentCamera->SetAspectRatio((float)winWidth / (float)winHeight);
	}
}

void ModuleCamera3D::SetMasterAsCurrentCam()
{
	currentCamera->SetFrustumIsHidden(false);

	if (masterCamera == nullptr)
	{
		LOG("[ERROR] Camera: Could not set the master camera as the current camera! Error: Master Camera was nullptr.");
		LOG("[WARNING] Camera: Created a new Master Camera. Reason: Master Camera was nullptr!");

		CreateMasterCamera();
	}

	C_Camera* cam = currentCamera->GetOwner()->GetComponent<C_Camera>();
	if (cam == nullptr)
	{
		LOG("[ERROR] Camera: Could not set the master camera as the current camera! Error: Master Camera did not have a Camera Component.");
		LOG("[WARNING] Camera: Created a new Camera Component for the Master Camera. Reason: Master Camera did not have a Camera Component!");

		currentCamera->GetOwner()->CreateComponent(COMPONENT_TYPE::CAMERA);
	}

	currentCamera = cam;
	currentCamera->updateMatrix = true;
}

void ModuleCamera3D::PointAt(const float3& position, const float3& reference, bool orbit)
{
	currentCamera->PointAt(position, reference);
	this->reference = reference;

	if (!orbit)
	{
		this->reference = position;

		Frustum frustum = currentCamera->GetFrustum();
		currentCamera->SetPosition(frustum.Pos() + frustum.Front() * 0.05f);
	}
}

void ModuleCamera3D::LookAt(const float3& spot)
{
	currentCamera->LookAt(spot);
	reference = spot;
}

void ModuleCamera3D::ReturnToWorldOrigin()
{
	PointAt(positionOrigin, referenceOrigin, true);
}

float3 ModuleCamera3D::GetPosition() const
{
	return currentCamera->GetFrustum().Pos();
}

float3 ModuleCamera3D::GetReference() const
{
	return reference;
}

void ModuleCamera3D::SetPosition(const float3& position)
{
	currentCamera->SetPosition(position);
}

void ModuleCamera3D::SetReference(const float3& reference)
{
	this->reference = reference;
}

float ModuleCamera3D::GetMovementSpeed() const
{
	return movementSpeed;
}

float ModuleCamera3D::GetRotationSpeed() const
{
	return rotationSpeed;
}

float ModuleCamera3D::GetZoomSpeed() const
{
	return zoomSpeed;
}

void ModuleCamera3D::SetMovementSpeed(const float& movementSpeed)
{
	this->movementSpeed = movementSpeed;
}

void ModuleCamera3D::SetRotationSpeed(const float& rotationSpeed)
{
	this->rotationSpeed = rotationSpeed;
}

void ModuleCamera3D::SetZoomSpeed(const float& zoomSpeed)
{
	this->zoomSpeed = zoomSpeed;
}

float3 ModuleCamera3D::GetMasterCameraPosition() const
{
	return masterCamera->GetComponent<C_Transform>()->GetWorldPosition();
}

float3 ModuleCamera3D::GetMasterCameraRotation() const
{
	return masterCamera->GetComponent<C_Transform>()->GetLocalEulerRotation();
}

float3 ModuleCamera3D::GetMasterCameraScale() const
{
	return masterCamera->GetComponent<C_Transform>()->GetWorldScale();
}

void ModuleCamera3D::SetMasterCameraPosition(const float3& position)
{
	masterCamera->GetComponent<C_Transform>()->SetPosition(position.x, position.y, position.z);
}

void ModuleCamera3D::SetMasterCameraRotation(const float3& rotation)
{
	masterCamera->GetComponent<C_Transform>()->SetLocalEulerRotation(rotation);
}

void ModuleCamera3D::SetMasterCameraScale(const float3& scale)
{
	masterCamera->GetComponent<C_Transform>()->SetScale(scale.x, scale.y, scale.z);
}

void ModuleCamera3D::CastRay()
{
	float2 mousePosition;
	mousePosition.x = app->input->GetMouseX();
	mousePosition.y = app->input->GetMouseY();

	int winWidth;
	int winHeight;
	app->window->GetWindowsSize(app->window->window, winWidth, winHeight);

	float normalizedMouseX = mousePosition.x / (float)winWidth;
	float normalizedMouseY = mousePosition.y / (float)winHeight;

	float rayOriginX = (normalizedMouseX - 0.5f) * 2;
	float rayOriginY = (normalizedMouseY - 0.5f) * 2;

	lastRaycast = currentCamera->GetFrustum().UnProjectLineSegment(rayOriginX, rayOriginY);

	//app->sceneIntro->SelectGameObjectThroughRaycast(lastRaycast);
}

bool ModuleCamera3D::DrawLastRaycast() const
{
	return drawRay;
}

void ModuleCamera3D::SetDrawLastRaycast(bool setTo)
{
	drawRay = setTo;
}

void ModuleCamera3D::WASDMovement(float dt)
{
	float3 newPosition = currentCamera->GetOwner()->GetComponent<C_Transform>()->GetPosition();
	Frustum frustum = currentCamera->GetFrustum();
	float movSpeed = movementSpeed * dt;

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)		
	{																			
		movSpeed = movementSpeed * 2 * dt;										
	}																			

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)			
	{																			
		newPosition += frustum.Front() * movSpeed;								
	}																			
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)			
	{																			
		newPosition -= frustum.Front() * movSpeed;								
	}																			


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)			
	{																			
		newPosition -= frustum.WorldRight() * movSpeed;							
	}																			
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)			
	{																			
		newPosition += frustum.WorldRight() * movSpeed;							
	}																			


	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_STATE::KEY_REPEAT)			
	{																			
		newPosition += frustum.Up() * movSpeed;									
	}																			
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_REPEAT)			
	{																			
		newPosition -= frustum.Up() * movSpeed;									
	}
	
	SetPosition(newPosition);

}


void ModuleCamera3D::Orbit(float dt)
{
	Frustum frustum = currentCamera->GetFrustum();
	/*int winWidth;
	int winHeight;
	app->window->GetWindowsSize(app->window->window, winWidth, winHeight);*/

	float2 mouseMotion;
	mouseMotion.x = app->input->GetMouseXMotion() * 10.0f;
	mouseMotion.y = app->input->GetMouseYMotion() * 10.0f;
	LOG("MouseX: %f MouseY: %f", mouseMotion.x, mouseMotion.y);
	float sensitivity = rotationSpeed * dt;

	float3 newZ = frustum.Pos() - reference;

	if (mouseMotion.x != 0.0f)
	{
		Quat newX = Quat(frustum.Up(), -mouseMotion.x * sensitivity);
		newZ = newX.Transform(newZ);
	}

	if (mouseMotion.y != 0.0f)
	{
		Quat new_Y = Quat(frustum.WorldRight(), -mouseMotion.y * sensitivity);
		newZ = new_Y.Transform(newZ);
	}

	float3 newPosition = newZ + reference;

	LOG("NEW POS: { %.3f, %.3f, %.3f }", newPosition.x, newPosition.y, newPosition.z);

	PointAt(newPosition, reference, true);
}

void ModuleCamera3D::PanCamera(float dt)
{
	float3 newX = float3::zero;
	float3 newY = float3::zero;
	float3 newPosition = currentCamera->GetOwner()->GetComponent<C_Transform>()->GetPosition();

	Frustum frustum = currentCamera->GetFrustum();
	float2 mouseMotion;
	mouseMotion.x = app->input->GetMouseXMotion();
	mouseMotion.y = app->input->GetMouseYMotion();

	if (mouseMotion.x != 0)
	{
		newX = -mouseMotion.x * frustum.WorldRight() * dt;
	}

	if (mouseMotion.y != 0)
	{
		newY = mouseMotion.y * frustum.Up() * dt;
	}

	newPosition += newX + newY;

	SetPosition(newPosition);
}

void ModuleCamera3D::Zoom(float dt)
{
	Frustum frustum = currentCamera->GetFrustum();
	float3 newPosition = currentCamera->GetOwner()->GetComponent<C_Transform>()->GetPosition();

	float3 new_Z = frustum.Front() * (float)app->input->GetMouseZ() * zoomSpeed * dt;

	newPosition += new_Z;

	SetPosition(newPosition);
}
