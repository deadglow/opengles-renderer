#pragma once
#include "reMath.h"

struct reTransform
{
public:
	vec3_t position;
	rotor3_t rotation;
	float scale;

	static reTransform Identity() { return { v3_zero, rot3_identity(), 1.f }; }

	void Reset() {
		*this = Identity();
	}

	vec3_t GetRight() const { return rot3_transform(rotation, v3_right); }
	vec3_t GetUp() const { return rot3_transform(rotation, v3_up); }
	vec3_t GetForward() const { return rot3_transform(rotation, v3_forward); }

	mat4_t ConstructMatrix() const
	{
		return m4_trs(position, rotation, scale);
	}

	void Rotate(const rotor3_t& rotor)
	{
		rotation = rot3_mul(rotor, rotation);
	}
};