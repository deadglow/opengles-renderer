#pragma once

#include "CommonTypes.hpp"
#include <vector>
#include "Math.h"
#include "Colour.hpp"

namespace Enoki::Graphics
{
	struct DebugLine
	{
		vec3_t a;
		vec3_t b;
		Colour col;
		SecondsF64 time;
	};

	class DebugRender
	{
	public:
		static constexpr int MAX_LINES = 8192 * 2;

		struct LineVertex
		{
			vec3_t pos;
			vec4_t colour;
			float blend;
		};

		DebugRender();
		~DebugRender();

		bool DrawLine(const vec3_t& a, const vec3_t& b, const Colour& col, const SecondsF64 time = SecondsF64::zero());
		bool DrawRay(const vec3_t& origin, const vec3_t& ray, const Colour& col, const SecondsF64 time = SecondsF64::zero());
		bool DrawTriangle(const vec3_t& a, const vec3_t& b, const vec3_t& c, const Colour& col, const SecondsF64 time = SecondsF64::zero());
		bool DrawSphere(const vec3_t& centre, const float radius, const Colour& col, const SecondsF64 time = SecondsF64::zero());
		bool DrawCube(const vec3_t& pos, const rotor3_t& rot, const vec3_t& extents, const Colour& col, const SecondsF64 time = SecondsF64::zero());

		void Render();

	private:

		bool WillLinesFit(const int count) const;
		bool AddLine(const vec3_t& a, const vec3_t& b, const Colour& col, const SecondsF64 time);
		void ValidateLines();

		std::vector <DebugLine> m_lines;
		
		uint32 m_debugVAO;
		uint32 m_debugVBO;
	};	
}
