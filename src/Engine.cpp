#include "Engine.hpp"
#include "EngineImpl.hpp"

using namespace Enoki;

Engine::Impl* Engine::m_instance = nullptr;

Engine::Impl* Engine::GetInstance()
{
	return m_instance;
}

void Engine::SetInstance(Engine::Impl* instance)
{
	m_instance = instance;
}

Graphics::ShaderManager* Engine::GetShaderManager()
{
	return &m_instance->shaderManager;
}

Graphics::ModelManager* Engine::GetModelManager()
{
	return &m_instance->modelManager;
}

Graphics::MaterialManager* Engine::GetMaterialManager()
{
	return &m_instance->materialManager;
}

Graphics::Renderer* Engine::GetRenderer()
{
	return &m_instance->renderer;
}

Physics::PhysicsSystem* Engine::GetPhysicsSystem()
{
	return &m_instance->physicsSystem;
}

Time* Engine::GetTime()
{
	return &m_instance->time;
}

Input* Engine::GetInput()
{
	return &m_instance->input;
}

Graphics::DebugRender* Engine::GetDebugRender()
{
#if defined(ENABLE_DEBUG_RENDER)
	return &m_instance->debugRender;
#else
	return nullptr;
#endif
}
