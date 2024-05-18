#include "reFlyCam.hpp"

void reFlyCam::ApplyViewAngles()
{
	rotor3_t pitch = rot3_plane_angle(bv3_yz, m_viewAngles.x);
	rotor3_t yaw = rot3_plane_angle(bv3_zx, m_viewAngles.y);
	m_camera.m_transform.rotation = rot3_mul(yaw, pitch);
}

void reFlyCam::MoveWithInput(const vec2_t& camInput, const vec3_t& moveInput, float dt)
{
	m_viewAngles.x += m_turnSpeed * camInput.y * dt;
	m_viewAngles.y += m_turnSpeed * -camInput.x * dt;
	ApplyViewAngles();

	reTransform& camTransform = m_camera.m_transform;

	vec3_t deltaX = v3_muls(v3_right, moveInput.x);
	vec3_t deltaZ = v3_muls(v3_forward, moveInput.z);
	vec3_t deltaXZ = v3_norm(v3_add(deltaX, deltaZ));
	deltaXZ = rot3_transform(camTransform.rotation, deltaXZ);

	vec3_t deltaUp = v3_muls(v3_up, moveInput.y);
	
	vec3_t translation = v3_muls(v3_add(deltaXZ, deltaUp), m_speed * dt);
	camTransform.position = v3_add(camTransform.position, translation);
}