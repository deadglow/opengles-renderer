#pragma once

namespace Enoki::Graphics
{
	class Model;

	namespace Primitives
	{
		enum PrimitiveType
		{
			INVALID,
			Quad,
			Cube,
			Sphere,
			Capsule
		};

		void ConstructPrimitive(const PrimitiveType primitive, Model& model);

		void ConstructQuad(Model& model);

		//TODO
		void ConstructCube(Model& model);
		void ConstructSphere(Model& model);
		void ConstructCapsule(Model& model);
	};
};