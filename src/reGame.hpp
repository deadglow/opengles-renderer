#pragma once

#include "reEngine.hpp"
#include "reEngineImpl.hpp"
#include "reTime.hpp"
#include "reRenderer.hpp"
#include "reShaderManager.hpp"
#include "reInput.hpp"
#include "reGuid.hpp"

template <typename K, typename V>
using umap = std::unordered_map<K, V>;

#include "reTestListener.hpp"


#include "SDL/SDL.h"

typedef SDL_Window* SDL_WindowHandle;

class reGame
{
public:
	double m_tickrate = 1.0 / 50.0;

	reGame() = default;
	~reGame() = default;

	int Init();
	void Dispose();
	int Run();

	void DoInput();
	void Update();
	void PhysicsUpdate();
	void Render();

	void RegisterBaseListeners();

	void RegisterListener(const reGuid<reIListener> guid, reIListener* listener);
	void DeregisterListener(const reGuid<reIListener> guid);

	void Quit();

	bool IsRunning() const { return m_running; }

private:
	SDL_WindowHandle m_window = nullptr;
	reEngine::Impl* m_engineInstance = nullptr;
	bool m_running = false;
	bool m_updateEnabled = true;

	umap<reGuid<reIListener>, reIListener*> m_listeners;

	reTestListener m_testListener;
};