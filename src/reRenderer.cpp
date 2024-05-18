#include "reRenderer.hpp"
#include "reEngine.hpp"
#include "reShaderManager.hpp"
#include "reTime.hpp"
#include "reCamera.hpp"
#include "reModelManager.hpp"
#include "reMaterialManager.hpp"
#include "reDebugRender.hpp"

#include "gles2.h"

using namespace reGraphics;

reRenderer::reRenderer(SDL_Window* window)
	: m_window(window)
{
	m_lightingSettings.m_ambient = vec3_t{ .5f, .5f, .5f };
	m_lightingSettings.m_sunCol = vec3_t{ 1.f, 1.f, 1.f };
	m_lightingSettings.m_sunDir = v3_norm({ 0.f, -.1f, 1.0f });

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

void reRenderer::AddModelToRender(const reModelInst& instance, const mat4_t& transform)
{
	const reModelManager* modelManager = reEngine::GetModelManager();
	const reModel& baseModel = *modelManager->GetModel(instance.m_baseModel);
	SDL_assert(&baseModel);
	SDL_assert(baseModel.m_loadedOnGpu);

	const std::vector<reGuid<reMaterial>>* materials;
	if (instance.m_materialOverrides.size() > 0)
	{
		materials = &instance.m_materialOverrides;
	}
	else if (baseModel.m_materials.size() > 0)
	{
		materials = &baseModel.m_materials;
	}
	else
	{
		materials = nullptr;
	}

	int count = baseModel.m_meshes.size();
	for (int i = 0; i < count; ++i)
	{
		reGuid<reMaterial> materialGuid = materials? materials->at(i) : reGuid<reMaterial>(0);

		if (m_renderList.find(materialGuid) == 0)
			m_renderList.emplace(materialGuid, std::vector<reRenderedMesh>());

		const reMesh& mesh = baseModel.m_meshes[i];
		m_renderList[materialGuid].push_back(reRenderedMesh{ &mesh, transform });
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

void reRenderer::DrawDebug(reDebugRender* debugRender)
{
	if (!debugRender)
		return;

	reMaterialManager* materialManager = reEngine::GetMaterialManager();
	materialManager->GetLinesMaterial().UseMaterial();

	SetUniformsGlobals();
	SetUniformsCamera(*m_camera);
	SetUniformsLighting();

	debugRender->Render();
	// loop through debug render lines and add two elements per line
	// glDrawElements(GL_LINE, numLines, GL_UNSIGNED_INT, 0);	
}

void reRenderer::GetRenderResolution(int& out_width, int& out_height) const
{
	SDL_assert(m_window);
	SDL_GL_GetDrawableSize(m_window, &out_width, &out_height);
}

void reRenderer::ClearScreen(const reColour& colour)
{
	int mask = GL_DEPTH_BUFFER_BIT;
	if (m_clearScreenColourEnabled)
	{
		vec4_t cVec = colour.GetVec4();
		glClearColor(cVec.x, cVec.y, cVec.z, cVec.w);
		mask |= GL_COLOR_BUFFER_BIT;
	}
	glClear(mask);
}

void reRenderer::SetUniformsGlobals()
{
	auto shaderManager = reEngine::GetShaderManager();
	auto time = reEngine::GetTime();

	float t = time->now;
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
	mat4_t iViewMat = camera.m_transform.ConstructMatrix();
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
	shaderManager->SetUniform("u_ambientCol", &m_lightingSettings.m_ambient);
	shaderManager->SetUniform("u_lightCol", &m_lightingSettings.m_sunCol);
	shaderManager->SetUniform("u_lightDir", &m_lightingSettings.m_sunDir);
}

void reRenderer::Render()
{
	ClearScreen(m_clearScreenColour);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

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

	// draw lines
	DrawDebug(reEngine::GetDebugRender());

	SDL_GL_SwapWindow(m_window);

	m_renderList.clear();
}