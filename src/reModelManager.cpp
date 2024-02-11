#include "reModelManager.hpp"
#include "SDL/SDL.h"
#include "gles2.h"

using namespace reGraphics;

reModelManager::reModelManager()
{
	// default shapes
	auto& model = LoadNewPrimitive("default_quad", rePrimitives::Quad);
	GPULoad(model.m_guid);
}

reModelManager::~reModelManager()
{
	UnloadAllModels();
}

void reModelManager::UnloadAllModels()
{
	for (auto it = m_models.cbegin(); it != m_models.cend(); ++it)
	{
		const reModel* model = &(it->second);
		if (model->m_loadedOnGpu)
		{
			GPUUnload(model->m_guid);
		}
	}

	m_models.clear();
	m_guidLookup.clear();
}

reModel& reModelManager::CreateEmptyModel(const char* name)
{
	SDL_assert(!IsModelLoaded(name));

	auto guid = reGuid<reModel>::CreateGUID();
	m_models.emplace(guid, reModel(guid, name));
	m_guidLookup.emplace(name, guid);

	return m_models[guid];
}

const reModel& reModelManager::LoadModel(const char* filename)
{
	SDL_assert(!IsModelLoaded(filename));

	reModel& model = CreateEmptyModel(filename);

	// use gltf loader here

	return model;
}

void reModelManager::UnloadModel(const reGuid<reModel> guid)
{
	SDL_assert(IsModelLoaded(guid));

	const reModel* model = &m_models[guid];
	if (model->m_loadedOnGpu)
	{
		GPUUnload(guid);
	}

	m_guidLookup.erase(model->m_name);
	m_models.erase(guid);
}

const reModel& reModelManager::LoadNewPrimitive(const char* name, const rePrimitives::PrimitiveType primitiveType)
{
	reModel& model = CreateEmptyModel(name);
	rePrimitives::ConstructPrimitive(primitiveType, model);
	return model;
}

const reModel* reModelManager::GetModel(const reGuid<reModel> guid) const
{
	if (IsModelLoaded(guid))
		return &m_models.at(guid);
	else
		return nullptr;
}

const reGuid<reModel>* reGraphics::reModelManager::GetModelIDByName(const char* name) const
{
	if (IsModelLoaded(name))
		return &m_guidLookup.at(name);
	else
		return nullptr;
}

bool reModelManager::IsModelLoaded(const reGuid<reModel> guid) const
{
	return m_models.find(guid) != m_models.end();
}

bool reGraphics::reModelManager::IsModelLoaded(const char* name) const
{
	return m_guidLookup.find(name) != m_guidLookup.end();
}

void reModelManager::GPULoad(const reGuid<reModel> guid)
{
	reModel* model = (reModel*)GetModel(guid);
	SDL_assert(model);

	SDL_assert(!model->m_loadedOnGpu);

	for (reMesh& mesh : model->m_meshes)
	{
		glGenVertexArrays(1, &mesh.m_VAO);
		glBindVertexArray(mesh.m_VAO);

		// vertices
		glGenBuffers(1, &mesh.m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBO);

		int vertsSize = sizeof(reVert) * mesh.m_vertices.size();
		glBufferData(GL_ARRAY_BUFFER, vertsSize, &mesh.m_vertices[0], GL_STATIC_DRAW);

		// indices
		glGenBuffers(1, &mesh.m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_EBO);

		int indicesSize = sizeof(reTri) * mesh.m_triangles.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &mesh.m_triangles[0], GL_STATIC_DRAW);

		// -- vertex attributes
		constexpr int vec3Size = sizeof(float) * 3;
		constexpr int positionOffset = 0;
		constexpr int normalOffset   = positionOffset + vec3Size;
		constexpr int tangentOffset  = normalOffset + vec3Size;
		constexpr int texCoordOffset = tangentOffset + vec3Size;

		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(reVert), (void*)positionOffset);
		glEnableVertexAttribArray(0);
		// normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(reVert), (void*)normalOffset);
		glEnableVertexAttribArray(1);
		// tangent
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(reVert), (void*)tangentOffset);
		glEnableVertexAttribArray(2);
		// texCoord
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(reVert), (void*)texCoordOffset);
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	model->m_loadedOnGpu = true;
}

void reModelManager::GPUUnload(const reGuid<reModel> guid)
{
	reModel* model = (reModel*)GetModel(guid);
	SDL_assert(model);

	SDL_assert(model->m_loadedOnGpu);

	for (reMesh& mesh : model->m_meshes)
	{
		glBindVertexArray(mesh.m_VAO);
		GLuint buffers[2] { mesh.m_VBO, mesh.m_EBO };
		glDeleteBuffers(2, buffers);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &mesh.m_VAO);

		mesh.m_EBO = 0;
		mesh.m_VBO = 0;
		mesh.m_VAO = 0;
	}
	model->m_loadedOnGpu = false;
}
