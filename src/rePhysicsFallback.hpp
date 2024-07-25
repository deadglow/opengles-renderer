#pragma once

#include "rePhysics.hpp"

namespace rePhysics
{
	class rePhysicsFallback : public reIPhysicsImpl
	{
	public:
		rePhysicsFallback() = default;
		void Init() override;
		void Dispose() override;
		void Update(const float dt) override;

		void DebugRender(bool bodies, bool constraints) override;
	};
};