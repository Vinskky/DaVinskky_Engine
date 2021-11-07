#include "R_Texture.h"
#include "OpenGL.h"
#include "External/DevIL/include/ilu.h"
#include "External/DevIL/include/ilut.h"


R_Texture::R_Texture()
{
}

R_Texture::R_Texture(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed) :
	id(id), width(width), height(height), depth(depth),bpp(bpp), bytes(bytes), format(format),compressed(compressed)
{
}

R_Texture::~R_Texture()
{
}

void R_Texture::Init()
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("Error: DevIL Version does not match with lib version.");
	}

	LOG("Initializing DevIL");
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void R_Texture::CleanUp()
{
	LOG("Shutting down DevIL");
	ilShutDown();
}

bool R_Texture::Load()
{
	bool ret = true;
	
	//creating procedurally a checker texture
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
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);


		
	return ret;
}

uint R_Texture::CreateTexture()
{
	return uint();
}

uint R_Texture::GetTextureID() const
{
	return id;
}

uint R_Texture::GetTextureWidth() const
{
	return width;
}

uint R_Texture::GetTextureHeight() const
{
	return height;
}

uint R_Texture::GetTextureDepth() const
{
	return depth;
}

uint R_Texture::GetTextureBpp() const
{
	return bpp;
}

uint R_Texture::GetTextureBytes() const
{
	return bytes;
}

TEXTURE_FORMAT R_Texture::GetTextureFormat() const
{
	return format;
}

bool R_Texture::TextureIsCompressed() const
{
	return compressed;
}
