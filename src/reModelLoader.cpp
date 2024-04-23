#include "reModelLoader.hpp"

#include "reModel.hpp"
#include "SDL/SDL.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

using namespace reGraphics;
using namespace tinygltf;

enum GLTFAtr
{
	GLTFAtr_POSITION,
	GLTFAtr_NORMAL,
	GLTFAtr_TANGENT,
	GLTFAtr_TEXCOORD_0,
	GLTFAtr_COUNT
};

const char* GLTFAtrStr[GLTFAtr_COUNT]
{
	"POSITION",
	"NORMAL",
	"TANGENT",
	"TEXCOORD_0",
};

template<typename Component>
void processArray(std::vector<Component>& vector, Accessor& accessor, std::vector<BufferView>& bufferViews, std::vector<Buffer>& buffers)
{
	auto& bufferView = bufferViews[accessor.bufferView];
	auto& buffer = buffers[bufferView.buffer];

	size_t stride = bufferView.byteStride;
	if (stride == 0)
		stride = sizeof(Component);
	
	vector.reserve(accessor.count);
	Component* cArray = (Component*)&vector[0];

	for (int i = 0; i < accessor.count; ++i)
	{
		cArray[i] = *(Component*)&buffer.data[i * stride];
	}
}

bool processGltfPrimitive(const Primitive& gltfPrimitive, Model& gltfModel, reModel& model)
{
	model.m_meshes.push_back(reMesh());
	reMesh& mesh = model.m_meshes.back();

	// get ids for vertex accesors
	size_t vertAccesors[GLTFAtr_COUNT];
	for (int i = 0; i < GLTFAtr_COUNT; ++i)
	{
		auto& attributes = gltfPrimitive.attributes;
		auto attribute = attributes.find(GLTFAtrStr[i]);

		vertAccesors[i] = (attribute != attributes.end()) ? (*attribute).second : -1;
	}

	SDL_assert(gltfPrimitive.indices > -1);
	SDL_assert(vertAccesors[GLTFAtr_POSITION] > -1);
	SDL_assert(vertAccesors[GLTFAtr_NORMAL] > -1);
	SDL_assert(vertAccesors[GLTFAtr_TANGENT] > -1);
	
	auto& bufferViews = gltfModel.bufferViews;
	auto& buffers = gltfModel.buffers;

	// indices
	{
		Accessor& accessor = gltfModel.accessors[gltfPrimitive.indices];
		processArray<GLuint>(mesh.m_vertexIndices, accessor, bufferViews, buffers);
	}

	// position
	{
		Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_POSITION]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC3);
		processArray<vec3_t>(mesh.m_vertexBuffers.position, accessor, bufferViews, buffers);
	}

	// normal
	{
		Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_NORMAL]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC3);
		processArray<vec3_t>(mesh.m_vertexBuffers.normal, accessor, bufferViews, buffers);
	}

	// tangent
	{
		Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_TANGENT]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC3);
		processArray<vec3_t>(mesh.m_vertexBuffers.tangent, accessor, bufferViews, buffers);
	}

	// texcoords
	// note: refactor to handle several texcoords
	if (vertAccesors[GLTFAtr_TEXCOORD_0] > -1)
	{
		Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_TEXCOORD_0]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC2);
		processArray<vec2_t>(mesh.m_vertexBuffers.texCoord[0], accessor, bufferViews, buffers);
	}

	return true;
}

bool reModelLoader::LoadFromFile(const char* filename, reModel& model)
{
	SDL_assert(filename);

	std::string relativePath("models\\");
	relativePath.append(filename);
	relativePath.append(".gltf");

	std::filesystem::path path = std::filesystem::current_path();
	path.append(relativePath);
	SDL_assert(std::filesystem::is_regular_file(path));

	TinyGLTF loader;
	Model gltfModel;
	std::string err, warn;
	loader.LoadBinaryFromFile(&gltfModel, &err, &warn, path.string());

	model.m_name = filename;

	// get data from loaded model into reModel
	for (const auto& gltfMesh : gltfModel.meshes)
	{
		for (const auto& gltfPrimitive : gltfMesh.primitives)
		{
			if (!processGltfPrimitive(gltfPrimitive, gltfModel, model))
				return false;
		}
	}

	return true;
}