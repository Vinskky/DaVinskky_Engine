#ifndef _C_MATERIALS_H_
#define _C_MATERIALS_H_

#include "Component.h"

class R_Texture;
class R_Material;

class C_Material : public Component
{
public:
	C_Material(GameObject* owner);
	~C_Material();

	void Enable()override;
	void Update()override;
	void Disable()override;

	void SetMaterial(R_Material* rmaterial);
	void SetTexture(R_Texture* rtexture);

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::MATERIAL; }
private:
	
	R_Material* rmaterial = nullptr;
	R_Texture* rtexture = nullptr;
};


#endif // !_C_MATERIALS_H_
