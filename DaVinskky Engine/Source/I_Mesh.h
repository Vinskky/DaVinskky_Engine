#ifndef _I_MESH_H_
#define _I_MESH_H_

struct aiMesh;

class R_Mesh;


namespace Importer
{
	namespace Mesh
	{
		bool Import(const aiMesh* aimesh, R_Mesh* rmesh);

		namespace Private
		{

		}

	}
}


#endif // !_I_MESH_H_

