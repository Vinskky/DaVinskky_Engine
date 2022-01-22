#include "R_Texture.h"

R_Texture::R_Texture()
{
	id = -1;
	width = -1;
	height = -1;
	depth = -1;
	bpp = -1;
	bytes = -1;
	format = TEXTURE_FORMAT::UNKNOWN;
	compressed = false;
}

R_Texture::R_Texture(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed) :
	id(id), width(width), height(height), depth(depth),bpp(bpp), bytes(bytes), format(format),compressed(compressed)
{
}

R_Texture::~R_Texture()
{
}

void R_Texture::CleanUp()
{
}

uint R_Texture::CreateTexture()
{
	return uint();
}

void R_Texture::SetTextureData(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed)
{
	this->id = id;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->bpp = bpp;
	this->bytes = bytes;
	this->format = format;
	this->compressed = compressed;
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