#include "reMaterial.hpp"
#include "reEngine.hpp"
#include "reShaderManager.hpp"

using namespace reGraphics;

void reMaterial::UseMaterial() const
{
	reShaderManager* shaderManager = reEngine::GetShaderManager();
	shaderManager->UseShader(m_shaderName.c_str());

	for (auto& uniform : m_shaderUniforms)
	{
		ApplyUniform(uniform.name.c_str(), uniform.value);
	}
}

void reMaterial::AddUniform(const char* name, const reUniformValue& value)
{
	m_shaderUniforms.push_back({ name, value });
}

void reMaterial::ApplyUniform(const char* uniformName, const reUniformValue& uniformValue) const
{
	reShaderManager* shaderManager = reEngine::GetShaderManager();
	
	switch (uniformValue.type)
	{
	case reUniformType::Bool:
	case reUniformType::Int:
	{
		const int* val = uniformValue.v_int;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case reUniformType::Float:
	{
		const float* val = uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case reUniformType::Vec2:
	{
		const vec2_t* val = (const vec2_t*)uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case reUniformType::Vec3:
	{
		const vec3_t* val = (const vec3_t*)uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case reUniformType::Vec4:
	{
		const vec4_t* val = (const vec4_t*)uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;
		
	default:
		break;
	}
}


