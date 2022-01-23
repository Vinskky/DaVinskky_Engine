#ifndef _C_MATERIALS_H_
#define _C_MATERIALS_H_

#include "Component.h"
#include "Globals.h"
#include "Color.h"
#include "JsonFile.h"

#include <string>

class R_Texture;
class R_Material;
class R_Shader;

class C_Material : public Component
{
public:
	C_Material(GameObject* owner, bool debugTextEnabled = false);
	~C_Material();

	void Enable()override;
	void Update()override;
	void Disable()override;
	bool CleanUp()override;

	void Save(json& jsonComp);
	void Load(json& jsonComp);

	void SetMaterial(R_Material* rmaterial);
	void SetTexture(R_Texture* rtexture);
	void SetShader(R_Shader* rshader);

	R_Texture* GetTexture()const;
	uint GetTextureID();
	const char* GetTexturePath() const;
	void SetTexturePath(const char* path);

	R_Shader* GetShader() const;
	const char* GetShaderPath() const;
	void SetShaderPath(const char* path);

	void SetDefaultTexture(bool text);
	bool UseDefaultTexture();

	Color GetMaterialColour();

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::MATERIAL; }

private:
	R_Material* rmaterial = nullptr;
	R_Texture* rtexture = nullptr;
	R_Shader* rshader = nullptr;

	bool debugTextEnabled;

	std::string texturePath;
	std::string shaderPath;
};

#endif // !_C_MATERIALS_H_