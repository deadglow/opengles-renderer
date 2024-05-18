#include "rePhysicsJolt.hpp"

#include <SDL/SDL.h>

#include <iostream>
#include <cstdarg>
#include <thread>

#include "reMath.h"

using namespace rePhysics;

using namespace JPH;
using namespace JPH::literals;

// TRACE //
static void TraceImpl(const char* inFMT, ...)
{
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	printf(buffer);
}

#ifdef JPH_ENABLE_ASSERTS
static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine)
{
	JPH::Trace("%s:%i: (%s) %s\n", inFile, (int)inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
	SDL_assert(false);

	return true;
}
#endif //JPH_ENABLE_ASSERTS
// __TRACE //


// LISTENER //
JPH::ValidateResult reContactListener::OnContactValidate(const JPH::Body& bodyA, const JPH::Body& bodyB, JPH::RVec3Arg baseOffset, const JPH::CollideShapeResult& collisionResult)
{
	printf("Contact Validate Callback\n");

	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void reContactListener::OnContactAdded(const JPH::Body& bodyA, const JPH::Body& bodyB, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings)
{
	printf("Contact added\n");
}

void reContactListener::OnContactPersisted(const JPH::Body& bodyA, const JPH::Body& bodyB, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings)
{
	printf("Contact persisted\n");
}

void reContactListener::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair)
{
	printf("Contact removed\n");
}

void reBodyActivationListener::OnBodyActivated(const JPH::BodyID& bodyId, uint64_t bodyUserData)
{
	printf("A body got activated\n");
};

void reBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& bodyId, uint64_t bodyUserData)
{
	printf("A body went to sleep\n");
};

// __LISTENER //


// INTERFACE IMPL //
void rePhysicsJolt::Init()
{
	RegisterDefaultAllocator();

	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl);

	Factory::sInstance = new Factory();

	// register physics types and install collision handlers
	RegisterTypes();

	m_tempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);
	m_jobSystem = new JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, (int)std::thread::hardware_concurrency() - 1 );

	m_physicsSystem = new PhysicsSystem();
	m_physicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, m_broadPhaseLayerInterface, m_objectVsBroadphaseLayerFilter, m_objectVsObjectLayerFilter);
	m_physicsSystem->SetContactListener(&m_contactListener);

	m_debugRenderer = new reJoltDebugRenderer();

	// temp stuff
	{
		// interact with bodies here
		BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();

		// floor //
		// make floor shape settings
		BoxShapeSettings floorShapeSettings(Vec3(100.f, 1.f, 100.f));
		floorShapeSettings.SetEmbedded(); // ref counted object on stack
		ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();
		ShapeRefC floorShape = floorShapeResult.Get();
		// create body settings
		BodyCreationSettings floorSettings(floorShape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, reCollisionLayer::LAYER_STATIC);
		Body* floor = bodyInterface.CreateBody(floorSettings);
		bodyInterface.AddBody(floor->GetID(), EActivation::DontActivate);

		// create a dynamic body to bounce on the floor
		BodyCreationSettings sphereSettings(new SphereShape(1.5f), RVec3(10.0_r, 2.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, reCollisionLayer::LAYER_DYNAMIC);
		BodyID sphereId = bodyInterface.CreateAndAddBody(sphereSettings, EActivation::Activate);

		bodyInterface.SetLinearVelocity(sphereId, RVec3(-50.0_r, 0.0_r, 0.0_r));

		// create coobs
		BoxShapeSettings boxShapeSettings(Vec3(2.f, 2.f, 2.f));
		boxShapeSettings.SetEmbedded();
		ShapeSettings::ShapeResult boxShapeResult = boxShapeSettings.Create();
		ShapeRefC boxShape = boxShapeResult.Get();

		vec3_t pos[6] = {
			{ 0.f, 3.f, 0.f },
			{ 0.f, 5.f, 0.f },
			{ 0.f, 7.f, 0.f },
			{ 0.f, 9.f, 0.f },
			{ 0.f, 12.f, 0.f },
			{ 0.f, 15.f, 0.f },
		};
		for (int i = 0; i < 6; ++i)
		{
			vec3_t v = pos[i];
			BodyCreationSettings boxSettings(boxShape, RVec3(v.x, v.y, v.z), Quat::sIdentity(), EMotionType::Dynamic, reCollisionLayer::LAYER_DYNAMIC);
			BodyID boxId = bodyInterface.CreateAndAddBody(boxSettings, EActivation::Activate);
		}
	}

	// improves collision detection performance, call this infrequently cos it's slow
	m_physicsSystem->OptimizeBroadPhase();
}

void rePhysicsJolt::Dispose()
{
	delete m_debugRenderer;
	delete m_physicsSystem;
	delete m_jobSystem;
	delete m_tempAllocator;

	UnregisterTypes();

	delete Factory::sInstance;
	Factory::sInstance = nullptr;
}

void rePhysicsJolt::Update(const float dt)
{
	m_physicsSystem->Update(dt, m_collisionSteps, m_tempAllocator, m_jobSystem);
}

void rePhysicsJolt::DebugRender(bool bodies, bool constraints)
{
	if (bodies)
		m_debugRenderer->DrawBodies(m_physicsSystem);

	if (constraints)
		m_debugRenderer->DrawConstraints(m_physicsSystem);
}

// __INTERFACE IMPL //