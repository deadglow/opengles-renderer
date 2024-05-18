#pragma once

#include "reCamera.hpp"

class reFlyCam
{
public:
	reFlyCam(float fov, float nearClip, float farClip)
		: m_camera(fov, nearClip, farClip)
		, m_viewAngles(v2_zero)
	{}

	void ApplyViewAngles();
	void MoveWithInput(const vec2_t& camInput, const vec3_t& moveInput, float dt);

	reCamera m_camera;
	vec2_t m_viewAngles;

	float m_turnSpeed = 70 * M_DEG2RAD;
	float m_speed = 10;
};