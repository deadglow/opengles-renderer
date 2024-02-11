#include "rePrimitiveFactory.hpp"
#include "math_3d.h"
#include "reModel.hpp"
#include "SDL/SDL.h"

using namespace reGraphics;

void rePrimitives::ConstructPrimitive(const PrimitiveType primitive, reModel& model)
{
	SDL_assert(primitive != PrimitiveType::INVALID);

	switch (primitive)
	{
	case PrimitiveType::Quad:
		ConstructQuad(model);
		break;
	case PrimitiveType::Cube:
		ConstructCube(model);
		break;
	case PrimitiveType::Sphere:
		ConstructSphere(model);
		break;
	case PrimitiveType::Capsule:
		ConstructCapsule(model);
		break;

	default:
		break;
	}
}

void rePrimitives::ConstructQuad(reModel& model)
{
	model.m_meshes.reserve(1);
	reMesh& mesh = model.m_meshes.emplace_back();

	vec3_t normal{ 0.f, 0.f, 1.f };
	vec3_t tangent{ -1.f, 0.f, 0.f };

	// (facing away) TR, TL, BL, BR
	mesh.m_vertices = {
		{ vec3_t { 0.5f, 0.5f, 0.0f },   normal, tangent, vec2_t{ 1.f, 1.f } },
		{ vec3_t { -0.5f, 0.5f, 0.0f },  normal, tangent, vec2_t{ 0.f, 1.f } },
		{ vec3_t { -0.5f, -0.5f, 0.0f }, normal, tangent, vec2_t{ 0.f, 0.f } },
		{ vec3_t { 0.5f, -0.5f, 0.0f },  normal, tangent, vec2_t{ 0.f, 1.f } },
	};

	mesh.m_triangles = {
		{ 0, 1, 3 },
		{ 2, 3, 1 },
	};
}

void rePrimitives::ConstructCube(reModel& model)
{
}

void rePrimitives::ConstructSphere(reModel& model)
{
}

void rePrimitives::ConstructCapsule(reModel& model)
{
}
