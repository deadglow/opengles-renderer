#include "DebugRender.hpp"
#include "Engine.hpp"
#include "Time.hpp"

#include "SDL/SDL.h"

#include "gles2.h"

using namespace Enoki::Graphics;

DebugRender::DebugRender()
{
	m_lines.reserve(MAX_LINES);

	glGenVertexArrays(1, &m_debugVAO);
	glGenBuffers(1, &m_debugVBO);

	glBindVertexArray(m_debugVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);

	constexpr size_t bufferSize = DebugRender::MAX_LINES * sizeof(LineVertex) * 2;
	char empty[bufferSize];
	memset(empty, 0, bufferSize);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, empty, GL_STATIC_DRAW);

	size_t offset = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offset);
	glEnableVertexAttribArray(0); // pos

	offset += sizeof(vec3_t);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offset);
	glEnableVertexAttribArray(1); // colour

	offset += sizeof(vec4_t);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offset);
	glEnableVertexAttribArray(2); // blend

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

DebugRender::~DebugRender()
{
	SDL_assert(m_debugVAO != 0);

	glBindVertexArray(m_debugVAO);
	glDeleteBuffers(1, &m_debugVBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &m_debugVAO);
	m_debugVAO = 0;
	m_debugVBO = 0;
}

bool DebugRender::DrawLine(const vec3_t& a, const vec3_t& b, const Colour& col, const SecondsF64 time)
{
	if (!WillLinesFit(1))
		return false;

	AddLine(a, b, col, time);

	return true;
}

bool DebugRender::DrawRay(const vec3_t& origin, const vec3_t& ray, const Colour& col, const SecondsF64 time)
{
	return DrawLine(origin, v3_add(origin, ray), col, time);
}

bool DebugRender::DrawTriangle(const vec3_t& a, const vec3_t& b, const vec3_t& c, const Colour& col, const SecondsF64 time)
{
	if (!WillLinesFit(3))
		return false;

	AddLine(a, b, col, time);
	AddLine(b, c, col, time);
	AddLine(c, a, col, time);

	return true;
}

bool DebugRender::DrawSphere(const vec3_t& centre, const float radius, const Colour& col, const SecondsF64 time)
{
	// TODO
	// if (!WillLinesFit(howevermanylinesman))
	// 	return false;

	// const int segments = 8;
	// const int rows = 4;
	// const float segmentAngle = 2 * M_PI / segments;

	// for (int i = 0; i < segments; ++i)
	// {
	// 	for (int j = 0; j < rows; ++j)
	// 	{
	// 		float angle = segmentAngle * i;
	// 		float rowAngle = (M_PI / rows) * j;

	// 		vec3_t v = { sinf(angle), cosf(angle), sinf(rowAngle) };
	// 	}
	// }

	return true;
}

bool DebugRender::DrawCube(const vec3_t& pos, const rotor3_t& rot, const vec3_t& extents, const Colour& col, const SecondsF64 time)
{
	if (!WillLinesFit(12))
		return false;

	vec3_t points[8];
	int count = 0;
	for (int x = -1; x < 2; x += 2)
	{
		for (int y = -1; y < 2; y += 2)
		{
			for (int z = -1; z < 2; z += 2)
			{
				vec3_t v = v3_mul(vec3(x, y, z), extents);
				points[count] = rot3_transform(rot, v);
				count++;
			}
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		for (int j = i; j < 8; ++j)
		{
			AddLine(points[i], points[j], col, time);
		}
	}

	return true;
}

void DebugRender::Render()
{
	std::vector<LineVertex> lines;
	lines.reserve(m_lines.size() * 2);

	for (auto& line : m_lines)
	{
		lines.push_back({
			line.a,
			line.col.GetVec4(),
			0.f
		});
		lines.push_back({
			line.b,
			line.col.GetVec4(),
			1.f
		});
	}

	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0f);

	int count = lines.size();
	glBindVertexArray(m_debugVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(LineVertex), &lines[0]);

	glDrawArrays(GL_LINES, 0, count);

	glBindVertexArray(0);

	ValidateLines();
}

void DebugRender::ValidateLines()
{
	SecondsF64 now = Engine::GetTime()->now;

	for (int i = m_lines.size() - 1; i > -1; i--)
	{
		DebugLine& line = m_lines[i];
		if (line.time < now)
			m_lines.erase(m_lines.begin() + i);
	}
}

bool DebugRender::WillLinesFit(const int count) const
{
	return (MAX_LINES - m_lines.size()) >= count;
}

bool DebugRender::AddLine(const vec3_t& a, const vec3_t& b, const Colour& col, const SecondsF64 time)
{
	SDL_assert(WillLinesFit(1));

	SecondsF64 expiry = Engine::GetTime()->now + time;

	if (time < SecondsF64(0.00001))
		expiry = SecondsF64(-0.0);

	m_lines.push_back({
		.a = a,
		.b = b,
		.col = col,
		.time = expiry
	});

	return true;
}
