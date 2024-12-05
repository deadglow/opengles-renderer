#include "Physics.hpp"

#include "SDL/SDL.h"

#include "PhysicsFallback.hpp"

#if defined(PHYSICS_USE_JOLT)
	#include "PhysicsJolt.hpp"
#endif

using namespace Enoki::Physics;

void PhysicsSystem::Init()
{
#if defined(PHYSICS_USE_JOLT)
	m_impl = new PhysicsJolt();
#else
	m_impl = new PhysicsFallback();
#endif
	// we need a physics implementation
	SDL_assert(m_impl != nullptr);

	m_impl->Init();
}

void PhysicsSystem::Dispose()
{
	if (m_impl)
		m_impl->Dispose();
}

void PhysicsSystem::Update(const float dt)
{
	SDL_assert(m_impl);
	m_impl->Update(dt);
}

void PhysicsSystem::Update()
{
	Update(m_timestep);
}

void PhysicsSystem::DebugRender(bool bodies, bool constraints)
{
	if (m_impl)
		m_impl->DebugRender(bodies, constraints);
}
