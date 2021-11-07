#ifndef _C_MATERIALS_H_
#define _C_MATERIALS_H_

#include "Component.h"
#include "Globals.h"
#include "Color.h"

class R_Texture;
class R_Material;

class C_Material : public Component
{
public:
	C_Material(GameObject* owner, bool debugTextEnabled = false);
	~C_Material();

	void Enable()override;
	void Update()override;
	void Disable()override;

	void SetMaterial(R_Material* rmaterial);
	void SetTexture(R_Texture* rtexture);

	R_Texture* GetTexture()const;
	bool UseDefaultTexture();
	Color GetMaterialColour();
	uint GetTextureID();

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::MATERIAL; }
private:
	
	R_Material* rmaterial = nullptr;
	R_Texture* rtexture = nullptr;
	bool debugTextEnabled;
};


#endif // !_C_MATERIALS_H_
