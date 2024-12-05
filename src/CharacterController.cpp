#include "CharacterController.hpp"

using namespace Enoki;

CharacterController::CharacterController(const MotionData& motionData)
	: motionData(motionData)
{
}

void CharacterController::ApplyMotion(const vec3_t& accel, float dt)
{
	vec3_t moveVector = v3_muls(motionData.velocity, dt);
	MoveAndCollide(moveVector);

	motionData.velocity = v3_add(motionData.velocity, accel);
}

void CharacterController::SetPosition(const vec3_t& pos)
{
	transform.position = pos;
}

void CharacterController::MoveAndCollide(const vec3_t& moveVector)
{
	vec3_t finalPos = v3_add(transform.position, moveVector);
	finalPos.y = fmaxf(finalPos.y, 0.f);
	SetPosition(finalPos);
}

float CharacterController::GetDistanceFromGround() const
{
	return transform.position.y;
}