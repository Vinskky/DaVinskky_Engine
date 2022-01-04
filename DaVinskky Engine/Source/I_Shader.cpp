#include "Application.h"
#include "I_Shader.h"
#include "R_Shader.h"
#include "ModuleFileSystem.h"

#include "External/Glew/include/glew.h"
#include "External/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

void Importer::Shader::Import(const char* wholePath, R_Shader* shader)
{
	std::string path;
	std::string name;
	std::string ext;
	app->fileSystem->SplitFilePath(wholePath, &path, &name, &ext);

	char* buffer;
	int size = app->fileSystem->Load(wholePath, &buffer);
	if (size <= 0)
	{
		delete[] buffer;
		LOG("Shader file: %s not found or can't be loaded", wholePath);
		return;
	}

	std::string file(buffer);
	if (file.find(VERTEX_SHADER) != std::string::npos)
	{
		shader->vertexID = (GLuint)ImportVertexShader(file, shader);
	}
	if (file.find(FRAGMENT_SHADER) != std::string::npos)
	{
		shader->fragmentID = (GLuint)ImportFragmentShader(file, shader);
	}

	delete[] buffer;

	if (shader->vertexID != 0 && shader->fragmentID != 0)
	{
		GLint outcome;
		shader->shaderProgramID = glCreateProgram();
		glAttachShader(shader->shaderProgramID, shader->vertexID);
		glAttachShader(shader->shaderProgramID, shader->fragmentID);
		glLinkProgram(shader->shaderProgramID);

		glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &outcome);
		if (outcome == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shader->shaderProgramID, 512, NULL, info);
			LOG("Shader compiling error: %s", info);
		}
		else if (shader->uniforms.size() == 0)
		{
			GetShaderUniforms(shader);
		}

		glDeleteShader(shader->vertexID);
		glDeleteShader(shader->fragmentID);
		glDeleteShader(shader->shaderProgramID);
	}
	else
	{
		LOG("Vertex shader: %d or Fragment shader: %d are not correctly compiled.", shader->vertexID, shader->fragmentID);
	}
}

int Importer::Shader::ImportVertexShader(std::string shaderFile, R_Shader* shader)
{
	std::string vertexFile = std::string("#version 330 core\r\n") +
		std::string("#define ") +
		VERTEX_SHADER + "\r\n";
	vertexFile += shaderFile;

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)vertexFile.c_str();
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);

	GLint outcome;
	GLchar info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}



	return (outcome != 0) ? vertexShader : -1;
}

int Importer::Shader::ImportFragmentShader(std::string shaderFile, R_Shader* shader)
{
	std::string fragmentFile = std::string("#version 330 core\r\n") +
		std::string("#define ") +
		FRAGMENT_SHADER + "\r\n";
	fragmentFile += shaderFile;

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* source = (const GLchar*)fragmentFile.c_str();
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);

	GLint outcome;
	GLchar info[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}


	return (outcome != 0) ? fragmentShader : -1;
}

void Importer::Shader::GetShaderUniforms(R_Shader* shader)
{
}

void Importer::Shader::SetShaderUniforms(R_Shader* shader)
{
}

void Importer::Shader::Recompile(R_Shader* shader)
{
	glDetachShader(shader->shaderProgramID, shader->vertexID);
	glDetachShader(shader->shaderProgramID, shader->fragmentID);
	glDeleteProgram(shader->shaderProgramID);

	Importer::Shader::Import(shader->GetPathShader(), shader);
}
