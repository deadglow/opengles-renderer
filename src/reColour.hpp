#pragma once
#include "reMath.h"
#include <stdint.h>

struct reColour
{
	enum Channel : unsigned char
	{
		C_RED,
		C_GREEN,
		C_BLUE,
		C_ALPHA
	};

	reColour() = default;

	reColour(const uint8_t r_arg, const uint8_t g_arg, const uint8_t b_arg, const uint8_t a_arg)
	{
		r = r_arg;
		g = g_arg;
		b = b_arg;
		a = a_arg;
	};

	reColour(const vec4_t& floatColour)
		: reColour(
			(uint8_t)(floatColour.x * 255),
			(uint8_t)(floatColour.y * 255),
			(uint8_t)(floatColour.z * 255),
			(uint8_t)(floatColour.w * 255)
		)
	{};

	reColour(const uint32_t colour) { *(uint32_t*)channels = colour; };

	uint32_t GetUint32() const { return *(uint32_t*)channels; }

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
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		uint8_t channels[4];
	};
};