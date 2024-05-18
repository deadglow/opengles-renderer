#pragma once

#include <stdint.h>

// - - - - - - - - - - - - - - - - - - - - - - - -
//   ENUMS
// - - - - - - - - - - - - - - - - - - - - - - - -
namespace rePhysics
{
	namespace reCollisionLayer
	{
		enum Enum : uint16_t
		{
			LAYER_STATIC,
			LAYER_DYNAMIC,
			LAYER_COUNT,
		};
	};

	namespace reCollisionLayerBroad
	{
		enum Enum : uint16_t
		{
			LAYER_STATIC,
			LAYER_DYNAMIC,
			LAYER_COUNT,
		};
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