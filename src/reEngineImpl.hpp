#pragma once

#include "reRenderer.hpp"
#include "reModelManager.hpp"
#include "reShaderManager.hpp"
#include "reMaterialManager.hpp"
#include "reTime.hpp"
#include "reInput.hpp"

class reEngine::Impl
{
public:
	Impl(const reGraphics::reRenderer& renderer, const reTime& time)
		: m_renderer(renderer)
		, m_time(time)
	{
	}

	reGraphics::reShaderManager m_shaderManager;
	reGraphics::reModelManager m_modelManager;
	reGraphics::reMaterialManager m_materialManager;
	reGraphics::reRenderer m_renderer;

	reTime m_time;
	reInput m_input;
};