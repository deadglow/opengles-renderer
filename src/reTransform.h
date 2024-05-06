#pragma once
#include "reMath.h"

struct reTransform
{
	vec3_t position;
	rotor3_t rotation;
	float scale;

	void Reset()
	{
		position = v3_zero;
		rotation = rot3_identity();
		scale = 1.f;
	}

	mat4_t ComputeMatrix() const
	{
		return m4_trs(position, rotation, scale);
	}

	void Rotate(const rotor3_t& rotor)
	{
		rotation = rot3_mul(rotor, rotation);
	}
};