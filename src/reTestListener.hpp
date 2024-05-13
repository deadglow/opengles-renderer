#pragma once

#include "reListener.hpp"
#include "reCamera.hpp"

class reTestListener : reIListener
{
public:
	void OnRegister() override;
	void OnUpdate() override;
	void OnPreRender() override;

	reCamera m_camera = reCamera(70.f, 0.001f, 1000.0f);
};