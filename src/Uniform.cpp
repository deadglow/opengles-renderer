#include "Uniform.hpp"

using namespace Enoki::Graphics;

UniformValue::UniformValue(const bool& val)
	: type(UniformType::Bool)
	, v_int{ val, 0, 0, 0 }
{
}

UniformValue::UniformValue(const int& val)
	: type(UniformType::Int)
	, v_int{ val, 0, 0, 0 }
{
}

UniformValue::UniformValue(const float& val)
	: type(UniformType::Float)
	, v_float{ val, 0.f, 0.f, 0.f }
{
}

UniformValue::UniformValue(const vec2_t& val)
	: type(UniformType::Vec2)
	, v_float{ val.x, val.y, 0.f, 0.f}
{
}

UniformValue::UniformValue(const vec3_t& val)
	: type(UniformType::Vec3)
	, v_float{ val.x, val.y, val.z, 0.f}
{
}

UniformValue::UniformValue(const vec4_t& val)
	: type(UniformType::Vec4)
	, v_float{ val.x, val.y, val.z, val.w}
{
}
