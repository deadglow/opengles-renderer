#pragma once

#include "math_rotor.h"
#include "math_3d.h"

#include <cmath>

template <typename T>
T clamp(T v, T min, T max)
{
	return std::max(min, std::min(v, max));
}