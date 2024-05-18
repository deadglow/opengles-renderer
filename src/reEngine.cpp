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

rePhysics::rePhysicsSystem* reEngine::GetPhysicsSystem()
{
	return &m_instance->m_physicsSystem;
}

reTime* reEngine::GetTime()
{
	return &m_instance->m_time;
}

reInput* reEngine::GetInput()
{
	return &m_instance->m_input;
}

reDebugRender* reEngine::GetDebugRender()
{
#if defined(ENABLE_DEBUG_RENDER)
	return &m_instance->m_debugRender;
#else
	return nullptr;
#endif
}
