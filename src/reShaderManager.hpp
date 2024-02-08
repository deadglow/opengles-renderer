#pragma once

#include <unordered_map>
#include <string>

#include "math_3d.h"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

using GLuint = unsigned int;
using GLint = int;

namespace reGraphics
{
	enum class daShaderType : char
	{
		EMPTY = 0,
		VERTEX = 1,
		FRAGMENT = 2
	};

	struct reShaderProgram
	{
		reShaderProgram() = default;
		reShaderProgram(GLuint program, const char* name, const char* vertShader, const char* fragShader);

		GLuint program;
		std::string name;
		std::string vertShader;
		std::string fragShader;
	};

	class reShaderManager
	{
	public:
		bool IsShaderLoaded(const char* name) const;

		const reShaderProgram& GetShader(const char* name) const;

		void ReloadShader(const char* name);

		void ReloadAllShaders();

		const reShaderProgram& LoadShaderProgramFromFiles(const char* name, const char* vertShaderFilename, const char* fragShaderFilename);

		void UnloadShader(const char* name);

		void UnloadAllShaders();

		void UseShader(const reShaderProgram& shader);
		void UseShader(const char* name);

		void SetUniform(const char* name, const int* value, int count = 1);
		void SetUniform(const char* name, const float* value, int count = 1);
		void SetUniform(const char* name, const vec2_t* value, int count = 1);
		void SetUniform(const char* name, const vec3_t* value, int count = 1);
		void SetUniform(const char* name, const vec4_t* value, int count = 1);
		void SetUniform(const char* name, const mat4_t* value, int count = 1);

		const reShaderProgram* GetCurrentShader() const;

	private:
		reShaderProgram LoadShaderProgramFromFilesNoEmplace(const char* name, const char* vertShaderFilename, const char* fragShaderFilename);

		GLuint LoadShaderFromSource(const char* source, GLint glShaderType);

		GLuint LoadShaderFromFile(const char* filename, GLint glShaderType);

		reShaderProgram ConstructShaderProgram(GLuint vertShader, GLuint fragShader, const char* name, const char* vertShaderName, const char* fragShaderName);

		GLuint GetUniformLocation(const char* name);

	private:
		umap<std::string, reShaderProgram> m_shadermap;
		const reShaderProgram* m_currentShader;
	};
};