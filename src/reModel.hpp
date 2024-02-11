#pragma once

#include "math_3d.h"
#include "reGuid.hpp"
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
		vec3_t tangent;
		vec2_t texCoord;
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
		reModel(const reGuid<reModel> guid, const char* name)
			: m_guid(guid)
			, m_name(name)
		{}

		reGuid<reModel> m_guid;
		std::string m_name;

		std::vector<reMesh> m_meshes;
		std::vector<reGuid<reMaterial>> m_materials;

		bool m_loadedOnGpu = false;
	};

	class reModelInst
	{
	public:
		reModelInst(const reGuid<reModel> model)
			: m_baseModel(model)
		{}

		reGuid<reModel> m_baseModel;
		std::vector<reGuid<reMaterial>> m_materialOverrides;
	};
}