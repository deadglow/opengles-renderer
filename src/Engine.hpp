#pragma once

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

namespace Enoki
{
	namespace Graphics
	{
		class ShaderManager;
		class ModelManager;
		class MaterialManager;
		class Renderer;
		class DebugRender;
	}

	namespace Physics
	{
		class PhysicsSystem;
	}

	class Time;
	class Input;

	class Engine
	{
	public:
		class Impl;

		Engine() = delete;

		static Engine::Impl* GetInstance();
		static void SetInstance(Engine::Impl* instance);

		static Graphics::ShaderManager* GetShaderManager();
		static Graphics::ModelManager* GetModelManager();
		static Graphics::MaterialManager* GetMaterialManager();
		static Graphics::Renderer* GetRenderer();
		static Physics::PhysicsSystem* GetPhysicsSystem();
		static Time* GetTime();
		static Input* GetInput();
		static Graphics::DebugRender* GetDebugRender();

	public:
		static Engine::Impl* m_instance;
	};
}