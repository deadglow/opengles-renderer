#pragma once

#include "reMath.h"
#include "reGuid.hpp"
#include <vector>
#include <string>

using GLuint = unsigned int;
using GLushort = unsigned short;
using GLint = int;

namespace reGraphics
{
	class reMaterial;

	enum reVertexAttribs
	{
		POSITION,
		NORMAL,
		TANGENT,
		TEXCOORD0,
		COUNT
	};

	struct reVertexBuffers
	{
		static constexpr size_t MAX_TEXCOORDS = 3;

		std::vector<vec3_t> position;
		std::vector<vec3_t> normal;
		std::vector<vec4_t> tangent;
		std::vector<vec2_t> texCoord[MAX_TEXCOORDS];
	};

	class reMesh
	{
	public:
		static constexpr int REQUIRED_BUFFERS = 3;
		
		reVertexBuffers m_vertexBuffers;
		std::vector<GLuint> m_vertexIndices;
		GLuint m_VAO, m_EBO;
		GLuint m_VBOs[reVertexAttribs::COUNT];
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

// namespace reGraphics
// {
// 	class reMaterial;
	
// 	enum reVertexAttribute
// 	{
// 		ATRIB_POSITION,
// 		ATRIB_NORMAL,
// 		ATRIB_TANGENT,
// 		ATRIB_TEXCOORD,
// 		ATRIB_COUNT
// 	};

// 	struct reBuffer
// 	{
// 		std::vector<unsigned char> m_bytes;
// 	};

// 	struct reBufferView
// 	{
// 		enum BufferTarget : uint16_t
// 		{
// 			INVALID,
// 			ARRAY_BUFFER,
// 			ELEMENT_ARRAY_BUFFER,
// 		};
// 		uint32_t m_bufferIndex;
// 		uint32_t m_byteLength;
// 		uint32_t m_byteOffset;
// 	};

// 	template <reVertexAttribute V, typename T>
// 	struct reAccessor
// 	{
// 		static constexpr size_t COMPONENT_SIZE = sizeof(T);
// 		static constexpr reVertexAttribute ATTRIBUTE = V;

// 		reBufferView m_view;
// 		uint32_t m_count;
// 		uint32_t m_count;
// 	};

// 	struct reMesh
// 	{
// 		struct reAccessors
// 		{
// 			static constexpr size_t MAX_TEXCOORDS = 3;

// 			reAccessor<ATRIB_POSITION, vec3_t> m_position;
// 			reAccessor<ATRIB_NORMAL, vec3_t> m_normal;
// 			reAccessor<ATRIB_TANGENT, vec3_t> m_tangent;
// 			reAccessor<ATRIB_TEXCOORD, vec2_t> m_texcoords[MAX_TEXCOORDS];
// 		};
		
// 		reAccessors m_accessors;
// 		GLuint m_VAO, m_EBO;
// 	};

// 	struct reModel
// 	{
// 	public:
// 		reModel() = default;
// 		reModel(const reGuid<reModel> guid, const char* name)
// 			: m_guid(guid)
// 			, m_name(name)
// 		{}

// 		reGuid<reModel> m_guid;
// 		std::string m_name;

// 		std::vector<reMesh> m_meshes;
// 		std::vector<reBuffer> m_buffers;

// 		std::vector<reGuid<reMaterial>> m_materials;

// 		bool m_loadedOnGpu = false;
// 		GLuint m_VBOs[reVertexAttribs::COUNT];
// 	};
// };