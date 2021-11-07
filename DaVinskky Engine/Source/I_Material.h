#ifndef _I_MATERIAL_H_
#define _I_MATERIAL_H_

struct aiMaterial;

class R_Material;

namespace Importer
{
	namespace Material
	{
		bool Import(const aiMaterial* aimat, R_Material* rmat);
	}
}
#endif // !_I_MATERIAL_H_

