#include "JoltDebugRenderer.hpp"
#include "Engine.hpp"
#include "DebugRender.hpp"

using namespace Enoki;
using namespace Enoki::Physics;

#if defined(JPH_DEBUG_RENDERER)

inline vec3_t& jolt2vec(const JPH::RVec3Arg& in) { return *(vec3_t*)&in; }
inline Colour jolt2col(const JPH::ColorArg& col)
{
	return Colour(col.mU32);
}

void JoltDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
{
	Engine::GetDebugRender()->DrawLine(jolt2vec(inFrom), jolt2vec(inTo), jolt2col(inColor));
}

void JoltDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
	Engine::GetDebugRender()->DrawTriangle(jolt2vec(inV1), jolt2vec(inV2), jolt2vec(inV3), jolt2col(inColor));
}

void JoltDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
{
	// null
}

void JoltDebugRenderer::DrawBodies(JPH::PhysicsSystem* physicsSystem)
{
	physicsSystem->DrawBodies(drawSettingsBodies, this);
}

void JoltDebugRenderer::DrawConstraints(JPH::PhysicsSystem* physicsSystem)
{
	physicsSystem->DrawBodies(drawSettingsConstraints, this);
}


#else // JPH_DEBUG_RENDERER

void JoltDebugRenderer::DrawBodies()
{
}

void JoltDebugRenderer::DrawConstraints()
{
}

#endif // JPH_DEBUG_RENDERER

