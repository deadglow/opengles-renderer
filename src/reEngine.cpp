#include "reEngine.hpp"
#include "reEngineImpl.hpp"

reEngine::Impl* reEngine::m_instance = nullptr;

reEngine::Impl* reEngine::GetInstance()
{
	return m_instance;
}

void reEngine::SetInstance(reEngine::Impl* instance)
{
	m_instance = instance;
}

reGraphics::reShaderManager* reEngine::GetShaderManager()
{
	return &m_instance->m_shaderManager;
}

reGraphics::reModelManager* reEngine::GetModelManager()
{
	return &m_instance->m_modelManager;
}

reGraphics::reMaterialManager* reEngine::GetMaterialManager()
{
	return &m_instance->m_materialManager;
}

reGraphics::reRenderer* reEngine::GetRenderer()
{
	return &m_instance->m_renderer;
}

reTime* reEngine::GetTime()
{
	return &m_instance->m_time;
}

reInput* reEngine::GetInput()
{
	return &m_instance->m_input;
}