#include "reJoltDebugRenderer.hpp"
#include "reEngine.hpp"
#include "reDebugRender.hpp"

using namespace rePhysics;

#if defined(JPH_DEBUG_RENDERER)

inline vec3_t& jolt2vec(const JPH::RVec3Arg& in) { return *(vec3_t*)&in; }
inline reColour jolt2col(const JPH::ColorArg& col)
{
	return reColour(col.mU32);
}

void reJoltDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
{
	reEngine::GetDebugRender()->DrawLine(jolt2vec(inFrom), jolt2vec(inTo), jolt2col(inColor));
}

void reJoltDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
	reEngine::GetDebugRender()->DrawTriangle(jolt2vec(inV1), jolt2vec(inV2), jolt2vec(inV3), jolt2col(inColor));
}

void reJoltDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
{
	// null
}

void reJoltDebugRenderer::DrawBodies(JPH::PhysicsSystem* physicsSystem)
{
	physicsSystem->DrawBodies(m_drawSettingsBodies, this);
}

void reJoltDebugRenderer::DrawConstraints(JPH::PhysicsSystem* physicsSystem)
{
	physicsSystem->DrawBodies(m_drawSettingsConstraints, this);
}


#else // JPH_DEBUG_RENDERER

void reJoltDebugRenderer::DrawBodies()
{
}

void reJoltDebugRenderer::DrawConstraints()
{
}

#endif // JPH_DEBUG_RENDERER

