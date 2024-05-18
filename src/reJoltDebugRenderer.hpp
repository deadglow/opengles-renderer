#pragma once

#if not defined(JPH_DEBUG_RENDERER)
namespace rePhysics
{
	class reJoltDebugRenderer
	{
		void DrawBodies(JPH::PhysicsSystem* physicsSystem);
		void DrawConstraints(JPH::PhysicsSystem* physicsSystem);
	};
}

#else // not defined(JPH_DEBUG_RENDERER)

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

namespace rePhysics
{
	class reJoltDebugRenderer final : public JPH::DebugRendererSimple
	{
	public:
		reJoltDebugRenderer() : JPH::DebugRendererSimple() { };
		virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
		virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) override;
		virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight) override;

		void DrawBodies(JPH::PhysicsSystem* physicsSystem);
		void DrawConstraints(JPH::PhysicsSystem* physicsSystem);

		JPH::BodyManager::DrawSettings m_drawSettingsBodies {
			.mDrawShapeWireframe = true,
			.mDrawCenterOfMassTransform = true,
		};
		JPH::BodyManager::DrawSettings m_drawSettingsConstraints;
	};
}

#endif // not defined(JPH_DEBUG_RENDERER)