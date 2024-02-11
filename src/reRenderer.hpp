#pragma once
#include <unordered_map>
#include <vector>
#include "reGuid.hpp"
#include "reMaterial.hpp"
#include "math_3d.h"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

class SDL_Window;
class reCamera;

namespace reGraphics
{
	class reMesh;
	class reModel;
	class reModelInst;

	struct reLightingSettings
	{
		vec3_t m_ambient;
		vec3_t m_sunDir;
		vec3_t m_sunCol;
	};

	struct reRenderedMesh
	{
		const reMesh* mesh;
		const mat4_t transform;
	};

	class reRenderer
	{
	public:
		reRenderer(SDL_Window* window);
		~reRenderer();

		void Render();
		void GetRenderResolution(int& out_width, int& out_height) const;
		float GetAspectRatio() const;

		void AddModelToRender(const reModelInst& model, const mat4_t& transform);

		void RenderMesh(const reRenderedMesh& mesh);

	private:
		void ClearScreen(const vec4_t& colour);

		void SetUniformsGlobals();
		void SetUniformsCamera(const reCamera& camera);
		void SetUniformsLighting();

	public:
		reCamera* m_camera = nullptr;

	private:
		SDL_Window* m_window = nullptr;

		bool m_clearScreenColourEnabled = true;
		vec4_t m_clearScreenColour{ 0.f, 0.f, 0.f, 1.f };
		reLightingSettings m_lightingSettings;

		umap<reGuid<reMaterial>, std::vector<reRenderedMesh>> m_renderList;
		
	};
}