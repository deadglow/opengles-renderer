#pragma once

#include "reMath.h"
#include "reTransform.h"

class reCamera
{
public:
	reCamera(float fov, float nearClip, float farClip)
		: m_transform(reTransform::Identity())
		, m_viewAngles(v2_zero)
		, m_fov(fov)
		, m_nearClip(nearClip)
		, m_farClip(farClip)
	{}

	mat4_t GetPerspectiveMatrix(float aspectRatio) const
	{
		return m4_perspective(m_fov, aspectRatio, m_nearClip, m_farClip);
	}

	void ApplyViewAngles()
	{
		rotor3_t pitch = rot3_plane_angle(bv3_yz, m_viewAngles.x);
		rotor3_t yaw = rot3_plane_angle(bv3_zx, m_viewAngles.y);
		m_transform.rotation = rot3_mul(yaw, pitch);
	};

	reTransform m_transform;
	vec2_t m_viewAngles;

private:
	float m_fov;
	float m_nearClip;
	float m_farClip;
};