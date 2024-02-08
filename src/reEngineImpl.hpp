#include "reRenderer.hpp"
#include "reModelManager.hpp"
#include "reShaderManager.hpp"
#include "reTime.hpp"
#include "reInput.hpp"

class reEngine::Impl
{
public:
	Impl(const reTime& time)
		: m_time(time)
	{
	}

	reGraphics::reShaderManager m_shaderManager;
	reGraphics::reModelManager m_modelManager;
	reTime m_time;
	reInput m_input;
};