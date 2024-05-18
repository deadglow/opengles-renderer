#include "reCharacterController.hpp"

void reCharacterController::ApplyMotion(float dt)
{
	m_motionData.velocity = v3_add(m_motionData.velocity, v3_muls(m_motionData.gravity, dt));
	vec3_t moveVector = v3_muls(m_motionData.velocity, dt);

	m_transform.position = v3_add(m_transform.position, moveVector);
}

void reCharacterController::MoveAndCollide(const vec3_t& moveVector)
{
	vec3_t finalPos = v3_add(m_transform.position, moveVector);
	finalPos.y = fmaxf(finalPos.y, 0.f);
	m_transform.position = finalPos;
}

float reCharacterController::GetDistanceFromGround() const
{
	return m_transform.position.y;
}