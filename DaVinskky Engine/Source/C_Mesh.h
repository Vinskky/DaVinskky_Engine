#ifndef _C_MESH_H_
#define _C_MESH_H_

#include "Component.h"
class R_Mesh;

class C_Mesh: public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh();

	void Enable()override;
	void Update()override;
	void Disable()override;

	void SetMesh(R_Mesh* rmesh);

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::MESH; }
private:
	R_Mesh* rmesh = nullptr;
};


#endif // !_C_MESH_H_

