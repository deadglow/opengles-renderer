#pragma once
#include "CommonTypes.hpp"
#include "Math.h"

namespace Enoki
{
	struct Colour
	{
		enum Channel : uint8
		{
			C_RED,
			C_GREEN,
			C_BLUE,
			C_ALPHA
		};

		Colour() = default;

		Colour(const uint8 r_arg, const uint8 g_arg, const uint8 b_arg, const uint8 a_arg)
		{
			r = r_arg;
			g = g_arg;
			b = b_arg;
			a = a_arg;
		};

		Colour(const vec4_t& floatColour)
			: Colour(
				(uint8)(floatColour.x * 255),
				(uint8)(floatColour.y * 255),
				(uint8)(floatColour.z * 255),
				(uint8)(floatColour.w * 255)
			)
		{};

		Colour(const uint32 colour) { *(uint32*)channels = colour; };

		uint32 GetUint32() const { return *(uint32*)channels; }

		vec4_t GetVec4() const {
			return vec4(
				(float)r  / 255.f,
				(float)g  / 255.f,
				(float)b  / 255.f,
				(float)a  / 255.f
			);
		}

		float GetAsFloat(const Channel channel) { return (float)channels[channel] / 255.f; }

		union {
			struct
			{
				uint8 r;
				uint8 g;
				uint8 b;
				uint8 a;
			};
			uint8 channels[4];
		};
	};
}