#pragma once
#include "math_3d.h"
#include <vector>
#include <string>

using GLuint = unsigned int;
using GLint = int;

namespace reGraphics
{
	class reMaterial;

	struct reVert
	{
		vec3_t position;
		vec3_t normal;
	};

	struct reTri
	{
		GLuint indices[3];
	};

	class reMesh
	{
	public:
		std::vector<reVert> m_vertices;
		std::vector<reTri> m_triangles;
		GLuint m_VAO, m_VBO, m_EBO;
	};

	class reModel
	{
	public:

		reModel() = default;
		reModel(const std::string& name);
		std::string m_name;
		std::vector<reMesh> m_meshes;
		std::vector<reMaterial*> m_materials;

		float m_unitScale = 1.0f;
		bool m_loadedOnGpu = false;
	};
}