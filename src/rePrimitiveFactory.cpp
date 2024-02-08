#include "rePrimitiveFactory.hpp"
#include "math_3d.h"
#include "reModel.hpp"
#include "SDL/SDL.h"

using namespace reGraphics;

void reGraphics::rePrimitives::ConstructPrimitive(const PrimitiveType primitive, reModel& model)
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
	model.m_meshes.push_back(reMesh());

	reMesh& mesh = model.m_meshes[0];

	vec3_t normal{ 0.f, 0.f, 1.f };
	// (facing away) TR, TL, BL, BR
	mesh.m_vertices = {
		{ vec3_t { 0.5f, 0.5f, 0.f }, normal },
		{ vec3_t { -0.5f, 0.5f, 0.f }, normal },
		{ vec3_t { -0.5f, -0.5f, 0.f }, normal },
		{ vec3_t { -0.5f, -0.5f, 0.f }, normal },
	};

	mesh.m_triangles = {
		{ 0, 1, 3 },
		{ 2, 3, 1 },
	};

	model.m_unitScale = 1.f;
}

void rePrimitives::ConstructCube(reModel& model)
{
	model.m_meshes.push_back(reMesh());

	reMesh& mesh = model.m_meshes[0];

	vec3_t normal[6] {
		{ 0.f, 0.f, 1.f }, // +Z
		{ 0.f, 0.f, 1.f }, // -Z
		{ 0.f, 0.f, 1.f }, // +X
		{ 0.f, 0.f, 1.f }, // -X
		{ 0.f, 0.f, 1.f }, // +Y
		{ 0.f, 0.f, 1.f }, // -Y
	};
	
	// (facing away) TR, TL, BL, BR
	mesh.m_vertices = {
		{ vec3_t { 0.5f, 0.5f, 0.f }, normal[0] },
		{ vec3_t { -0.5f, 0.5f, 0.f }, normal[0] },
		{ vec3_t { -0.5f, -0.5f, 0.f }, normal[0] },
		{ vec3_t { -0.5f, -0.5f, 0.f }, normal[0] },
	};

	mesh.m_triangles = {
		{ 0, 1, 3 },
		{ 2, 3, 1 },
	};

	model.m_unitScale = 1.f;
}

void rePrimitives::ConstructSphere(reModel& model)
{
}

void rePrimitives::ConstructCapsule(reModel& model)
{
}
