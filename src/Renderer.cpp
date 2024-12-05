#include "Renderer.hpp"
#include "Engine.hpp"
#include "ShaderManager.hpp"
#include "Time.hpp"
#include "Camera.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"
#include "DebugRender.hpp"

#include "gles2.h"

using namespace Enoki::Graphics;

Renderer::Renderer(SDL_Window* window)
	: m_window(window)
{
	m_lightingSettings.m_ambient = vec3_t{ .5f, .5f, .5f };
	m_lightingSettings.m_sunCol = vec3_t{ 1.f, 1.f, 1.f };
	m_lightingSettings.m_sunDir = v3_norm({ 0.f, -.1f, 1.0f });

	glEnable(GL_CULL_FACE);
}

Renderer::~Renderer()
{
}

float Renderer::GetAspectRatio() const
{
	int w, h;
	GetRenderResolution(w, h);

	return (float)w / (float)h;
}

void Renderer::AddModelToRender(const ModelInst& instance, const mat4_t& transform)
{
	const ModelManager* modelManager = Engine::GetModelManager();
	const Model& baseModel = *modelManager->GetModel(instance.baseModel);
	SDL_assert(&baseModel);
	SDL_assert(baseModel.loadedOnGpu);

	const std::vector<Guid<Material>>* materials;
	if (instance.materialOverrides.size() > 0)
	{
		materials = &instance.materialOverrides;
	}
	else if (baseModel.materials.size() > 0)
	{
		materials = &baseModel.materials;
	}
	else
	{
		materials = nullptr;
	}

	int count = baseModel.meshes.size();
	for (int i = 0; i < count; ++i)
	{
		Guid<Material> materialGuid = materials? materials->at(i) : Guid<Material>(0);

		if (m_renderList.find(materialGuid) == 0)
			m_renderList.emplace(materialGuid, std::vector<RenderedMesh>());

		const Mesh& mesh = baseModel.meshes[i];
		m_renderList[materialGuid].push_back(RenderedMesh{ &mesh, transform });
	}
}

void Renderer::RenderMesh(const RenderedMesh& mesh)
{
	auto shaderManager = Engine::GetShaderManager();

	mat4_t m2w = mesh.transform;
	mat4_t im2w = m4_invert_affine(m2w);
	shaderManager->SetUniform("u_m2w", &m2w);
	shaderManager->SetUniform("u_im2w", &im2w);

	glBindVertexArray(mesh.mesh->VAO);
	
	int indexCount = mesh.mesh->vertexIndices.size();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::DrawDebug(DebugRender* debugRender)
{
	if (!debugRender)
		return;

	MaterialManager* materialManager = Engine::GetMaterialManager();
	materialManager->GetLinesMaterial().UseMaterial();

	SetUniformsGlobals();
	SetUniformsCamera(*camera);
	SetUniformsLighting();

	debugRender->Render();
	// loop through debug render lines and add two elements per line
	// glDrawElements(GL_LINE, numLines, GL_UNSIGNED_INT, 0);	
}

void Renderer::GetRenderResolution(int& out_width, int& out_height) const
{
	SDL_assert(m_window);
	SDL_GL_GetDrawableSize(m_window, &out_width, &out_height);
}

void Renderer::ClearScreen(const Colour& colour)
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

void Renderer::SetUniformsGlobals()
{
	auto shaderManager = Engine::GetShaderManager();
	auto time = Engine::GetTime();

	float t = time->now.count();
	shaderManager->SetUniform("u_time", &t);

	float dt = time->deltaTime.count();
	shaderManager->SetUniform("u_deltaTime", &dt);

	float aspect = GetAspectRatio();
	shaderManager->SetUniform("u_aspect", &aspect);
}

void Renderer::SetUniformsCamera(const Camera& camera)
{
	auto shaderManager = Engine::GetShaderManager();
	float aspect = GetAspectRatio();

	mat4_t perspective = camera.GetPerspectiveMatrix(aspect);
	mat4_t iPerspective = m4_invert_affine(perspective);
	mat4_t iViewMat = camera.transform.ConstructMatrix();
	mat4_t viewMat = m4_invert_affine(iViewMat);

	mat4_t vpMat = m4_mul(perspective, viewMat);
	mat4_t ivpMat = m4_invert_affine(vpMat);

	shaderManager->SetUniform("u_pMat", &perspective);
	shaderManager->SetUniform("u_ipMat", &iPerspective);
	shaderManager->SetUniform("u_vMat", &viewMat);
	shaderManager->SetUniform("u_ivMat", &iViewMat);
}

void Renderer::SetUniformsLighting()
{
	auto shaderManager = Engine::GetShaderManager();
	shaderManager->SetUniform("u_ambientCol", &m_lightingSettings.m_ambient);
	shaderManager->SetUniform("u_lightCol", &m_lightingSettings.m_sunCol);
	shaderManager->SetUniform("u_lightDir", &m_lightingSettings.m_sunDir);
}

void Renderer::Render()
{
	ClearScreen(m_clearScreenColour);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	const MaterialManager* materialManager = Engine::GetMaterialManager();

	// draw objects
	for (auto& [matGuid, list] : m_renderList)
	{
		const Material* material = materialManager->GetMaterial(matGuid);
		if (!material)
			material = &materialManager->GetDefaultMaterial();
		
		material->UseMaterial();

		// have to set these every time because UBOs not supported :(
		SetUniformsGlobals();
		SetUniformsCamera(*camera);
		SetUniformsLighting();

		for (const auto& mesh : list)
		{
			RenderMesh(mesh);
		}
	}

	// draw skybox
	// post process

	// draw lines
	DrawDebug(Engine::GetDebugRender());

	SDL_GL_SwapWindow(m_window);

	m_renderList.clear();
}