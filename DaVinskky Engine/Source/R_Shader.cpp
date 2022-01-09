#include "R_Shader.h"

R_Shader::R_Shader(const char* path):shaderProgramID(0),vertexID(0),fragmentID(0)
{
}

R_Shader::~R_Shader()
{
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void R_Shader::CreateDefaultShader()
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core/n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexID);

	LOG(LogShader(vertexID));

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentID);

	LOG(LogShader(fragmentID));
}

void R_Shader::LinkShaderProgram()
{
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexID);
	glAttachShader(shaderProgramID, fragmentID);
	glLinkProgram(shaderProgramID);

	// Check linking errors
	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		LOG("Shader linking error : %s", infoLog);
	}
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

void R_Shader::SetNameShader(const char* name)
{
	this->name = name;
}

const char* R_Shader::GetNameShader() const
{
	return name.c_str();
}

void R_Shader::SetPathShader(const char* name)
{
	this->path = name;
}

const char* R_Shader::GetPathShader() const
{
	return path.c_str();
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
