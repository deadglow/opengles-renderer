#pragma once

#include <string>
#include "Math.h"

namespace Enoki::Graphics
{
	enum class UniformType : char
	{
		Invalid,
		Bool,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
	};

	struct UniformValue {
		UniformValue() = default;
		UniformValue(const bool& val);
		UniformValue(const int& val);
		UniformValue(const float& val);
		UniformValue(const vec2_t& val);
		UniformValue(const vec3_t& val);
		UniformValue(const vec4_t& val);

		UniformType type;
		union {
			float v_float[4];
			int v_int[4];
		};
	};

	struct Uniform {
		const std::string name;
		UniformValue value;
	};
};