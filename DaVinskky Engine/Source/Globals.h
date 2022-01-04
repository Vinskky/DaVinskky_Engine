#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

// Memory Managing Macros
// Deletes a buffer
#define RELEASE( x )			\
	{							\
		if ( x != nullptr )		\
		{						\
			delete x;			\
			x = nullptr;		\
		}						\
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x )		\
	{							\
		if (x != nullptr)		\
		{						\
			delete[] x;			\
			x = nullptr;		\
		}						\
	}

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "DaVinskky Engine"
#define ORGANIZATION "UPC"

// Paths
#define LIBRARY_PATH "Library/"
#define TEXTURES_PATH "Library/Textures/"
#define MESHES_PATH "Library/Meshes/"
#define SCENES_PATH "Library/Scenes/"
#define ASSETS_MODELS_PATH "Assets/Models/"
#define ASSETS_TEXTURES_PATH "Assets/Textures/"
#define ASSETS_SHADERS_PATH "Assets/Shaders/"

//Shaders
#define VERTEX_SHADER "__Vertex_Shader__"
#define FRAGMENT_SHADER "__Fragment_Shader__"

// ImGui Globals
#define IMGUI_BLACK ImVec4(0.0f,0.0f,0.0f,1.f)
#define IMGUI_LIGHT_GREY ImVec4(0.8f,0.8f,0.8f,1.f)
#define IMGUI_GREY ImVec4(0.6f,0.6f,0.6f,1.f)
#define IMGUI_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_YELLOW ImVec4(1.f,1.f,0.f,1.f)
#define IMGUI_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_WHITE ImVec4(1.f,1.f,1.f,1.f)

#define IMGUI_BULLET(color, field, format, ...) \
    ImGui::BulletText(field); \
    ImGui::SameLine(); \
    ImGui::TextColored(color, format, __VA_ARGS__)