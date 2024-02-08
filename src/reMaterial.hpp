#pragma once

#include <vector>
#include <string>
#include "math_3d.h"

namespace reGraphics
{
	class reShaderManager;

	enum class reUniformType : char
	{
		Invalid,
		Bool,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
	};

	struct reUniformValue {
		reUniformValue() = default;
		reUniformValue(const bool& val);
		reUniformValue(const int& val);
		reUniformValue(const float& val);
		reUniformValue(const vec2_t& val);
		reUniformValue(const vec3_t& val);
		reUniformValue(const vec4_t& val);

		reUniformType type;
		union {
			float v_float[4];
			int v_int[4];
		};
	};

	struct reUniformSet {
		std::string name;
		reUniformValue value;
	};

	class reMaterial
	{
	public:
		reMaterial(const char* shaderName);

		void UseMaterial();

		void AddUniform(const std::string& name, const reUniformValue& value);

	private:
		void ApplyUniform(const char* uniformName, const reUniformValue& uniformValue);

	private:
		std::string m_shaderName;
		std::vector<reUniformSet> m_shaderUniforms;
	};
};