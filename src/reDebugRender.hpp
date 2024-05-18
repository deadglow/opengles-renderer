#pragma once

#include <vector>
#include "reMath.h"
#include "reColour.hpp"

using GLuint = unsigned int;
using GLushort = unsigned short;
using GLint = int;

struct reDebugLine
{
	vec3_t a;
	vec3_t b;
	reColour col;
	double time;
};

class reDebugRender
{
public:
	static constexpr int MAX_LINES = 8192 * 2;

	reDebugRender();
	~reDebugRender();

	bool DrawLine(const vec3_t& a, const vec3_t& b, const reColour& col, const float time = 0);
	bool DrawRay(const vec3_t& origin, const vec3_t& ray, const reColour& col, const float time = 0);
	bool DrawTriangle(const vec3_t& a, const vec3_t& b, const vec3_t& c, const reColour& col, const float time = 0);
	bool DrawSphere(const vec3_t& centre, const float radius, const reColour& col, const float time = 0);
	bool DrawCube(const vec3_t& pos, const rotor3_t& rot, const vec3_t& extents, const reColour& col, const float time = 0);

	void Render();

private:
	struct LineVertex
	{
		vec3_t pos;
		vec4_t colour;
		float blend;
	};

	bool WillLinesFit(const int count) const;
	bool AddLine(const vec3_t& a, const vec3_t& b, const reColour& col, const float time);
	void ValidateLines();

	std::vector <reDebugLine> m_lines;
	
	GLuint m_debugVAO;
	GLuint m_debugVBO;
};