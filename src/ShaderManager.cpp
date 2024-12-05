#include "ShaderManager.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

#include "gles2.h"
#include "SDL/SDL.h"

static inline std::string_view SHADER_PATH = "assets\\shaders\\";
static inline std::string_view DEFAULT_VS = "default";
static inline std::string_view DEFAULT_FS = "default";
static inline std::string_view LINES_VS = "lines";
static inline std::string_view LINES_FS = "lines";

using namespace Enoki::Graphics;
//#define LOG_INVALID_UNIFORM

ShaderProgram::ShaderProgram(uint32 program, std::string_view name, std::string_view vertShader, std::string_view fragShader)
	: program(program)
	, name(name)
	, vertShader(vertShader)
	, fragShader(fragShader)
{
}

ShaderManager::ShaderManager()
{
	LoadShaderProgramFromFiles("default", DEFAULT_VS, DEFAULT_FS);
	LoadShaderProgramFromFiles("lines", LINES_VS, LINES_FS);
}

bool ShaderManager::IsShaderLoaded(std::string_view name) const
{
	return m_shadermap.count(std::string(name)) > 0;
}

const ShaderProgram& ShaderManager::GetShader(std::string_view name) const
{
	SDL_assert(IsShaderLoaded(name));
	
	return m_shadermap.at(std::string(name));
}

void ShaderManager::ReloadShader(std::string_view name)
{
	SDL_assert(IsShaderLoaded(name));

	ShaderProgram& shader = m_shadermap[std::string(name)];

	glDeleteProgram(shader.program);

	shader = LoadShaderProgramFromFilesNoEmplace(shader.name.c_str(), shader.vertShader.c_str(), shader.fragShader.c_str());
}

void ShaderManager::ReloadAllShaders()
{
	for (auto [key, value] : m_shadermap)
	{
		ReloadShader(key.c_str());
	}
}

const ShaderProgram& ShaderManager::LoadShaderProgramFromFiles(std::string_view name, std::string_view vertShaderFilename, std::string_view fragShaderFilename)
{
	SDL_assert(!IsShaderLoaded(name));

	std::string nameStr(name);
	m_shadermap.emplace(nameStr, LoadShaderProgramFromFilesNoEmplace(name, vertShaderFilename, fragShaderFilename));
	
	return m_shadermap[nameStr];
}

void ShaderManager::UnloadShader(std::string_view name)
{
	SDL_assert(IsShaderLoaded(name));

	std::string nameStr(name);
	
	const ShaderProgram& program = m_shadermap[nameStr];
	glDeleteProgram(program.program);

	m_shadermap.erase(nameStr);
}

void ShaderManager::UnloadAllShaders()
{
	for (auto [key, value] : m_shadermap)
	{
		UnloadShader(key.c_str());
	}
}

void ShaderManager::UseShader(const ShaderProgram& shader)
{
	SDL_assert(shader.program);
	glUseProgram(shader.program);
	m_currentShader = &shader;
}

void ShaderManager::UseShader(std::string_view name)
{
	const ShaderProgram& shader = GetShader(name);
	UseShader(shader);
}

void ShaderManager::SetUniform(std::string_view name, const int32_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform1iv(location, count, value);
}

void ShaderManager::SetUniform(std::string_view name, const float* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform1fv(location, count, value);
}

void ShaderManager::SetUniform(std::string_view name, const vec2_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform2fv(location, count, (const float*)value);
}

void ShaderManager::SetUniform(std::string_view name, const vec3_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform3fv(location, count, (const float*)value);
}

void ShaderManager::SetUniform(std::string_view name, const vec4_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform4fv(location, count, (const float*)value);
}

void ShaderManager::SetUniform(std::string_view name, const mat4_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, count, false, (const float*)value);
}

const ShaderProgram* ShaderManager::GetCurrentShader() const
{
	return m_currentShader;
}

ShaderProgram ShaderManager::LoadShaderProgramFromFilesNoEmplace(std::string_view name, std::string_view vertShaderFilename, std::string_view fragShaderFilename)
{
	// load vert/frag shaders
	uint32 vertShader = LoadShaderFromFile(vertShaderFilename, GL_VERTEX_SHADER);
	uint32 fragShader = LoadShaderFromFile(fragShaderFilename, GL_FRAGMENT_SHADER);

	ShaderProgram newProgram = ConstructShaderProgram(vertShader, fragShader, name, vertShaderFilename, fragShaderFilename);

	// clean up shaders
	glDetachShader(newProgram.program, vertShader);
	glDetachShader(newProgram.program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return newProgram;
}

uint32 ShaderManager::LoadShaderFromSource(std::string_view source, int32 glShaderType)
{
	SDL_assert(!source.empty());
	SDL_assert(glShaderType == GL_VERTEX_SHADER || glShaderType == GL_FRAGMENT_SHADER);

	uint32 glShader = glCreateShader(glShaderType);
	const char* buffer = source.data();
	glShaderSource(glShader, 1, &buffer, NULL);
	glCompileShader(glShader);

	int32 compileStatus;
	glGetShaderiv(glShader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		int length = 0;
		glGetShaderiv(glShader, GL_INFO_LOG_LENGTH, &length);
		length++;
		
		char* errorStr = new char[length];
		glGetShaderInfoLog(glShader, length, NULL, errorStr);
		printf("glCompile failure! Error: %s", errorStr);

		delete[] errorStr;
	}

	return glShader;
}

uint32 ShaderManager::LoadShaderFromFile(std::string_view filename, int32 glShaderType)
{
	SDL_assert(glShaderType == GL_VERTEX_SHADER || glShaderType == GL_FRAGMENT_SHADER);
	
	std::string relativePath(SHADER_PATH);
	relativePath.append(filename);
	
	std::string_view extension = glShaderType == GL_VERTEX_SHADER ? ".vs" : ".fs";
	relativePath.append(extension);

	std::filesystem::path path = std::filesystem::current_path();
	path.append(relativePath);
	SDL_assert(std::filesystem::is_regular_file(path));

	printf("\nLoading shader %s\n", path.string().c_str());

	std::ifstream f(path);
	SDL_assert(f);

	std::ostringstream ss;
	ss << f.rdbuf();

	std::string source;
	source = ss.str();
	SDL_assert(source.size());

	f.close();

	return LoadShaderFromSource(source.c_str(), glShaderType);
}

ShaderProgram ShaderManager::ConstructShaderProgram(uint32 vertShader, uint32 fragShader, std::string_view name, std::string_view vertShaderName, std::string_view fragShaderName)
{
	SDL_assert(vertShader && fragShader);
	
	uint32 program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	int linkStatus = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus)
	{
		int length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		length++;
		
		char* errorStr = new char[length];
		glGetProgramInfoLog(program, length, NULL, errorStr);
		printf("glLink failure! Error: %s", errorStr);

		delete[] errorStr;
	}

	ShaderProgram newProgram(program, name, vertShaderName, fragShaderName);

	return newProgram;
}

GLuint ShaderManager::GetUniformLocation(std::string_view name)
{
	SDL_assert(m_currentShader);
	GLuint location = glGetUniformLocation(m_currentShader->program, name.data());
	//SDL_assert(location != -1);

	#ifdef LOG_INVALID_UNIFORM
	if (location == -1)
		printf("Could not find uniform %s in shader program %s\n", name, m_currentShader->name.c_str());
	#endif

	return location;
}

