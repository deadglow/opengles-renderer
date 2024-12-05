#pragma once

#include "CommonTypes.hpp"

#include <unordered_map>
#include <string>

#include "Math.h"

namespace Enoki::Graphics
{
	enum class daShaderType : char
	{
		EMPTY = 0,
		VERTEX = 1,
		FRAGMENT = 2
	};

	struct ShaderProgram
	{
		ShaderProgram() = default;
		ShaderProgram(uint32 program, std::string_view name, std::string_view vertShader, std::string_view fragShader);

		uint32 program;
		std::string name;
		std::string vertShader;
		std::string fragShader;
	};

	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager() = default;
		
		bool IsShaderLoaded(std::string_view name) const;

		const ShaderProgram& GetShader(std::string_view name) const;

		void ReloadShader(std::string_view name);

		void ReloadAllShaders();

		const ShaderProgram& LoadShaderProgramFromFiles(std::string_view name, std::string_view vertShaderFilename, std::string_view fragShaderFilename);

		void UnloadShader(std::string_view name);

		void UnloadAllShaders();

		void UseShader(const ShaderProgram& shader);
		void UseShader(std::string_view name);

		void SetUniform(std::string_view name, const int* value, int count = 1);
		void SetUniform(std::string_view name, const float* value, int count = 1);
		void SetUniform(std::string_view name, const vec2_t* value, int count = 1);
		void SetUniform(std::string_view name, const vec3_t* value, int count = 1);
		void SetUniform(std::string_view name, const vec4_t* value, int count = 1);
		void SetUniform(std::string_view name, const mat4_t* value, int count = 1);

		const ShaderProgram* GetCurrentShader() const;

	private:
		ShaderProgram LoadShaderProgramFromFilesNoEmplace(std::string_view name, std::string_view vertShaderFilename, std::string_view fragShaderFilename);

		uint32 LoadShaderFromSource(std::string_view source, int32 glShaderType);

		uint32 LoadShaderFromFile(std::string_view filename, int32 glShaderType);

		ShaderProgram ConstructShaderProgram(uint32 vertShader, uint32 fragShader, std::string_view name, std::string_view vertShaderName, std::string_view fragShaderName);

		uint32 GetUniformLocation(std::string_view name);

	private:
		std::unordered_map<std::string, ShaderProgram> m_shadermap;
		const ShaderProgram* m_currentShader;
	};
};