#pragma once

#include "Renderer.hpp"
#include "ModelManager.hpp"
#include "ShaderManager.hpp"
#include "MaterialManager.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "DebugRender.hpp"

namespace Enoki
{
	class Engine::Impl
	{
	public:
		Impl(const Graphics::Renderer& renderer, const Time& time)
			: renderer(renderer)
			, time(time)
		{
		}

		Time time;
		Input input;

		Graphics::ShaderManager shaderManager;
		Graphics::ModelManager modelManager;
		Graphics::MaterialManager materialManager;
		Graphics::Renderer renderer;
		Graphics::DebugRender debugRender;

		Physics::PhysicsSystem physicsSystem;

	};
}