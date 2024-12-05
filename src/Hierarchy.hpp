#pragma once

#include <unordered_map>
#include "Guid.hpp"
#include "Transform.hpp"

namespace Enoki
{
	class Hierarchy
	{
	private:
		std::unordered_map<Guid<Transform>, Transform> m_transforms;
	};
}