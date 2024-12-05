#pragma once

#include "Engine.hpp"
#include "EngineImpl.hpp"
#include "Time.hpp"
#include "Renderer.hpp"
#include "ShaderManager.hpp"
#include "Input.hpp"
#include "Guid.hpp"

#include "TestListener.hpp"

#include "SDL/SDL.h"

namespace Enoki
{
	class Game
	{
	public:
		double m_tickrate = 1.0 / 50.0;

		Game() = default;
		~Game() = default;

		int Init();
		void Dispose();
		int Run();

		void DoInput();
		void Update();
		void PhysicsUpdate();
		void Render();

		void RegisterBaseListeners();

		void RegisterListener(const Guid<IListener> guid, IListener* listener);
		void DeregisterListener(const Guid<IListener> guid);

		void Quit();

		bool IsRunning() const { return m_running; }

	private:
		SDL_Window* m_window = nullptr;
		Engine::Impl* m_engineInstance = nullptr;
		bool m_running = false;
		bool m_updateEnabled = true;

		std::unordered_map<Guid<IListener>, IListener*> m_listeners;

		TestListener m_testListener;
	};
	
}