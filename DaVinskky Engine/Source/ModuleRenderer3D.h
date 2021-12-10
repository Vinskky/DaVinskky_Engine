#ifndef _MODULE_RENDER_3D_H_
#define _MODULE_RENDER_3D_H_

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

class R_Mesh;
class C_Mesh;
class C_Material;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D();
	~ModuleRenderer3D();

	bool Init()override;
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawMesh(C_Mesh* mesh, C_Material* cmaterial);

	void DebugDrawNormals(R_Mesh* mesh);
	void LoadDebugTexture();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;
	
	bool wireframe = false;

	uint debugTextureId;
};

#endif // !_MODULE_RENDER_3D_H_

