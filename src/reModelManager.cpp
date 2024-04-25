#include "reModelManager.hpp"
#include "SDL/SDL.h"
#include "gles2.h"

#include "reModelLoader.hpp"

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
	bool success = reModelLoader::LoadFromFile(filename, model);
	SDL_assert(success);

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

const reGuid<reModel>* reModelManager::GetModelIDByName(const char* name) const
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

bool reModelManager::IsModelLoaded(const char* name) const
{
	return m_guidLookup.find(name) != m_guidLookup.end();
}

void LoadRequiredBuffers(reMesh& mesh)
{
	// REQUIRED BUFFERS
	glGenBuffers(reMesh::REQUIRED_BUFFERS, &mesh.m_VBOs[0]);

	// position
	auto& bufferPos = mesh.m_vertexBuffers.position;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[reVertexAttribs::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, bufferPos.size() * sizeof(vec3_t), &bufferPos[0], GL_STATIC_DRAW);
	glVertexAttribPointer(reVertexAttribs::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(reVertexAttribs::POSITION);

	// normal
	auto& bufferNormal = mesh.m_vertexBuffers.normal;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[reVertexAttribs::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, bufferNormal.size() * sizeof(vec3_t), &bufferNormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(reVertexAttribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(reVertexAttribs::NORMAL);

	// tangent
	auto& bufferTangent = mesh.m_vertexBuffers.normal;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[reVertexAttribs::TANGENT]);
	glBufferData(GL_ARRAY_BUFFER, bufferTangent.size() * sizeof(vec4_t), &bufferTangent[0], GL_STATIC_DRAW);
	glVertexAttribPointer(reVertexAttribs::TANGENT, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(reVertexAttribs::TANGENT);
}

void LoadOptionalBuffers(reMesh& mesh)
{
	// todo make this support multiple texCoords
	auto& bufferTexcoord = mesh.m_vertexBuffers.texCoord[0];
	if (bufferTexcoord.size() > 0)
	{
		glGenBuffers(1, &mesh.m_VBOs[reVertexAttribs::TEXCOORD0]);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[reVertexAttribs::TEXCOORD0]);
		glBufferData(GL_ARRAY_BUFFER, bufferTexcoord.size() * sizeof(vec2_t), &bufferTexcoord[0], GL_STATIC_DRAW);
		glVertexAttribPointer(reVertexAttribs::TEXCOORD0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(reVertexAttribs::TEXCOORD0);
	}
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

		LoadRequiredBuffers(mesh);
		LoadOptionalBuffers(mesh);

		// indices
		glGenBuffers(1, &mesh.m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_EBO);
		int indicesSize = sizeof(GLuint) * mesh.m_vertexIndices.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &mesh.m_vertexIndices[0], GL_STATIC_DRAW);

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
		glDeleteBuffers(1, &mesh.m_VAO);
		glDeleteBuffers(reVertexAttribs::COUNT, mesh.m_VBOs);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &mesh.m_VAO);

		memset(mesh.m_VBOs, 0, reVertexAttribs::COUNT);
		mesh.m_EBO = 0;
		mesh.m_VAO = 0;
	}
	model->m_loadedOnGpu = false;
}
