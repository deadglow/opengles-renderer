#include "reUniform.hpp"

using namespace reGraphics;

reUniformValue::reUniformValue(const bool& val)
	: type(reUniformType::Bool)
	, v_int{ val, 0, 0, 0 }
{
}

reUniformValue::reUniformValue(const int& val)
	: type(reUniformType::Int)
	, v_int{ val, 0, 0, 0 }
{
}

reUniformValue::reUniformValue(const float& val)
	: type(reUniformType::Float)
	, v_float{ val, 0.f, 0.f, 0.f }
{
}

reUniformValue::reUniformValue(const vec2_t& val)
	: type(reUniformType::Vec2)
	, v_float{ val.x, val.y, 0.f, 0.f}
{
}

reUniformValue::reUniformValue(const vec3_t& val)
	: type(reUniformType::Vec3)
	, v_float{ val.x, val.y, val.z, 0.f}
{
}

reUniformValue::reUniformValue(const vec4_t& val)
	: type(reUniformType::Vec4)
	, v_float{ val.x, val.y, val.z, val.w}
{
}
