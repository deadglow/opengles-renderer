#pragma once

#include "Camera.hpp"

namespace Enoki
{
	class FlyCam
	{
	public:
		FlyCam(const Camera::CameraSettings& camSettings)
			: camera(camSettings)
			, viewAngles(v2_zero)
		{}

		void ApplyViewAngles();
		void MoveWithInput(const vec2_t& camInput, const vec3_t& moveInput, float dt);

		Camera camera;
		vec2_t viewAngles;

		float turnSpeed = 70 * M_DEG2RAD;
		float speed = 10;
	};
}