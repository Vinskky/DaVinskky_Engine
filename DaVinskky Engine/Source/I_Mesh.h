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
			void GetVertices(const aiMesh* aimesh, R_Mesh* rmesh);
			void GetNormals(const aiMesh* aimesh, R_Mesh* rmesh);
			void GetTextCoords(const aiMesh* aimesh, R_Mesh* rmesh);
			void GetIndices(const aiMesh* aimesh, R_Mesh* rmesh);
		}

	}
}


#endif // !_I_MESH_H_

