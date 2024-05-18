#pragma once

#include "reListener.hpp"
#include "reFlyCam.hpp"

class reTestListener : reIListener
{
public:
	void OnRegister() override;
	void OnUpdate() override;
	void OnPreRender() override;

	reFlyCam m_flyCam = reFlyCam(70.f, 0.001f, 1000.0f);
};