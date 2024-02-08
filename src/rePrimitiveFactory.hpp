#pragma once

namespace reGraphics
{
	class reModel;

	namespace rePrimitives
	{
		enum PrimitiveType
		{
			INVALID,
			Quad,
			Cube,
			Sphere,
			Capsule
		};

		void ConstructPrimitive(const PrimitiveType primitive, reModel& model);

		void ConstructQuad(reModel& model);

		//TODO
		void ConstructCube(reModel& model);
		void ConstructSphere(reModel& model);
		void ConstructCapsule(reModel& model);
	};
};