#include "PlayerController.hpp"
#include "Time.hpp"
#include "Engine.hpp"

using namespace Enoki;

PlayerController::PlayerController(const MotionData& motionData, const PlayerMotionData& playerMotionData, const FPView::FPViewSettings& viewSettings, const Camera::CameraSettings& cameraSettings)
	: CharacterController(motionData)
	, playerMotionData(playerMotionData)
	, fpView(viewSettings)
	, camera(cameraSettings)
{
}

void PlayerController::MoveWithInput(const MovementInput& moveInput, float dt)
{
	rotor3_t yaw = rot3_plane_angle(bv3_zx, fpView.viewAngles.y * M_DEG2RAD);
	vec3_t forward = rot3_transform(yaw, v3_forward);

	vec3_t right = v3_cross(v3_up, forward);

	vec3_t deltaX = v3_muls(right, moveInput.moveInput.x);
	vec3_t deltaZ = v3_muls(forward, moveInput.moveInput.y);
	vec3_t deltaXZ = v3_norm(v3_add(deltaX, deltaZ));

	float moveSpeed = 10.f;
	float maxAcceleration = 60.0f;
	float frameAcceleration = maxAcceleration * dt;

	vec3_t targetVelocity = v3_muls(deltaXZ, moveSpeed);
	vec3_t curVelocity = motionData.velocity;

	vec3_t deltaVelocity = v3_sub(targetVelocity, curVelocity);
	float magnitude = v3_length(deltaVelocity);

	if (magnitude > frameAcceleration)
	{
		deltaVelocity = v3_muls(v3_divs(deltaVelocity, magnitude), frameAcceleration);
	}

	ApplyMotion(deltaVelocity, dt);
}

void PlayerController::SetPosition(const vec3_t& pos)
{
	CharacterController::SetPosition(pos);
	camera.transform.position = v3_add(transform.position, v3_muls(transform.GetUp(), fpView.settings.camHeight));
}

void PlayerController::UpdateViewAndCamera(const FPView::FPInput& fpInput)
{
	fpView.UpdateView(fpInput);
	fpView.ApplyToCamera(camera);
}

void FPView::UpdateView(const FPInput& fpInput)
{
	viewAngles = v2_add(viewAngles, v2_muls(fpInput.m_axes, settings.sensitivity));
	viewAngles.x = clamp(viewAngles.x, -settings.maxPitch, settings.maxPitch);
}

void FPView::ApplyToCamera(Camera& camera) const
{
	rotor3_t pitch = rot3_plane_angle(bv3_yz, viewAngles.x * M_DEG2RAD);
	rotor3_t yaw = rot3_plane_angle(bv3_zx, viewAngles.y * M_DEG2RAD);
	camera.transform.rotation = rot3_mul(yaw, pitch);
}
