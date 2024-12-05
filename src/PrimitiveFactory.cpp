#include "PrimitiveFactory.hpp"
#include "Math.h"
#include "Model.hpp"
#include "SDL/SDL.h"

using namespace Enoki::Graphics;

void Primitives::ConstructPrimitive(const PrimitiveType primitive, Model& model)
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

void Primitives::ConstructQuad(Model& model)
{
	model.meshes.reserve(1);
	Mesh& mesh = model.meshes.emplace_back();

	// (facing away) TR, TL, BL, BR
	mesh.vertexBuffers.position = {
		vec3_t { 0.5f, 0.5f, 0.0f },
		vec3_t { -0.5f, 0.5f, 0.0f },
		vec3_t { -0.5f, -0.5f, 0.0f },
		vec3_t { 0.5f, -0.5f, 0.0f },
	};

	vec3_t normal{ 0.f, 0.f, 1.f };
	mesh.vertexBuffers.normal = { normal, normal, normal, normal };

	vec4_t tangent{ -1.f, 0.f, 0.f, 1.0f };
	mesh.vertexBuffers.tangent = { tangent, tangent, tangent, tangent };

	mesh.vertexBuffers.texCoord[0] = {
		vec2_t{ 1.f, 1.f },
		vec2_t{ 0.f, 1.f },
		vec2_t{ 0.f, 0.f },
		vec2_t{ 0.f, 1.f },
	};

	mesh.vertexIndices = {
		0, 1, 3,
		2, 3, 1,
	};
}

void Primitives::ConstructCube(Model& model)
{
}

void Primitives::ConstructSphere(Model& model)
{
}

void Primitives::ConstructCapsule(Model& model)
{
}
