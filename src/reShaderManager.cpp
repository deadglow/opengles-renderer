#include "reShaderManager.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

#include "gles2.h"
#include "SDL/SDL.h"

const char* SHADER_PATH = "assets\\shaders\\";
const char* DEFAULT_VS = "default";
const char* DEFAULT_FS = "default";
const char* LINES_VS = "lines";
const char* LINES_FS = "lines";

using namespace reGraphics;
//#define LOG_INVALID_UNIFORM

reShaderProgram::reShaderProgram(GLuint program, const char* name, const char* vertShader, const char* fragShader)
	: program(program)
	, name(name)
	, vertShader(vertShader)
	, fragShader(fragShader)
{
}

reShaderManager::reShaderManager()
{
	LoadShaderProgramFromFiles("default", DEFAULT_VS, DEFAULT_FS);
	LoadShaderProgramFromFiles("lines", LINES_VS, LINES_FS);
}

bool reShaderManager::IsShaderLoaded(const char* name) const
{
	return m_shadermap.count(std::string(name)) > 0;
}

const reShaderProgram& reShaderManager::GetShader(const char* name) const
{
	SDL_assert(IsShaderLoaded(name));
	
	return m_shadermap.at(std::string(name));
}

void reShaderManager::ReloadShader(const char* name)
{
	SDL_assert(IsShaderLoaded(name));

	reShaderProgram& shader = m_shadermap[std::string(name)];

	glDeleteProgram(shader.program);

	shader = LoadShaderProgramFromFilesNoEmplace(shader.name.c_str(), shader.vertShader.c_str(), shader.fragShader.c_str());
}

void reShaderManager::ReloadAllShaders()
{
	for (auto [key, value] : m_shadermap)
	{
		ReloadShader(key.c_str());
	}
}

const reShaderProgram& reShaderManager::LoadShaderProgramFromFiles(const char* name, const char* vertShaderFilename, const char* fragShaderFilename)
{
	SDL_assert(!IsShaderLoaded(name));

	std::string nameStr(name);
	m_shadermap.emplace(nameStr, LoadShaderProgramFromFilesNoEmplace(name, vertShaderFilename, fragShaderFilename));
	
	return m_shadermap[nameStr];
}

void reShaderManager::UnloadShader(const char* name)
{
	SDL_assert(IsShaderLoaded(name));

	std::string nameStr(name);
	
	const reShaderProgram& program = m_shadermap[nameStr];
	glDeleteProgram(program.program);

	m_shadermap.erase(nameStr);
}

void reShaderManager::UnloadAllShaders()
{
	for (auto [key, value] : m_shadermap)
	{
		UnloadShader(key.c_str());
	}
}

void reShaderManager::UseShader(const reShaderProgram& shader)
{
	SDL_assert(shader.program);
	glUseProgram(shader.program);
	m_currentShader = &shader;
}

void reShaderManager::UseShader(const char* name)
{
	const reShaderProgram& shader = GetShader(name);
	UseShader(shader);
}

void reShaderManager::SetUniform(const char* name, const int32_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform1iv(location, count, value);
}

void reShaderManager::SetUniform(const char* name, const float* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform1fv(location, count, value);
}

void reShaderManager::SetUniform(const char* name, const vec2_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform2fv(location, count, (const float*)value);
}

void reShaderManager::SetUniform(const char* name, const vec3_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform3fv(location, count, (const float*)value);
}

void reShaderManager::SetUniform(const char* name, const vec4_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform4fv(location, count, (const float*)value);
}

void reShaderManager::SetUniform(const char* name, const mat4_t* value, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, count, false, (const float*)value);
}

const reShaderProgram* reShaderManager::GetCurrentShader() const
{
	return m_currentShader;
}

reShaderProgram reShaderManager::LoadShaderProgramFromFilesNoEmplace(const char* name, const char* vertShaderFilename, const char* fragShaderFilename)
{
	// load vert/frag shaders
	GLuint vertShader = LoadShaderFromFile(vertShaderFilename, GL_VERTEX_SHADER);
	GLuint fragShader = LoadShaderFromFile(fragShaderFilename, GL_FRAGMENT_SHADER);

	reShaderProgram newProgram = ConstructShaderProgram(vertShader, fragShader, name, vertShaderFilename, fragShaderFilename);

	// clean up shaders
	glDetachShader(newProgram.program, vertShader);
	glDetachShader(newProgram.program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return newProgram;
}

GLuint reShaderManager::LoadShaderFromSource(const char* source, GLint glShaderType)
{
	SDL_assert(source);
	SDL_assert(glShaderType == GL_VERTEX_SHADER || glShaderType == GL_FRAGMENT_SHADER);

	GLuint glShader = glCreateShader(glShaderType);
	glShaderSource(glShader, 1, &source, NULL);
	glCompileShader(glShader);

	GLint compileStatus;
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

GLuint reShaderManager::LoadShaderFromFile(const char* filename, GLint glShaderType)
{
	SDL_assert(glShaderType == GL_VERTEX_SHADER || glShaderType == GL_FRAGMENT_SHADER);
	
	std::string relativePath(SHADER_PATH);
	relativePath.append(filename);
	
	const char* extension = glShaderType == GL_VERTEX_SHADER ? ".vs" : ".fs";
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

reShaderProgram reShaderManager::ConstructShaderProgram(GLuint vertShader, GLuint fragShader, const char* name, const char* vertShaderName, const char* fragShaderName)
{
	SDL_assert(vertShader && fragShader);
	
	GLuint program = glCreateProgram();
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

	reShaderProgram newProgram(program, name, vertShaderName, fragShaderName);

	return newProgram;
}

GLuint reShaderManager::GetUniformLocation(const char* name)
{
	SDL_assert(m_currentShader);
	GLuint location = glGetUniformLocation(m_currentShader->program, name);
	//SDL_assert(location != -1);

	#ifdef LOG_INVALID_UNIFORM
	if (location == -1)
		printf("Could not find uniform %s in shader program %s\n", name, m_currentShader->name.c_str());
	#endif

	return location;
}

