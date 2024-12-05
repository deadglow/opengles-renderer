#include "Material.hpp"
#include "Engine.hpp"
#include "ShaderManager.hpp"

using namespace Enoki;
using namespace Enoki::Graphics;

void Material::UseMaterial() const
{
	ShaderManager* shaderManager = Engine::GetShaderManager();
	shaderManager->UseShader(m_shaderName.c_str());

	for (auto& uniform : m_shaderUniforms)
	{
		ApplyUniform(uniform.name.c_str(), uniform.value);
	}
}

void Material::AddUniform(const char* name, const UniformValue& value)
{
	m_shaderUniforms.push_back({ name, value });
}

void Material::ApplyUniform(const char* uniformName, const UniformValue& uniformValue) const
{
	ShaderManager* shaderManager = Engine::GetShaderManager();
	
	switch (uniformValue.type)
	{
	case UniformType::Bool:
	case UniformType::Int:
	{
		const int* val = uniformValue.v_int;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case UniformType::Float:
	{
		const float* val = uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case UniformType::Vec2:
	{
		const vec2_t* val = (const vec2_t*)uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case UniformType::Vec3:
	{
		const vec3_t* val = (const vec3_t*)uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;

	case UniformType::Vec4:
	{
		const vec4_t* val = (const vec4_t*)uniformValue.v_float;
		shaderManager->SetUniform(uniformName, val);
	}
	break;
		
	default:
		break;
	}
}


