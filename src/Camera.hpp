#pragma once

#include "Math.h"
#include "Transform.hpp"

namespace Enoki
{
	class Camera
	{
	public:
		struct CameraSettings
		{
			float fov;
			float nearClip;
			float farClip;
		};

	public:
		Camera(const CameraSettings& settings)
			: transform(Transform::Identity())
			, settings(settings)
		{}

		mat4_t GetPerspectiveMatrix(float aspectRatio) const
		{
			return m4_perspective(settings.fov, aspectRatio, settings.nearClip, settings.farClip);
		}

		Transform transform;
		CameraSettings settings;
	};
}