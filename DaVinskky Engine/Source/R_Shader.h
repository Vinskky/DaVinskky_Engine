#ifndef _R_Shader_H_
#define _R_Shader_H_

#include "Globals.h"
#include "OpenGL.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4.h"
#include "External/MathGeoLib/include/Math/float4x4.h"

#include <string>
#include <vector>

enum class UNIFORM_TYPE
{
	INT,
	FLOAT,
	BOOL,
	INT_VEC2,
	INT_VEC3,
	INT_VEC4,
	FLOAT_VEC2,
	FLOAT_VEC3,
	FLOAT_VEC4,
	MATRIX4,
	NONE
};

struct Uniform
{
	UNIFORM_TYPE uniformType = UNIFORM_TYPE::NONE;
	std::string name;
	GLenum GLtype;

	bool boolean;
	GLint integer;
	GLfloat floatNumber;
	float2 vec2;
	float3 vec3;
	float4 vec4;
	float4x4 matrix4 = float4x4::zero;

};

class R_Shader
{
public:
	R_Shader(const char* path);
	~R_Shader();

	void SetNameShader(const char* name);
	const char* GetNameShader() const;

	void SetPathShader(const char* name);
	const char* GetPathShader()const;

public:
	//Functionality to be able to save/modify data of Shader
	void SetUniformMatrix4(std::string name, GLfloat* value);
	void SetUniformVec2f(std::string name, GLfloat* value);
	void SetUniformVec3f(std::string name, GLfloat* value);
	void SetUniformVec4f(std::string name, GLfloat* value);

	void SetUniformVec2i(std::string name, GLint* value);
	void SetUniformVec3i(std::string name, GLint* value);
	void SetUniformVec4i(std::string name, GLint* value);

	void SetUniform1f(std::string name, GLfloat value);
	void SetUniform1i(std::string name, GLint value);


public:
	uint32 shaderProgramID;
	uint32 vertexID;
	uint32 fragmentID;

	std::vector<Uniform> uniforms;

private:
	std::string name;
	std::string path;

};



#endif // !_R_Shader_H_

