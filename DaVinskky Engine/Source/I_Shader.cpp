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
	if (shader == nullptr)
		return;

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
		shader->vertexID = (GLuint)ImportVertexShader(file);
	}
	if (file.find(FRAGMENT_SHADER) != std::string::npos)
	{
		shader->fragmentID = (GLuint)ImportFragmentShader(file);
	}

	delete[] buffer;

	if (shader->vertexID != 0 && shader->fragmentID != 0)
	{
		GLint outcome;
		shader->shaderProgramID = glCreateProgram();
		glAttachShader(shader->shaderProgramID, shader->vertexID);
		glAttachShader(shader->shaderProgramID, shader->fragmentID);
		glLinkProgram(shader->shaderProgramID);

		// Check linking errors
		glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &outcome);
		if (outcome == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shader->shaderProgramID, 512, NULL, info);
			glDeleteProgram(shader->shaderProgramID);
			glDeleteShader(shader->vertexID);
			glDeleteShader(shader->fragmentID);
			LOG("Shader compiling error: %s", info);
		}
		else if (shader->uniforms.size() == 0)
		{
			GetShaderUniforms(shader);
		}
		glDetachShader(shader->shaderProgramID, shader->vertexID);
		glDetachShader(shader->shaderProgramID, shader->fragmentID);

		glDeleteShader(shader->vertexID);
		glDeleteShader(shader->fragmentID);
	}
	else
	{
		LOG("Vertex Shader: %d or Fragment Shader: %d are not correctly compiled.", shader->vertexID, shader->fragmentID);
	}
}

int Importer::Shader::ImportVertexShader(std::string shaderFile)
{
	std::string vertexFile = std::string("#version 330 core\r\n") +
		std::string("#define ") +
		VERTEX_SHADER + "\r\n";
	vertexFile += shaderFile;

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)vertexFile.c_str();
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);

	// Check vertex compilation errors
	GLint outcome;
	GLchar info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == GL_FALSE)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		glDeleteShader(vertexShader);
		LOG("Vertex Shader compilation error (%s)", info);
	}

	return (outcome != 0) ? vertexShader : -1;
}

int Importer::Shader::ImportFragmentShader(std::string shaderFile)
{
	std::string fragmentFile = std::string("#version 330 core\r\n") +
		std::string("#define ") +
		FRAGMENT_SHADER + "\r\n";
	fragmentFile += shaderFile;

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* source = (const GLchar*)fragmentFile.c_str();
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);

	GLint outcome;
	GLchar info[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		glDeleteShader(fragmentShader);
		LOG("Fragment Shader compilation error (%s)", info);
	}

	return (outcome != 0) ? fragmentShader : -1;
}

void Importer::Shader::GetShaderUniforms(R_Shader* shader)
{
	GLint activeUniforms;

	glGetProgramiv(shader->shaderProgramID, GL_ACTIVE_UNIFORMS, &activeUniforms);

	for (uint i = 0; i < activeUniforms; ++i)
	{
		Uniform uniform;
		GLint length;
		GLint size;
		GLchar name[32];
		glGetActiveUniform(shader->shaderProgramID, i, sizeof(name), &length, &size, &uniform.GLtype, name);
		uniform.name = name;

		if (CheckUniformName(uniform.name))
		{
			uint uinformLoc = glGetUniformLocation(shader->shaderProgramID, uniform.name.c_str());

			switch (uniform.GLtype)
			{
			case GL_INT:
				uniform.uniformType = UNIFORM_TYPE::INT;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, &uniform.integer);
				break;
			case GL_FLOAT:
				uniform.uniformType = UNIFORM_TYPE::FLOAT;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.floatNumber);
				break;
			case GL_BOOL:
				uniform.uniformType = UNIFORM_TYPE::BOOL;
				
				break;
			case GL_INT_VEC2:
				uniform.uniformType = UNIFORM_TYPE::INT_VEC2;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec2);
				
				break;
			case GL_INT_VEC3:
				uniform.uniformType = UNIFORM_TYPE::INT_VEC3;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec3);
				
				break;
			case GL_INT_VEC4:
				uniform.uniformType = UNIFORM_TYPE::INT_VEC4;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec4);
				
				break;
			case GL_FLOAT_VEC2:
				uniform.uniformType = UNIFORM_TYPE::FLOAT_VEC2;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, (GLfloat*)&uniform.vec2);
				
				break;
			case GL_FLOAT_VEC3:
				uniform.uniformType = UNIFORM_TYPE::FLOAT_VEC3;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, (GLfloat*)&uniform.vec3);
				
				break;
			case GL_FLOAT_VEC4:
				uniform.uniformType = UNIFORM_TYPE::FLOAT_VEC4;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, (GLfloat*)&uniform.vec4);
				
				break;
			case GL_FLOAT_MAT4:
				uniform.uniformType = UNIFORM_TYPE::MATRIX4;
				glGetnUniformfv(shader->shaderProgramID, uinformLoc, sizeof(uniform.matrix4), &uniform.matrix4.v[0][0]);
				break;

			default: uniform.uniformType = UNIFORM_TYPE::NONE; break;

			}

			if (uniform.uniformType != UNIFORM_TYPE::NONE) shader->uniforms.push_back(uniform);
		}
	}
}

void Importer::Shader::SetShaderUniforms(R_Shader* shader)
{
	if (shader->uniforms.empty())
		return;

	for (uint i = 0; i < shader->uniforms.size(); ++i)
	{
		switch (shader->uniforms[i].uniformType)
		{
		case UNIFORM_TYPE::FLOAT:
			shader->SetUniform1f(shader->uniforms[i].name.c_str(), shader->uniforms[i].floatNumber);
			break;
		case UNIFORM_TYPE::FLOAT_VEC2:
			shader->SetUniformVec2f(shader->uniforms[i].name.c_str(), shader->uniforms[i].vec2.ptr());
			break;
		case UNIFORM_TYPE::FLOAT_VEC3:
			shader->SetUniformVec3f(shader->uniforms[i].name.c_str(), shader->uniforms[i].vec3.ptr());
			break;
		case UNIFORM_TYPE::FLOAT_VEC4:
			shader->SetUniformVec4f(shader->uniforms[i].name.c_str(), shader->uniforms[i].vec4.ptr());
			break;
		case UNIFORM_TYPE::INT:
			shader->SetUniform1i(shader->uniforms[i].name.c_str(), shader->uniforms[i].integer);
			break;
		case UNIFORM_TYPE::INT_VEC2:
			shader->SetUniformVec2i(shader->uniforms[i].name.c_str(), (GLint*)shader->uniforms[i].vec2.ptr());
			break;
		case UNIFORM_TYPE::INT_VEC3:
			shader->SetUniformVec3i(shader->uniforms[i].name.c_str(), (GLint*)shader->uniforms[i].vec3.ptr());
			break;
		case UNIFORM_TYPE::INT_VEC4:
			shader->SetUniformVec4i(shader->uniforms[i].name.c_str(), (GLint*)shader->uniforms[i].vec4.ptr());
			break;
		case UNIFORM_TYPE::MATRIX4:
			shader->SetUniformMatrix4(shader->uniforms[i].name.c_str(), (GLfloat*)shader->uniforms[i].matrix4.Transposed().ptr());
			break;
		default:
			break;
		}
	}
}

void Importer::Shader::Recompile(R_Shader* shader)
{
	glDetachShader(shader->shaderProgramID, shader->vertexID);
	glDetachShader(shader->shaderProgramID, shader->fragmentID);
	glDeleteProgram(shader->shaderProgramID);

	Importer::Shader::Import(shader->GetPathShader(), shader);
}

bool Importer::Shader::CheckUniformName(std::string name)
{
	if (name != "inColor" && name != "time" && name != "modelMatrix" &&
		name != "viewMatrix" && name != "projectionMatrix" && name != "cameraPosition")
	{
		return true;
	}
	else
	{
		return false;
	}		
}