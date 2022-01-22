#ifndef _R_TEXTURE_H_
#define _R_TEXTURE_H_

#include "Globals.h"

enum class TEXTURE_FORMAT
{
	UNKNOWN = 0,							// Default format. It means that DevIL could not find the format of the imported texture.
	COLOUR_INDEX = 0x1900,					// 0x1900 = IL_COLOUR_INDEX. 
	RGB = 0x1907,							// 0x1907 = IL_RGB.
	RGBA = 0x1908,							// 0x1908 = IL_RGBA.
	BGR = 0x80E0,							// 0x80E0 = IL_BGR.
	BGRA = 0x80E1,							// 0x80E1 = IL_BGRA.
	LUMINANCE = 0x1909						// 0x1909 = IL_LUMINANCE.
};

class R_Texture
{
public:
	R_Texture();
	R_Texture(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed);
	~R_Texture();

	void CleanUp();

	uint CreateTexture();

	void SetTextureData(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed = true);

public:
	// Get Private parameters
	uint GetTextureID() const;
	uint GetTextureWidth() const;
	uint GetTextureHeight() const;
	uint GetTextureDepth() const;
	uint GetTextureBpp() const;
	uint GetTextureBytes() const;
	TEXTURE_FORMAT GetTextureFormat() const;
	bool TextureIsCompressed() const;

private:
	uint id;
	uint width;
	uint height;
	uint depth;
	uint bpp;
	uint bytes;
	TEXTURE_FORMAT format;
	bool compressed;		//indicator for the use of filters dds
};


#endif // !_I_TEXTURE_H_

