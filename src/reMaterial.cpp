#include "reMaterial.hpp"
#include "reEngine.hpp"
#include "reShaderManager.hpp"

using namespace reGraphics;

reGraphics::reUniformValue::reUniformValue(const bool& val)
	: type(reUniformType::Bool)
	, v_int{ val, 0, 0, 0 }
{
}

reGraphics::reUniformValue::reUniformValue(const int& val)
	: type(reUniformType::Int)
	, v_int{ val, 0, 0, 0 }
{
}

reGraphics::reUniformValue::reUniformValue(const float& val)
	: type(reUniformType::Float)
	, v_float{ val, 0.f, 0.f, 0.f }
{
}

reGraphics::reUniformValue::reUniformValue(const vec2_t& val)
	: type(reUniformType::Vec2)
	, v_float{ val.x, val.y, 0.f, 0.f}
{
}

reGraphics::reUniformValue::reUniformValue(const vec3_t& val)
	: type(reUniformType::Vec3)
	, v_float{ val.x, val.y, val.z, 0.f}
{
}

reGraphics::reUniformValue::reUniformValue(const vec4_t& val)
	: type(reUniformType::Vec4)
	, v_float{ val.x, val.y, val.z, val.w}
{
}

reMaterial::reMaterial(const char* shaderName)
	: m_shaderName(shaderName)
{
}

void reMaterial::UseMaterial()
{
	reShaderManager* shaderManager = reEngine::GetShaderManager();
	shaderManager->UseShader(m_shaderName.c_str());

	for (auto& uniform : m_shaderUniforms)
	{
		ApplyUniform(uniform.name.c_str(), uniform.value);
	}
}

void reMaterial::AddUniform(const std::string& name, const reUniformValue& value)
{
	m_shaderUniforms.push_back({ name, value });
}

void reMaterial::ApplyUniform(const char* uniformName, const reUniformValue& uniformValue)
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


