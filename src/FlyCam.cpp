#include "FlyCam.hpp"

using namespace Enoki;

void FlyCam::ApplyViewAngles()
{
	rotor3_t pitch = rot3_plane_angle(bv3_yz, viewAngles.x);
	rotor3_t yaw = rot3_plane_angle(bv3_zx, viewAngles.y);
	camera.transform.rotation = rot3_mul(yaw, pitch);
}

void FlyCam::MoveWithInput(const vec2_t& camInput, const vec3_t& moveInput, float dt)
{
	viewAngles.x += turnSpeed * camInput.y * dt;
	viewAngles.y += turnSpeed * -camInput.x * dt;
	ApplyViewAngles();

	Transform& camTransform = camera.transform;

	vec3_t deltaX = v3_muls(v3_right, moveInput.x);
	vec3_t deltaZ = v3_muls(v3_forward, moveInput.z);
	vec3_t deltaXZ = v3_norm(v3_add(deltaX, deltaZ));
	deltaXZ = rot3_transform(camTransform.rotation, deltaXZ);

	vec3_t deltaUp = v3_muls(v3_up, moveInput.y);
	
	vec3_t translation = v3_muls(v3_add(deltaXZ, deltaUp), speed * dt);
	camTransform.position = v3_add(camTransform.position, translation);
}