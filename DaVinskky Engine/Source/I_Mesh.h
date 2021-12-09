#ifndef _I_MESH_H_
#define _I_MESH_H_

struct aiMesh;

class R_Mesh;

#define NUM_HEADER_CATEGORIES 4

namespace Importer
{
	namespace Mesh
	{
		bool Import(const aiMesh* aimesh, R_Mesh* rmesh);
		bool Save(const R_Mesh* rmesh, const char* path);
		bool Load(const char* path, R_Mesh* rmesh); 

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