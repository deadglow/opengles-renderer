#pragma once

#include "Physics.hpp"

#include "Jolt/Jolt.h"
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "JoltDebugRenderer.hpp"

// LAYERS //
namespace Enoki::Physics
{
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override
		{
			switch (CollisionLayer(a))
			{
			case CollisionLayer::LAYER_STATIC:
				return CollisionLayer(b) == CollisionLayer::LAYER_DYNAMIC;
			case CollisionLayer::LAYER_DYNAMIC:
				return true;

			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			m_objectToBroadPhase[(uint16_t)CollisionLayer::LAYER_STATIC] = JPH::BroadPhaseLayer((uint8_t)CollisionLayerBroad::LAYER_STATIC);
			m_objectToBroadPhase[(uint16_t)CollisionLayer::LAYER_DYNAMIC] = JPH::BroadPhaseLayer((uint8_t)CollisionLayerBroad::LAYER_DYNAMIC);
		}

		virtual uint32_t GetNumBroadPhaseLayers() const override
		{
			return (uint32_t)CollisionLayerBroad::LAYER_COUNT;
		}

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override
		{
			JPH_ASSERT(layer < (uint16_t)CollisionLayer::LAYER_COUNT);
			return m_objectToBroadPhase[layer];
		}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override
		{
			switch ((CollisionLayerBroad)(JPH::BroadPhaseLayer::Type)layer)
			{
			case CollisionLayerBroad::LAYER_STATIC: return "LAYER_STATIC";
			case CollisionLayerBroad::LAYER_DYNAMIC: return "LAYER_DYNAMIC";

			default:
				JPH_ASSERT(false);
				return "LAYER_INVALID";
			}
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer m_objectToBroadPhase[(uint16_t)CollisionLayer::LAYER_COUNT];
	};

	//  ObjectVsBroadPhaseLayerFilter //
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer broad) const override
		{
			const auto reObj = CollisionLayer(obj);
			const auto reBroad = (CollisionLayerBroad)(JPH::BroadPhaseLayer::Type)broad;
			switch (reObj)
			{
			case CollisionLayer::LAYER_STATIC:
				return reBroad == CollisionLayerBroad::LAYER_DYNAMIC;
			case CollisionLayer::LAYER_DYNAMIC:
				return true;
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};
}
// __LAYERS //

// LISTENERS //
namespace Enoki::Physics
{
	class ContactListener : public JPH::ContactListener
	{
	public:
		virtual JPH::ValidateResult OnContactValidate(const JPH::Body& bodyA, const JPH::Body& bodyB, JPH::RVec3Arg baseOffset, const JPH::CollideShapeResult& collisionResult) override;
		virtual void OnContactAdded(const JPH::Body& bodyA, const JPH::Body& bodyB, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings) override;
		virtual void OnContactPersisted(const JPH::Body& bodyA, const JPH::Body& bodyB, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings) override;
		virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override;
	};

	class reBodyActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void OnBodyActivated(const JPH::BodyID& bodyId, uint64_t bodyUserData) override;
		virtual void OnBodyDeactivated(const JPH::BodyID& bodyId, uint64_t bodyUserData) override;
	};
}
// __LISTENERS //

// INTERFACE IMPL //
namespace Enoki::Physics
{
	class PhysicsJolt : public IPhysicsImpl
	{
	public:
		static constexpr uint32_t cMaxBodies = 65535;
		static constexpr uint32_t cNumBodyMutexes = 0; // zero is default
		static constexpr uint32_t cMaxBodyPairs = 65535;
		static constexpr uint32_t cMaxContactConstraints = 10240;

	public:
		PhysicsJolt() = default;
		void Init() override;
		void Dispose() override;
		void Update(const float dt) override;

		void DebugRender(bool bodies, bool constraints) override;

	public:
		int m_collisionSteps = 1;

	private:
		// jolt phys backend
		JPH::PhysicsSystem* physicsSystem;
		
		// temporary allocations during the physics update
		JPH::TempAllocatorImpl* m_tempAllocator;

		// TODO: !!!! JobSystemThreadPool is an example impl. Should use own job pool
		JPH::JobSystemThreadPool* m_jobSystem;

		// layer interfaces
		BPLayerInterfaceImpl m_broadPhaseLayerInterface;
		ObjectVsBroadPhaseLayerFilterImpl m_objectVsBroadphaseLayerFilter;
		ObjectLayerPairFilterImpl m_objectVsObjectLayerFilter;

		// listeners
		ContactListener m_contactListener;

		// debug render
		JoltDebugRenderer* m_debugRenderer = nullptr;
	};
};
// __INTERFACE IMPL //