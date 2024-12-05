#include "ModelLoader.hpp"

#include "Model.hpp"
#include "SDL/SDL.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

using namespace Enoki;
using namespace Enoki::Graphics;

const char* PATH_MODEL = "assets\\models\\";

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

template<typename Component, typename BufferComponent = Component>
void processArray(std::vector<Component>& vector, tinygltf::Accessor& accessor, std::vector<tinygltf::BufferView>& bufferViews, std::vector<tinygltf::Buffer>& buffers)
{
	auto& bufferView = bufferViews[accessor.bufferView];
	auto& buffer = buffers[bufferView.buffer];

	size_t offset = bufferView.byteOffset + accessor.byteOffset;
	size_t stride = bufferView.byteStride;
	if (stride == 0)
		stride = sizeof(BufferComponent);
	
	vector.reserve(accessor.count);

	for (int i = 0; i < accessor.count; ++i)
	{
		vector.push_back((Component)*(BufferComponent*)&buffer.data[i * stride + offset]);
	}
}

bool processGltfPrimitive(const tinygltf::Primitive& gltfPrimitive, tinygltf::Model& gltfModel, Model& model)
{
	model.meshes.push_back(Mesh());
	Mesh& mesh = model.meshes.back();

	// get ids for vertex accesors
	int vertAccesors[GLTFAtr_COUNT];
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
		tinygltf::Accessor& accessor = gltfModel.accessors[gltfPrimitive.indices];
		if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			processArray<uint32, uint16>(mesh.vertexIndices, accessor, bufferViews, buffers);
		}
		else
		{
			processArray<uint32>(mesh.vertexIndices, accessor, bufferViews, buffers);
		}
	}

	// position
	{
		tinygltf::Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_POSITION]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC3);
		processArray<vec3_t>(mesh.vertexBuffers.position, accessor, bufferViews, buffers);
	}

	// normal
	{
		tinygltf::Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_NORMAL]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC3);
		processArray<vec3_t>(mesh.vertexBuffers.normal, accessor, bufferViews, buffers);
	}

	// tangent
	{
		tinygltf::Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_TANGENT]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC4);
		processArray<vec4_t>(mesh.vertexBuffers.tangent, accessor, bufferViews, buffers);
	}

	// texcoords
	// note: refactor to handle several texcoords
	if (vertAccesors[GLTFAtr_TEXCOORD_0] > -1)
	{
		tinygltf::Accessor& accessor = gltfModel.accessors[vertAccesors[GLTFAtr_TEXCOORD_0]];
		SDL_assert(accessor.type == TINYGLTF_TYPE_VEC2);
		processArray<vec2_t>(mesh.vertexBuffers.texCoord[0], accessor, bufferViews, buffers);
	}

	return true;
}

bool Enoki::Graphics::ModelLoader::LoadFromFile(std::string_view filename, Model& model)
{
	SDL_assert(!filename.empty());

	std::string relativePath(PATH_MODEL);
	relativePath.append(filename);
	relativePath.append(".gltf");

	std::filesystem::path path = std::filesystem::current_path();
	path.append(relativePath);
	SDL_assert(std::filesystem::is_regular_file(path));

	tinygltf::TinyGLTF loader;
	tinygltf::Model gltfModel;
	std::string err, warn;
	bool success = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, path.string());
	if (warn.size() > 0)
	{
		printf("\nLoad warning on %s : %s", path.string().c_str(), warn.c_str());
	}

	if (!success)
	{
		printf("\nLoad error on %s : %s", path.string().c_str(), err.c_str());
		printf(err.c_str());
		SDL_assert(success);
	}

	model.name = filename;

	// get data from loaded model into Model
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