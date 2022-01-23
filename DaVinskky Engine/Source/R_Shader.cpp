#include "R_Shader.h"

R_Shader::R_Shader() :shaderProgramID(0), vertexID(0), fragmentID(0)
{
}

R_Shader::~R_Shader()
{
	glDeleteProgram(shaderProgramID);
}

void R_Shader::UseShader()
{
	glUseProgram(shaderProgramID);
}

const char* R_Shader::LogShader(uint shaderObject)
{
	// Check compilation errors
	GLint success;
	char infoLog[512];

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
		LOG("Shader compilation error : %s", infoLog);
		glDeleteShader(shaderObject);
	}

	return infoLog;
}

const char* R_Shader::GetShaderPath() const
{
	return path.c_str();
}

void R_Shader::SetShaderPath(const char* name)
{
	this->path = name;
}

void R_Shader::SetUniformMatrix4(std::string name, GLfloat* value)
{
	int uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniformMatrix4fv(uinformLoc, 1, GL_FALSE, value);
}

void R_Shader::SetUniformVec2f(std::string name, GLfloat* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform2fv(uinformLoc, 1, value);
}

void R_Shader::SetUniformVec3f(std::string name, GLfloat* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform3fv(uinformLoc, 1, value);
}

void R_Shader::SetUniformVec4f(std::string name, GLfloat* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform4fv(uinformLoc, 1, value);
}

void R_Shader::SetUniformVec2i(std::string name, GLint* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform2iv(uinformLoc, 1, value);
}

void R_Shader::SetUniformVec3i(std::string name, GLint* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform3iv(uinformLoc, 1, value);
}

void R_Shader::SetUniformVec4i(std::string name, GLint* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform4iv(uinformLoc, 1, value);
}

void R_Shader::SetUniform1f(std::string name, GLfloat value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform1f(uinformLoc, value);
}

void R_Shader::SetUniform1i(std::string name, GLint value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform1i(uinformLoc, value);
}

void R_Shader::SetBool(std::string name, bool value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform1i(uinformLoc, (GLint)value);
}