#ifndef _C_MATERIALS_H_
#define _C_MATERIALS_H_

#include "Component.h"
#include "Globals.h"
#include "Color.h"

#include <string>

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
	bool CleanUp()override;

	void SetMaterial(R_Material* rmaterial);
	void SetTexture(R_Texture* rtexture);

	R_Texture* GetTexture()const;
	bool UseDefaultTexture();
	void SetDefaultTexture(bool text);
	Color GetMaterialColour();
	uint GetTextureID();

	void SetTexturePath(const char* path);
	const char* GetTexturePath() const;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::MATERIAL; }
private:
	
	R_Material* rmaterial = nullptr;
	R_Texture* rtexture = nullptr;
	bool debugTextEnabled;
	std::string _path;
};


#endif // !_C_MATERIALS_H_
