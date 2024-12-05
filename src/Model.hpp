#pragma once

#include "Math.h"
#include "Guid.hpp"
#include <vector>
#include <string>

namespace Enoki::Graphics
{
	class Material;

	enum VertexAttribs
	{
		POSITION,
		NORMAL,
		TANGENT,
		TEXCOORD0,
		COUNT
	};

	struct VertexBuffers
	{
		static constexpr size MAX_TEXCOORDS = 3;

		std::vector<vec3_t> position;
		std::vector<vec3_t> normal;
		std::vector<vec4_t> tangent;
		std::vector<vec2_t> texCoord[MAX_TEXCOORDS];
	};

	class Mesh
	{
	public:
		static constexpr int REQUIRED_BUFFERS = 3;
		
		VertexBuffers vertexBuffers;
		std::vector<uint32> vertexIndices;
		uint32 VAO, EBO;
		uint32 VBOs[VertexAttribs::COUNT];
	};

	class Model
	{
	public:
		Model() = default;
		Model(const Guid<Model> guid, std::string_view name)
			: guid(guid)
			, name(name)
		{}

		Guid<Model> guid;
		std::string name;

		std::vector<Mesh> meshes;
		std::vector<Guid<Material>> materials;

		bool loadedOnGpu = false;
	};

	class ModelInst
	{
	public:
		ModelInst(const Guid<Model> model)
			: baseModel(model)
		{}

		Guid<Model> baseModel;
		std::vector<Guid<Material>> materialOverrides;
	};
}

// namespace reGraphics
// {
// 	class Material;
	
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

// 	struct Mesh
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
// 		Uint32 VAO, EBO;
// 	};

// 	struct Model
// 	{
// 	public:
// 		Model() = default;
// 		Model(const reGuid<Model> guid, const char* name)
// 			: guid(guid)
// 			, name(name)
// 		{}

// 		reGuid<Model> guid;
// 		std::string name;

// 		std::vector<Mesh> meshes;
// 		std::vector<reBuffer> m_buffers;

// 		std::vector<reGuid<Material>> materials;

// 		bool loadedOnGpu = false;
// 		Uint32 VBOs[VertexAttribs::COUNT];
// 	};
// };