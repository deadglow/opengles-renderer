#pragma once

#include <stdint.h>

// - - - - - - - - - - - - - - - - - - - - - - - -
//   ENUMS
// - - - - - - - - - - - - - - - - - - - - - - - -
namespace rePhysics
{

	enum class CollisionLayer : uint16_t
	{
		LAYER_STATIC,
		LAYER_DYNAMIC,
		LAYER_COUNT,
	};

	enum class CollisionLayerBroad : uint16_t
	{
		LAYER_STATIC,
		LAYER_DYNAMIC,
		LAYER_COUNT,
	};

	enum class ShapeType : uint16_t
	{
		Sphere,
		Box,
		Plane,
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - -
//   MAIN CLASS
// - - - - - - - - - - - - - - - - - - - - - - - - -
namespace rePhysics
{
	class reIPhysicsImpl
	{
	public:
		reIPhysicsImpl() = default;
		virtual ~reIPhysicsImpl() = default;
		
		virtual void Init() = 0;
		virtual void Dispose() = 0;
		virtual void Update(const float dt) = 0;

		virtual void DebugRender(bool bodies, bool constraints) = 0;
	};

	class rePhysicsSystem
	{
	public:
		void Init();
		void Dispose();
		void Update(const float dt);
		void Update();

		void DebugRender(bool bodies, bool constraints);

	public:
		double m_timestep = 1.0 / 50.0;

	private:
		reIPhysicsImpl* m_impl = nullptr;
	};


};