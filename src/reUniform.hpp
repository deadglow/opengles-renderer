#pragma once

#include <string>
#include "reMath.h"

namespace reGraphics
{
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

	struct reUniform {
		const std::string name;
		reUniformValue value;
	};
};