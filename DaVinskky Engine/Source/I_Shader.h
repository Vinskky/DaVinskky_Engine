#ifndef _I_SHADER_H_
#define _I_SHADER_H_

#include <string>

class R_Shader;

namespace Importer
{
	namespace Shader
	{
		void Import(const char* path, R_Shader* shader);

		// Read file vertex part
		int ImportVertexShader(std::string shaderFile);

		// Read file fragment part
		int ImportFragmentShader(std::string shaderFile);

		void GetShaderUniforms(R_Shader* shader);

		void SetShaderUniforms(R_Shader* shader);

		void Recompile(R_Shader* shader);

		bool CompileText(std::string fileStr);

		bool CheckUniformName(std::string name);
	}
}

#endif // !_I_SHADER_H_