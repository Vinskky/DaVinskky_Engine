#include "R_Shader.h"

R_Shader::R_Shader() :shaderProgramID(0), vertexID(0), fragmentID(0)
{
}

R_Shader::~R_Shader()
{
	glDeleteProgram(shaderProgramID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void R_Shader::UseShader()
{
	glUseProgram(shaderProgramID);
}

void R_Shader::CreateDefaultShader()
{
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 color;\n"
		"layout (location = 2) in vec2 texCoord;\n"
		"out vec4 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform vec4 inColor;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = projection * view * model_matrix * vec4(position, 1.0f);\n"
		"   ourColor = inColor;\n"
		"   TexCoord = texCoord;\n"
		"}\0";

	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"in vec4 ourColor;\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform bool hasTexture;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"   vec4 texColor = (hasTexture) ? texture(ourTexture, TexCoord) : vec4(0.5f,0.5f,0.5f,1.0f);\n"
		"   color = texColor * ourColor;\n"
		"}\0";

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexID);

	// Check vertex compilation errors
	GLint success = 0;
	char infoLog[512];

	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		glDeleteShader(vertexID);

		LOG("Shader compilation error : %s", infoLog);
	}

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
		glDeleteShader(fragmentID);
		glDeleteShader(vertexID);

		LOG("Shader compilation error : %s", infoLog);
	}
}

void R_Shader::LinkShaderProgram()
{
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexID);
	glAttachShader(shaderProgramID, fragmentID);
	glLinkProgram(shaderProgramID);

	// Check linking errors
	int success = 0;
	char infoLog[512];

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		glDeleteProgram(shaderProgramID);
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		LOG("Shader linking error : %s", infoLog);
	}
	glDetachShader(shaderProgramID, vertexID);
	glDetachShader(shaderProgramID, fragmentID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
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

void R_Shader::SetBool(std::string name, bool value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform1i(uinformLoc, (GLint)value);
}