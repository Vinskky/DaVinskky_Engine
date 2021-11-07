#ifndef _I_TEXTURE_H_
#define _I_TEXTURE_H_

class R_Texture;

namespace Importer
{
	namespace Texture
	{
		bool Import(const char* path, R_Texture* rtexture);

		namespace Private
		{

		}
	}
}

#endif // !_I_TEXTURE_H_

