#pragma once
#include "reListener.hpp"
#include "reTransform.hpp"
#include "reCamera.hpp"

class reCharacterController
{
public:
	struct MotionData
	{
		vec3_t velocity;
		vec3_t gravity;
		vec3_t up;
	};
public:

	void ApplyMotion(float dt);
	void MoveAndCollide(const vec3_t& moveVector);
	float GetDistanceFromGround() const;
	
	reTransform m_transform;
	MotionData m_motionData;
};
