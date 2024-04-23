#include "reRenderer.hpp"
#include "reEngine.hpp"
#include "reShaderManager.hpp"
#include "reTime.hpp"
#include "reCamera.hpp"
#include "reModelManager.hpp"
#include "reMaterialManager.hpp"

#include "gles2.h"

using namespace reGraphics;

reRenderer::reRenderer(SDL_Window* window)
	: m_window(window)
{
	m_lightingSettings.m_ambient = vec3_t{ .1f, .1f, .1f };
	m_lightingSettings.m_sunCol = v3_norm({ 1.f, 1.f, 1.f });
	m_lightingSettings.m_sunDir = v3_norm({ 1.f, -.1f, .0f });

	glEnable(GL_CULL_FACE);
}

reRenderer::~reRenderer()
{
}

float reRenderer::GetAspectRatio() const
{
	int w, h;
	GetRenderResolution(w, h);

	return (float)w / (float)h;
}

void reRenderer::AddModelToRender(const reModelInst& model, const mat4_t& transform)
{
	const reModelManager* modelManager = reEngine::GetModelManager();
	const reModel* baseModel = modelManager->GetModel(model.m_baseModel);
	SDL_assert(baseModel);
	SDL_assert(baseModel->m_loadedOnGpu);

	int count = baseModel->m_meshes.size();
	for (int i = 0; i < count; ++i)
	{
		const reMesh& mesh = baseModel->m_meshes[i];

		reGuid<reMaterial> materialGuid(0);
		if (model.m_materialOverrides.size() > 0)
		{
			materialGuid = model.m_materialOverrides[i];
		}
		else if (baseModel->m_materials.size() > 0)
		{
			materialGuid = baseModel->m_materials[i];
		}

		if (m_renderList.find(materialGuid) == 0)
		{
			m_renderList.emplace(materialGuid, std::vector<reRenderedMesh>());
		}

		auto* meshList = &m_renderList[materialGuid];
		meshList->push_back(reRenderedMesh{ &mesh, transform });
	}
}

void reRenderer::RenderMesh(const reRenderedMesh& mesh)
{
	auto shaderManager = reEngine::GetShaderManager();

	mat4_t m2w = mesh.transform;
	mat4_t im2w = m4_invert_affine(m2w);
	shaderManager->SetUniform("u_m2w", &m2w);
	shaderManager->SetUniform("u_im2w", &im2w);

	glBindVertexArray(mesh.mesh->m_VAO);
	
	int indexCount = mesh.mesh->m_vertexIndices.size();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void reRenderer::GetRenderResolution(int& out_width, int& out_height) const
{
	SDL_assert(m_window);
	SDL_GL_GetDrawableSize(m_window, &out_width, &out_height);
}

void reRenderer::ClearScreen(const vec4_t& colour)
{
	if (m_clearScreenColourEnabled)
	{
		glClearColor(colour.x, colour.y, colour.z, colour.w);
	}
	glClear(GL_COLOR_BUFFER_BIT);
}

void reRenderer::SetUniformsGlobals()
{
	auto shaderManager = reEngine::GetShaderManager();
	auto time = reEngine::GetTime();

	float t = time->time;
	shaderManager->SetUniform("u_time", &t);

	float dt = time->deltaTime;
	shaderManager->SetUniform("u_deltaTime", &dt);

	float aspect = GetAspectRatio();
	shaderManager->SetUniform("u_aspect", &aspect);
}

void reRenderer::SetUniformsCamera(const reCamera& camera)
{
	auto shaderManager = reEngine::GetShaderManager();
	float aspect = GetAspectRatio();

	mat4_t perspective = camera.GetPerspectiveMatrix(aspect);
	mat4_t iPerspective = m4_invert_affine(perspective);
	mat4_t iViewMat = camera.GetTransform();
	mat4_t viewMat = m4_invert_affine(iViewMat);

	mat4_t vpMat = m4_mul(perspective, viewMat);
	mat4_t ivpMat = m4_invert_affine(vpMat);

	shaderManager->SetUniform("u_pMat", &perspective);
	shaderManager->SetUniform("u_ipMat", &iPerspective);
	shaderManager->SetUniform("u_vMat", &viewMat);
	shaderManager->SetUniform("u_ivMat", &iViewMat);
}

void reRenderer::SetUniformsLighting()
{
	auto shaderManager = reEngine::GetShaderManager();
}

void reRenderer::Render()
{
	ClearScreen(m_clearScreenColour);

	const reMaterialManager* materialManager = reEngine::GetMaterialManager();

	// draw objects
	for (auto& [matGuid, list] : m_renderList)
	{
		const reMaterial* material = materialManager->GetMaterial(matGuid);
		if (!material)
			material = &materialManager->GetDefaultMaterial();
		
		material->UseMaterial();

		// have to set these every time because UBOs not supported :(
		SetUniformsGlobals();
		SetUniformsCamera(*m_camera);
		SetUniformsLighting();

		for (const auto& mesh : list)
		{
			RenderMesh(mesh);
		}
	}


	// draw skybox
	// post process

	SDL_GL_SwapWindow(m_window);

	m_renderList.clear();
}