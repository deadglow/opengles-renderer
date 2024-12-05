#include "PhysicsJolt.hpp"

#include <SDL/SDL.h>

#include <iostream>
#include <cstdarg>
#include <thread>

#include "Math.h"

using namespace Enoki;
using namespace Enoki::Physics;

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
static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint32 inLine)
{
	JPH::Trace("%s:%i: (%s) %s\n", inFile, (int)inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
	SDL_assert(false);

	return true;
}
#endif //JPH_ENABLE_ASSERTS
// __TRACE //


// LISTENER //
JPH::ValidateResult ContactListener::OnContactValidate(const JPH::Body& bodyA, const JPH::Body& bodyB, JPH::RVec3Arg baseOffset, const JPH::CollideShapeResult& collisionResult)
{
	printf("Contact Validate Callback\n");

	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListener::OnContactAdded(const JPH::Body& bodyA, const JPH::Body& bodyB, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings)
{
	printf("Contact added\n");
}

void ContactListener::OnContactPersisted(const JPH::Body& bodyA, const JPH::Body& bodyB, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings)
{
	printf("Contact persisted\n");
}

void ContactListener::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair)
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
void PhysicsJolt::Init()
{
	JPH::RegisterDefaultAllocator();

	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl);

	JPH::Factory::sInstance = new JPH::Factory();

	// register physics types and install collision handlers
	JPH::RegisterTypes();

	m_tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
	m_jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, (int)std::thread::hardware_concurrency() - 1 );

	physicsSystem = new JPH::PhysicsSystem();
	physicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, m_broadPhaseLayerInterface, m_objectVsBroadphaseLayerFilter, m_objectVsObjectLayerFilter);
	physicsSystem->SetContactListener(&m_contactListener);

	m_debugRenderer = new JoltDebugRenderer();

	// temp stuff
	{
		// interact with bodies here
		JPH::BodyInterface& bodyInterface = physicsSystem->GetBodyInterface();

		// floor //
		// make floor shape settings
		JPH::BoxShapeSettings floorShapeSettings(JPH::Vec3(100.f, 1.f, 100.f));
		floorShapeSettings.SetEmbedded(); // ref counted object on stack
		JPH::ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();
		JPH::ShapeRefC floorShape = floorShapeResult.Get();
		// create body settings
		JPH::BodyCreationSettings floorSettings(floorShape, JPH::RVec3(0.0_r, -1.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Static, (JPH::ObjectLayer)CollisionLayer::LAYER_STATIC);
		JPH::Body* floor = bodyInterface.CreateBody(floorSettings);
		bodyInterface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);

		// create a dynamic body to bounce on the floor
		JPH::BodyCreationSettings sphereSettings(new JPH::SphereShape(1.5f), JPH::RVec3(10.0_r, 2.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, (JPH::ObjectLayer)CollisionLayer::LAYER_DYNAMIC);
		JPH::BodyID sphereId = bodyInterface.CreateAndAddBody(sphereSettings, JPH::EActivation::Activate);

		bodyInterface.SetLinearVelocity(sphereId, JPH::RVec3(-50.0_r, 0.0_r, 0.0_r));

		// create coobs
		JPH::BoxShapeSettings boxShapeSettings(JPH::Vec3(2.f, 2.f, 2.f));
		boxShapeSettings.SetEmbedded();
		JPH::ShapeSettings::ShapeResult boxShapeResult = boxShapeSettings.Create();
		JPH::ShapeRefC boxShape = boxShapeResult.Get();

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
			JPH::BodyCreationSettings boxSettings(boxShape, JPH::RVec3(v.x, v.y, v.z), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, (JPH::ObjectLayer)CollisionLayer::LAYER_DYNAMIC);
			JPH::BodyID boxId = bodyInterface.CreateAndAddBody(boxSettings, JPH::EActivation::Activate);
		}
	}

	// improves collision detection performance, call this infrequently cos it's slow
	physicsSystem->OptimizeBroadPhase();
}

void PhysicsJolt::Dispose()
{
	delete m_debugRenderer;
	delete physicsSystem;
	delete m_jobSystem;
	delete m_tempAllocator;

	JPH::UnregisterTypes();

	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void PhysicsJolt::Update(const float dt)
{
	physicsSystem->Update(dt, m_collisionSteps, m_tempAllocator, m_jobSystem);
}

void PhysicsJolt::DebugRender(bool bodies, bool constraints)
{
	if (bodies)
		m_debugRenderer->DrawBodies(physicsSystem);

	if (constraints)
		m_debugRenderer->DrawConstraints(physicsSystem);
}

// __INTERFACE IMPL //