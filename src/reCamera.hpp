#pragma once

#include "math_3d.h"

class reCamera
{
public:
	reCamera(float fov, float nearClip, float farClip)
		: m_position(vec3(0, 0, 0))
		, m_forward(vec3(0, 0, -1))
		, m_up(vec3(0, 1, 0))
		, m_fov(fov)
		, m_nearClip(nearClip)
		, m_farClip(farClip)
	{}

	mat4_t GetTransform() const
	{
		const vec3_t& p = m_position;
		const vec3_t& f = m_forward;
		const vec3_t& u = m_up;
		const vec3_t r = GetRight();

		return mat4(r.x, u.x, f.x, p.x,
			        r.y, u.y, f.y, p.y,
			        r.z, u.z, f.z, p.z,
			          0,   0,   0,   1
		);
	}

	mat4_t GetPerspectiveMatrix(float aspectRatio) const
	{
		return m4_perspective(m_fov, aspectRatio, m_nearClip, m_farClip);
	}

	vec3_t GetRight() const
	{
		return v3_cross(m_forward, m_up);
	}

	void LookAt(const vec3_t& point, const vec3_t& up)
	{
		mat4_t mat = m4_look_at(m_position, point, up);
		m_forward = (const vec3_t&)mat.m[2][0];
		m_up = (const vec3_t&)mat.m[1][0];
	}
	
	void LookAt(const vec3_t& point)
	{
		LookAt(point, vec3(0, 1, 0));
	}

	void Translate(const vec3_t& delta)
	{
		m_position = v3_add(m_position, delta);
	}

	void Rotate(const vec3_t& axis, float radians)
	{
		mat4_t rotMatrix = m4_rotation(radians, axis);
		m_forward = m4_mul_dir(rotMatrix, m_forward);
		m_up = m4_mul_dir(rotMatrix, m_up);
	}

	vec3_t m_position;
	vec3_t m_forward;
	vec3_t m_up;

private:

	float m_fov;
	float m_nearClip;
	float m_farClip;
};