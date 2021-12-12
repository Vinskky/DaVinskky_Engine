#include "I_Texture.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "OpenGL.h"
#include "External/DevIL/include/ilu.h"
#include "External/DevIL/include/ilut.h"
#include "R_Texture.h"

#include "External/mmgr/include/mmgr.h"

void Importer::Texture::Init()
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

void Importer::Texture::CleanUp()
{
	LOG("Shutting down DevIL");
	ilShutDown();
}

bool Importer::Texture::Import(const char* path, R_Texture* rtexture)
{
	bool ret = true;

	if (path == nullptr)
	{
		LOG("ERROR importing texture, path is nullptr");
		return false;
	}
	if(rtexture == nullptr)
	{
		LOG("ERROR importing texture, R_Texture is nullptr");
		return false;
	}

	LOG("Importer loading texture from: %s", path);

	char* buffer = nullptr;
	uint read = app->fileSystem->Load(path, &buffer);

	if (read == 0)
		return false;

	if (buffer == nullptr)
		return false;

	//DevIL load
	ILuint ilImage = 0;
	ilGenImages(1, &ilImage);
	ilBindImage(ilImage);

	ret = ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, read);
	if (!ret)
	{
		LOG("Error on DevIL ilLoadL() Error: [%s]",  iluErrorString(ilGetError()));
		return false;
	}

	uint colorChannels = ilGetInteger(IL_IMAGE_CHANNELS);
	if (colorChannels == 3)
	{
		ret = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	}
	else if (colorChannels == 4)
	{
		ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	}
	else
	{
		LOG("Warning texture loaded has less than 3 channels. path: %s", path);
	}

	if (!ret)
	{
		LOG("ERROR trying to convert Image to RGB or RGBA");
		return false;
	}

	ILinfo infoIl;
	iluGetImageInfo(&infoIl);

	if (infoIl.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	uint width = infoIl.Width;
	uint height = infoIl.Height;
	uint depth = infoIl.Depth;
	uint bpp = infoIl.Bpp;
	uint size = infoIl.SizeOfData;
	uint format = infoIl.Format;
	uint target = (uint)GL_TEXTURE_2D;
	int wrapping = (int)GL_REPEAT;
	int filter = (int)GL_LINEAR;

	uint texId = Private::CreateTexture(ilGetData(), width, height, target, wrapping, filter, format, format);

	if (texId != 0)
	{
		rtexture->SetTextureData(texId, width, height, depth, bpp, size, (TEXTURE_FORMAT)format);
	}

	ilDeleteImages(1, &ilImage);

	RELEASE_ARRAY(buffer);

	return ret;
}


uint Importer::Texture::Private::CreateTexture(const void* data, uint width, uint height, uint target, int filter, int wrapping, int internal_format, uint format)
{
	uint texId = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&texId);
	glBindTexture(target, texId);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapping);

	if (filter == GL_NEAREST)                                                                                    // Nearest filtering gets the color of the nearest neighbour pixel.
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filter == GL_LINEAR)                                                                                // Linear filtering interpolates the color of the neighbour pixels.
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))                                                // In case Anisotropic filtering is available, it will be used.
		{
			GLfloat maxAnisotropy;

			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)maxAnisotropy);
		}
	}
	else
	{
		LOG("[ERROR] Invalid filter type! Supported filters: GL_LINEAR and GL_NEAREST.");
	}

	glTexImage2D(target, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(target);

	glBindTexture(target, 0);

	if (texId != 0)
	{
		LOG("[STATUS] Texture Successfully loaded! Id: %u, Size: %u x %u", texId, width, height);
	}

	return texId;
}
