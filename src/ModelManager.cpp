#include "ModelManager.hpp"
#include "SDL/SDL.h"
#include "gles2.h"

#include "ModelLoader.hpp"

using namespace Enoki;
using namespace Enoki::Graphics;

ModelManager::ModelManager()
{
	// default shapes
	auto& model = LoadNewPrimitive("default_quad", Primitives::Quad);
	GPULoad(model.guid);
}

ModelManager::~ModelManager()
{
	UnloadAllModels();
}

void ModelManager::UnloadAllModels()
{
	for (auto it = m_models.cbegin(); it != m_models.cend(); ++it)
	{
		const Model* model = &(it->second);
		if (model->loadedOnGpu)
		{
			GPUUnload(model->guid);
		}
	}

	m_models.clear();
	m_guidLookup.clear();
}

Model& ModelManager::CreateEmptyModel(std::string_view name)
{
	SDL_assert(!IsModelLoaded(name));

	auto guid = Guid<Model>::CreateGUID();
	m_models.emplace(guid, Model(guid, name));
	m_guidLookup.emplace(name, guid);

	return m_models[guid];
}

const Model& ModelManager::LoadModel(std::string_view filename)
{
	SDL_assert(!IsModelLoaded(filename));

	Model& model = CreateEmptyModel(filename);
	bool success = ModelLoader::LoadFromFile(filename, model);
	SDL_assert(success);

	return model;
}

void ModelManager::UnloadModel(const Guid<Model> guid)
{
	SDL_assert(IsModelLoaded(guid));

	const Model* model = &m_models[guid];
	if (model->loadedOnGpu)
	{
		GPUUnload(guid);
	}

	m_guidLookup.erase(model->name);
	m_models.erase(guid);
}

const Model& ModelManager::LoadNewPrimitive(std::string_view name, const Primitives::PrimitiveType primitiveType)
{
	Model& model = CreateEmptyModel(name);
	Primitives::ConstructPrimitive(primitiveType, model);
	return model;
}

const Model* ModelManager::GetModel(const Guid<Model> guid) const
{
	if (IsModelLoaded(guid))
		return &m_models.at(guid);
	else
		return nullptr;
}

const Guid<Model>* ModelManager::GetModelIDByName(std::string_view name) const
{
	if (IsModelLoaded(name))
		return &m_guidLookup.at(std::string(name));
	else
		return nullptr;
}

bool ModelManager::IsModelLoaded(const Guid<Model> guid) const
{
	return m_models.find(guid) != m_models.end();
}

bool ModelManager::IsModelLoaded(std::string_view name) const
{
	return m_guidLookup.find(name.data()) != m_guidLookup.end();
}

void LoadRequiredBuffers(Mesh& mesh)
{
	// REQUIRED BUFFERS
	glGenBuffers(Mesh::REQUIRED_BUFFERS, &mesh.VBOs[0]);

	// position
	auto& bufferPos = mesh.vertexBuffers.position;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOs[VertexAttribs::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, bufferPos.size() * sizeof(vec3_t), &bufferPos[0], GL_STATIC_DRAW);
	glVertexAttribPointer(VertexAttribs::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VertexAttribs::POSITION);

	// normal
	auto& bufferNormal = mesh.vertexBuffers.normal;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOs[VertexAttribs::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, bufferNormal.size() * sizeof(vec3_t), &bufferNormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(VertexAttribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VertexAttribs::NORMAL);

	// tangent
	auto& bufferTangent = mesh.vertexBuffers.normal;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOs[VertexAttribs::TANGENT]);
	glBufferData(GL_ARRAY_BUFFER, bufferTangent.size() * sizeof(vec4_t), &bufferTangent[0], GL_STATIC_DRAW);
	glVertexAttribPointer(VertexAttribs::TANGENT, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VertexAttribs::TANGENT);
}

void LoadOptionalBuffers(Mesh& mesh)
{
	// todo make this support multiple texCoords
	auto& bufferTexcoord = mesh.vertexBuffers.texCoord[0];
	if (bufferTexcoord.size() > 0)
	{
		glGenBuffers(1, &mesh.VBOs[VertexAttribs::TEXCOORD0]);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOs[VertexAttribs::TEXCOORD0]);
		glBufferData(GL_ARRAY_BUFFER, bufferTexcoord.size() * sizeof(vec2_t), &bufferTexcoord[0], GL_STATIC_DRAW);
		glVertexAttribPointer(VertexAttribs::TEXCOORD0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VertexAttribs::TEXCOORD0);
	}
}

void ModelManager::GPULoad(const Guid<Model> guid)
{
	Model* model = (Model*)GetModel(guid);
	SDL_assert(model);
	SDL_assert(!model->loadedOnGpu);

	for (Mesh& mesh : model->meshes)
	{
		glGenVertexArrays(1, &mesh.VAO);
		glBindVertexArray(mesh.VAO);

		LoadRequiredBuffers(mesh);
		LoadOptionalBuffers(mesh);

		// indices
		glGenBuffers(1, &mesh.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		int indicesSize = sizeof(uint32) * mesh.vertexIndices.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &mesh.vertexIndices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	model->loadedOnGpu = true;
}

void ModelManager::GPUUnload(const Guid<Model> guid)
{
	Model* model = (Model*)GetModel(guid);
	SDL_assert(model);
	SDL_assert(model->loadedOnGpu);

	for (Mesh& mesh : model->meshes)
	{
		glBindVertexArray(mesh.VAO);
		glDeleteBuffers(1, &mesh.EBO);
		glDeleteBuffers(VertexAttribs::COUNT, mesh.VBOs);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &mesh.VAO);

		memset(mesh.VBOs, 0, VertexAttribs::COUNT);
		mesh.EBO = 0;
		mesh.VAO = 0;
	}
	model->loadedOnGpu = false;
}
