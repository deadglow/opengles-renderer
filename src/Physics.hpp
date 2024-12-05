#pragma once

#include "CommonTypes.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - -
//   ENUMS
// - - - - - - - - - - - - - - - - - - - - - - - -
namespace Enoki::Physics
{
	enum class CollisionLayer : uint16
	{
		LAYER_STATIC,
		LAYER_DYNAMIC,
		LAYER_COUNT,
	};

	enum class CollisionLayerBroad : uint16
	{
		LAYER_STATIC,
		LAYER_DYNAMIC,
		LAYER_COUNT,
	};

	enum class ShapeType : uint16
	{
		Sphere,
		Box,
		Plane,
	};
};

// - - - - - - - - - - - - - - - - - - - - - - - - -
//   MAIN CLASS
// - - - - - - - - - - - - - - - - - - - - - - - - -
namespace Enoki::Physics
{
	class IPhysicsImpl
	{
	public:
		IPhysicsImpl() = default;
		virtual ~IPhysicsImpl() = default;
		
		virtual void Init() = 0;
		virtual void Dispose() = 0;
		virtual void Update(const float dt) = 0;

		virtual void DebugRender(bool bodies, bool constraints) = 0;
	};

	class PhysicsSystem
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
		IPhysicsImpl* m_impl = nullptr;
	};


};