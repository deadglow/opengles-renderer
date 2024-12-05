#pragma once
#include "Listener.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

namespace Enoki
{
	class CharacterController
	{
	public:
		struct MotionData
		{
			vec3_t velocity;
			vec3_t up;
		};

	public:

		CharacterController(const MotionData& motionData);
		void ApplyMotion(const vec3_t& accel, float dt);
		virtual void SetPosition(const vec3_t& pos);
		void MoveAndCollide(const vec3_t& moveVector);
		float GetDistanceFromGround() const;
		
		Transform transform{ Transform::Identity() };
		MotionData motionData;
	};
	
}
