#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "R_Texture.h"
#include "R_Material.h"
#include "I_Scene.h"
#include "I_Texture.h"
#include "C_Material.h"
#include <string>

#include "External/mmgr/include/mmgr.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput()
{
	SetName("Input");
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= SCREEN_SIZE;
	mouseY /= SCREEN_SIZE;
	mouseZ = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouseButtons[i] == KEY_IDLE)
				mouseButtons[i] = KEY_DOWN;
			else
				mouseButtons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouseButtons[i] == KEY_REPEAT || mouseButtons[i] == KEY_DOWN)
				mouseButtons[i] = KEY_UP;
			else
				mouseButtons[i] = KEY_IDLE;
		}
	}

	mouseXMotion = mouseYMotion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		app->editor->GetEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouseZ = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouseX = e.motion.x / SCREEN_SIZE;
			mouseY = e.motion.y / SCREEN_SIZE;

			mouseXMotion = e.motion.xrel / SCREEN_SIZE;
			mouseYMotion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					app->renderer3D->OnResize();
			}
			break;
			case SDL_DROPFILE:
			{
				//DRAG & DROP:
				if (e.drop.file != nullptr)
				{
					const char* tmp = e.drop.file;
					
					std::string normStr = app->fileSystem->NormalizePath(tmp);

					normStr = normStr.substr(normStr.find_last_of("A"), normStr.size());

					std::string extStr = app->fileSystem->GetFileExtension(normStr.c_str());

					if (extStr == "fbx" || extStr == "FBX")
					{
						Importer::Scene::Import(normStr.c_str(), app->sceneIntro->sceneGameObjects);
					}
					if (extStr == "PNG" || extStr == "png" || extStr == "DDS" || extStr == "dds")
					{
						//Checks if selected gameobject has a component material created.
						C_Material* tmp = app->sceneIntro->selectedGameObj->GetComponent<C_Material>();
						if (tmp == nullptr)
						{
							C_Material* mat = (C_Material*)app->sceneIntro->selectedGameObj->CreateComponent(COMPONENT_TYPE::MATERIAL);
							R_Material* rmat = new R_Material();
							mat->SetMaterial(rmat);
							R_Texture* textMat = new R_Texture();
							mat->SetTexture(textMat);
							Importer::Texture::Import(normStr.c_str(), textMat);
						}
						else
						{
							if(tmp->GetTexture() != nullptr)
								Importer::Texture::Import(normStr.c_str(), tmp->GetTexture());
							else
							{
								R_Texture* textMat = new R_Texture();
								tmp->SetTexture(textMat);
								Importer::Texture::Import(normStr.c_str(), tmp->GetTexture());
							}
						}
					}
				}
				else
				{
					LOG("ERROR on Input module: String from SDL_DROPFILE even was nullptr");
				}
				SDL_free(e.drop.file);
			}
			break;
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}