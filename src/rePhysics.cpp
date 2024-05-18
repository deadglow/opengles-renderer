#include "rePhysics.hpp"

#include "SDL/SDL.h"

#define PHYSICS_USE_JOLT

#if defined(PHYSICS_USE_JOLT)
	#include "rePhysicsJolt.hpp"
#endif

using namespace rePhysics;

void rePhysicsSystem::Init()
{
#if defined(PHYSICS_USE_JOLT)
	m_impl = new rePhysicsJolt();
#else
	// we need a physics implementation
	SDL_assert(m_impl != nullptr);
#endif

	m_impl->Init();
}

void rePhysicsSystem::Dispose()
{
	if (m_impl)
		m_impl->Dispose();
}

void rePhysicsSystem::Update(const float dt)
{
	SDL_assert(m_impl);
	m_impl->Update(dt);
}

void rePhysicsSystem::Update()
{
	Update(m_timestep);
}

void rePhysicsSystem::DebugRender(bool bodies, bool constraints)
{
	if (m_impl)
		m_impl->DebugRender(bodies, constraints);
}
