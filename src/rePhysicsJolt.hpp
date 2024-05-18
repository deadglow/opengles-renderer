#pragma once

#include "rePhysics.hpp"

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

#include "reJoltDebugRenderer.hpp"

// LAYERS //
namespace rePhysics
{
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override
		{
			switch (reCollisionLayer::Enum(a))
			{
			case reCollisionLayer::LAYER_STATIC:
				return reCollisionLayer::Enum(b) == reCollisionLayer::LAYER_DYNAMIC;
			case reCollisionLayer::LAYER_DYNAMIC:
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
			m_objectToBroadPhase[reCollisionLayer::LAYER_STATIC] = JPH::BroadPhaseLayer(reCollisionLayerBroad::LAYER_STATIC);
			m_objectToBroadPhase[reCollisionLayer::LAYER_DYNAMIC] = JPH::BroadPhaseLayer(reCollisionLayerBroad::LAYER_DYNAMIC);
		}

		virtual uint32_t GetNumBroadPhaseLayers() const override
		{
			return reCollisionLayerBroad::LAYER_COUNT;
		}

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override
		{
			JPH_ASSERT(layer < reCollisionLayer::LAYER_COUNT);
			return m_objectToBroadPhase[layer];
		}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override
		{
			switch ((JPH::BroadPhaseLayer::Type)layer)
			{
			case reCollisionLayerBroad::LAYER_STATIC: return "LAYER_STATIC";
			case reCollisionLayerBroad::LAYER_DYNAMIC: return "LAYER_DYNAMIC";

			default:
				JPH_ASSERT(false);
				return "LAYER_INVALID";
			}
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer m_objectToBroadPhase[reCollisionLayer::LAYER_COUNT];
	};

	//  ObjectVsBroadPhaseLayerFilter //
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer broad) const override
		{
			const auto reObj = (reCollisionLayer::Enum)obj;
			const auto reBroad = (reCollisionLayerBroad::Enum)(JPH::BroadPhaseLayer::Type)broad;
			switch (reObj)
			{
			case reCollisionLayer::LAYER_STATIC:
				return reBroad == reCollisionLayerBroad::LAYER_DYNAMIC;
			case reCollisionLayer::LAYER_DYNAMIC:
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
namespace rePhysics
{
	class reContactListener : public JPH::ContactListener
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
namespace rePhysics
{
	class rePhysicsJolt : public reIPhysicsImpl
	{
	public:
		static constexpr uint32_t cMaxBodies = 65535;
		static constexpr uint32_t cNumBodyMutexes = 0; // zero is default
		static constexpr uint32_t cMaxBodyPairs = 65535;
		static constexpr uint32_t cMaxContactConstraints = 10240;

	public:
		rePhysicsJolt() = default;
		void Init() override;
		void Dispose() override;
		void Update(const float dt) override;

		void DebugRender(bool bodies, bool constraints) override;

	public:
		int m_collisionSteps = 1;

	private:
		// jolt phys backend
		JPH::PhysicsSystem* m_physicsSystem;
		
		// temporary allocations during the physics update
		JPH::TempAllocatorImpl* m_tempAllocator;

		// TODO: !!!! JobSystemThreadPool is an example impl. Should use own job pool
		JPH::JobSystemThreadPool* m_jobSystem;

		// layer interfaces
		BPLayerInterfaceImpl m_broadPhaseLayerInterface;
		ObjectVsBroadPhaseLayerFilterImpl m_objectVsBroadphaseLayerFilter;
		ObjectLayerPairFilterImpl m_objectVsObjectLayerFilter;

		// listeners
		reContactListener m_contactListener;

		// debug render
		reJoltDebugRenderer* m_debugRenderer = nullptr;
	};
};
// __INTERFACE IMPL //