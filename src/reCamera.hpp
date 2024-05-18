#pragma once

#include "reMath.h"
#include "reTransform.hpp"

class reCamera
{
public:
	reCamera(float fov, float nearClip, float farClip)
		: m_transform(reTransform::Identity())
		, m_fov(fov)
		, m_nearClip(nearClip)
		, m_farClip(farClip)
	{}

	mat4_t GetPerspectiveMatrix(float aspectRatio) const
	{
		return m4_perspective(m_fov, aspectRatio, m_nearClip, m_farClip);
	}

	reTransform m_transform;

private:
	float m_fov;
	float m_nearClip;
	float m_farClip;
};