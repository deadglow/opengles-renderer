#pragma once

#include "CharacterController.hpp"
#include "Camera.hpp"

namespace Enoki
{
	class FPView
	{
	public:
		struct FPInput
		{
			vec2_t m_axes;
		};

		struct FPViewSettings
		{
			float camHeight = 1.6f;
			float maxPitch = 80.f;
			float sensitivity = 0.1f;
		};

		FPView(const FPViewSettings& viewSettings) : settings(viewSettings)	{	}

		void UpdateView(const FPInput& fpInput);

		void ApplyToCamera(Camera& camera) const;

		FPViewSettings settings;
		vec2_t viewAngles { 0.f, 0.f };
	};

	class PlayerController : public CharacterController
	{
	public:
		struct MovementInput
		{
			vec2_t moveInput;
		};

		struct PlayerMotionData
		{
			vec3_t gravity;
		};

	public:
		PlayerController(const MotionData& motionData, const PlayerMotionData& playerMotionData, const FPView::FPViewSettings& viewSettings, const Camera::CameraSettings& cameraSettings);

		void MoveWithInput(const MovementInput& movementInput, float dt);
		virtual void SetPosition(const vec3_t& pos) override;
		void UpdateViewAndCamera(const FPView::FPInput& fpInput);

	public:
		Camera camera;
		FPView fpView;
		PlayerMotionData playerMotionData;
	};
}