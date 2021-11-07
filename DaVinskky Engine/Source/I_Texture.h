#ifndef _I_TEXTURE_H_
#define _I_TEXTURE_H_
#include "Globals.h"

class R_Texture;

namespace Importer
{
	namespace Texture
	{
		void Init();
		void CleanUp();

		bool Import(const char* path, R_Texture* rtexture);

		namespace Private
		{
			uint CreateTexture(const void* data, 
								uint width,
								uint height,
								uint target = 0x0DE1, //GL_TEXTURE_2D
								int filter = 0x2600, //GL_NEAREST
								int wrapping = 0x2901, //GL_REPEAT
								int internal_format = 0x1908, //GL_RGBA
								uint format = 0x1908); //GL_RGBA
		}
	}
}

#endif // !_I_TEXTURE_H_

