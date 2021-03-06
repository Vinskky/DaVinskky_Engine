#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "I_Texture.h"
#include "I_Shader.h"
#include "R_Mesh.h"
#include "R_Shader.h"
#include "C_Material.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "C_Mesh.h"
#include "GameObject.h"
#include "External/Glew/include/glew.h"
#include "External\SDL\include\SDL_opengl.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "External/mmgr/include/mmgr.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D():Module(),
wireframe(false),
context(NULL),
debugTextureId(0)
{
	SetName("Renderer");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Create context
	context = SDL_GL_CreateContext(app->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		GLenum err = glewInit();

		if (err != GLEW_OK)
		{
			LOG("Glew Init Error: %s\n", glewGetErrorString(err));
		}
		else
		{
			LOG("Using Glew %s", glewGetString(GLEW_VERSION));
			LOG("Vendor: %s", glGetString(GL_VENDOR));
			LOG("Renderer: %s", glGetString(GL_RENDERER));
			LOG("OpenGL version supported %s", glGetString(GL_VERSION));
			LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		// Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		// Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		// Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize();

	Importer::Texture::Init();
	LoadDebugTexture();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	C_Camera* currentCam = app->camera->GetCurrentCamera();
	if (currentCam != nullptr)
	{
		if (currentCam->updateMatrix)
		{
			RecalculateProjectionMatrix();
			currentCam->updateMatrix = false;
		}
	}

	glLoadMatrixf((GLfloat*)app->camera->GetCurrentCamera()->GetOpenGLViewMatrix());

	// light 0 on cam pos
	float3 camPosition = (app->camera->GetCurrentCamera() != nullptr) ? app->camera->GetPosition() : float3(0.0f, 20.0f, 0.0f);
	lights[0].SetPos(camPosition.x, camPosition.y, camPosition.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	update_status ret = update_status::UPDATE_CONTINUE;

	DrawWorldGrid(64);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		wireframe = !wireframe;

		LOG("Wireframe:  %s", wireframe ? "Activated" : "Deactivated");
		
		if (wireframe) // Activate wireframe mode
		{
			// Turn on wiremode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			// Turn off wiremode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	
	return ret;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(app->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	Importer::Texture::CleanUp();

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize()
{
	int winWidth;
	int winHeight;
	
	app->window->GetWindowsSize(app->window->window, winWidth, winHeight);

	glViewport(0, 0, (GLsizei)winWidth, (GLsizei)winHeight);

	if (app->camera->GetCurrentCamera() != nullptr)
	{
		app->camera->GetCurrentCamera()->SetAspectRatio((float)winWidth / (float)winHeight);
	}
	else
	{
		LOG("Renderer 3D: Could not recalculate aspect ratio!! Error: Current camera was nullptr");
	}

	RecalculateProjectionMatrix();
}

void ModuleRenderer3D::DrawMesh(C_Mesh* mesh, C_Material* cmaterial)
{
	R_Mesh* rmesh = mesh->GetMesh();
	if (rmesh == nullptr)
	{
		LOG("Error, Renderer 3D: Could not render Mesh, Mesh* was nullptr");
		return;
	}

	//glPushMatrix();
	
	//glMultMatrixf((GLfloat*) mesh->GetOwner()->GetComponent<C_Transform>()->GetWorldTransform().Transposed().ptr());

	if (cmaterial != nullptr)
	{
		if (!cmaterial->IsActive())
		{
			glDisable(GL_TEXTURE_2D);
		}
		if (cmaterial->GetTexture() == nullptr)                                                        // If the Material Component does not have a Texture Resource.
		{
			Color color = cmaterial->GetMaterialColour();
			glColor4f(color.r, color.g, color.b, color.a);                                                // Apply the diffuse color to the mesh.
		}
		else if (cmaterial->UseDefaultTexture())                                                        // If the default texture is set to be used (bool use_default_texture)
		{
			glBindTexture(GL_TEXTURE_2D, debugTextureId);                            // Binding the texture that will be rendered. Index = 0 means we are clearing the binding.
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, cmaterial->GetTextureID());                                    // Binding the texture_id in the Texture Resource of the Material Component.
		}

		cmaterial->GetShader()->UseShader();

		cmaterial->GetTextureID() ? cmaterial->GetShader()->SetUniform1i("hasTexture", (GLint)true) : cmaterial->GetShader()->SetUniform1i("hasTexture", (GLint)false);
		cmaterial->GetShader()->SetUniformVec4f("inColor", (GLfloat*)&cmaterial->GetMaterialColour());
		cmaterial->GetShader()->SetUniformMatrix4("model_matrix", mesh->GetOwner()->GetComponent<C_Transform>()->GetWorldTransform().Transposed().ptr());
		cmaterial->GetShader()->SetUniformMatrix4("view", app->camera->masterCamera->GetComponent<C_Camera>()->GetOpenGLViewMatrix());
		cmaterial->GetShader()->SetUniformMatrix4("projection", app->camera->masterCamera->GetComponent<C_Camera>()->GetOpenGLProjectionMatrix());
		cmaterial->GetShader()->SetUniformMatrix4("time", (GLfloat*)&app->sceneIntro->gameTime);

		Importer::Shader::SetShaderUniforms(cmaterial->GetShader());
	}

	glBindVertexArray(rmesh->VAO);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, rmesh->TBO);
	//glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

	//glBindBuffer(GL_ARRAY_BUFFER, rmesh->NBO);
	//glNormalPointer(GL_FLOAT, 0, nullptr);

	//glBindBuffer(GL_ARRAY_BUFFER, rmesh->VBO);
	//glVertexPointer(3, GL_FLOAT, 0, nullptr);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rmesh->IBO);
	glDrawElements(GL_TRIANGLES, rmesh->mIndex.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glUseProgram(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Debug Draw Normals
	if (mesh->GetDrawNormals())
	{
		DebugDrawNormals(rmesh);
	}
	//glPopMatrix();
}

void ModuleRenderer3D::DebugDrawNormals(R_Mesh* mesh)
{
	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < mesh->mVertices.size(); i += 3)
	{
		glVertex3f(mesh->mVertices[i], mesh->mVertices[i + 1], mesh->mVertices[i + 2]);
		glVertex3f(mesh->mVertices[i] + mesh->mNormals[i], mesh->mVertices[i + 1] + mesh->mNormals[i + 1], mesh->mVertices[i + 2] + mesh->mNormals[i + 2]);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
}

void ModuleRenderer3D::LoadDebugTexture()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &debugTextureId);
	glBindTexture(GL_TEXTURE_2D, debugTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}

void ModuleRenderer3D::RecalculateProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (app->camera->GetCurrentCamera() != nullptr)
	{
		glLoadMatrixf((GLfloat*)app->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix());
	}
	else
	{
		LOG("Renderer 3D: Could not recalculate aspect ratio!! Error: Current camera was nullptr");
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawWorldGrid(int nGrids)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	float destination = (float)nGrids;

	for (float origin = -destination; origin <= destination; origin += 1.0f)
	{
		glVertex3f(origin, 0.0f, -destination);
		glVertex3f(origin, 0.0f, destination);
		glVertex3f(-destination, 0.0f, origin);
		glVertex3f(destination, 0.0f, origin);
	}

	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ModuleRenderer3D::DrawCuboid(float3* vertex, Color color)
{
	// For a Cuboid with vertices ABCDEFGH
	GLfloat* A = (GLfloat*)&vertex[0];
	GLfloat* B = (GLfloat*)&vertex[1];
	GLfloat* C = (GLfloat*)&vertex[2];
	GLfloat* D = (GLfloat*)&vertex[3];
	GLfloat* E = (GLfloat*)&vertex[4];
	GLfloat* F = (GLfloat*)&vertex[5];
	GLfloat* G = (GLfloat*)&vertex[6];
	GLfloat* H = (GLfloat*)&vertex[7];

	glColor4f(color.r, color.g, color.b, color.a);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	// --- FRONT
	glVertex3fv(A);                                            // BOTTOM HORIZONTAL                                        // Firstly the Near Plane is constructed.
	glVertex3fv(B);                                            // -----------------
	glVertex3fv(D);                                            // TOP HORIZONTAL
	glVertex3fv(C);                                            // -------------

	glVertex3fv(B);                                            // LEFT VERTICAL
	glVertex3fv(D);                                            // -------------
	glVertex3fv(C);                                            // RIGHT VERTICAL
	glVertex3fv(A);                                            // --------------

	// --- BACK
	glVertex3fv(F);                                            // BOTTOM HORIZONTAL                                        // Secondly the Far Plane is constructed.
	glVertex3fv(E);                                            // -----------------
	glVertex3fv(G);                                            // TOP HORIZONTAL
	glVertex3fv(H);                                            // -------------- 

	glVertex3fv(E);                                            // LEFT VERTICAL 
	glVertex3fv(G);                                            // ------------- 
	glVertex3fv(H);                                            // RIGHT VERTICAL 
	glVertex3fv(F);                                            // -------------- 

	// --- RIGHT
	glVertex3fv(F);                                            // BOTTOM HORIZONTAL                                        // Lastly, the Near and Far Planes' corners are connected.
	glVertex3fv(B);                                         // -----------------
	glVertex3fv(H);                                         // TOP HORIZONTAL 
	glVertex3fv(D);                                         // -------------- 

	// --- LEFT
	glVertex3fv(E);                                            // BOTTOM HORIZONTAL                                        // ---
	glVertex3fv(A);                                            // -----------------
	glVertex3fv(C);                                            // TOP HORIZONTAL 
	glVertex3fv(G);                                            // -------------- 

	glEnd();
	glLineWidth(2.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}