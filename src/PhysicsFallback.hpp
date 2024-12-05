#pragma once

#include "Physics.hpp"

namespace Enoki::Physics
{
	class PhysicsFallback : public IPhysicsImpl
	{
	public:
		PhysicsFallback() = default;
		void Init() override;
		void Dispose() override;
		void Update(const float dt) override;

		void DebugRender(bool bodies, bool constraints) override;
	};
};