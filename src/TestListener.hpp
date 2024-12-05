#pragma once

#include "Listener.hpp"
#include "PlayerController.hpp"
#include "FlyCam.hpp"
#include "Camera.hpp"

namespace Enoki
{
	class TestListener : public IListener
	{
	public:
		void OnRegister() override;
		void OnUpdate() override;
		void OnPreRender() override;

		// FlyCam m_flyCam = FlyCam(70.f, 0.001f, 1000.0f);
		PlayerController playerController{
			PlayerController::MotionData { v3_zero, v3_up },
			PlayerController::PlayerMotionData { vec3(0.f, -9.81f, 0.f) },
			FPView::FPViewSettings(),
			Camera::CameraSettings { 70.f, 0.001f, 1000.0f }
		};
	};
}