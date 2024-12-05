#pragma once
#include <unordered_map>
#include <vector>
#include "Guid.hpp"
#include "Material.hpp"
#include "Math.h"
#include "Colour.hpp"

class SDL_Window;

namespace Enoki
{
	class Camera;
}

namespace Enoki::Graphics
{
	class Mesh;
	class Model;
	class ModelInst;
	class DebugRender;

	struct LightingSettings
	{
		vec3_t m_ambient;
		vec3_t m_sunDir;
		vec3_t m_sunCol;
	};

	struct RenderedMesh
	{
		const Mesh* mesh;
		const mat4_t transform;
	};

	class Renderer
	{
	public:
		Renderer(SDL_Window* window);
		~Renderer();

		void Render();
		void GetRenderResolution(int& out_width, int& out_height) const;
		float GetAspectRatio() const;

		void AddModelToRender(const ModelInst& model, const mat4_t& transform);

		void RenderMesh(const RenderedMesh& mesh);

		void DrawDebug(DebugRender* debugRender);

	private:
		void ClearScreen(const Colour& colour);

		void SetUniformsGlobals();
		void SetUniformsCamera(const Camera& camera);
		void SetUniformsLighting();

	public:
		Camera* camera = nullptr;

	private:
		SDL_Window* m_window = nullptr;

		bool m_clearScreenColourEnabled = true;
		Colour m_clearScreenColour{ 0, 0, 0, 255 };
		LightingSettings m_lightingSettings;

		std::unordered_map<Guid<Material>, std::vector<RenderedMesh>> m_renderList;
	};
}