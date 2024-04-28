#pragma once

#include "reEngine.hpp"
#include "reEngineImpl.hpp"
#include "reTime.hpp"
#include "reRenderer.hpp"
#include "reShaderManager.hpp"
#include "reInput.hpp"
#include "reCamera.hpp"

#include "SDL/SDL.h"

typedef SDL_Window* SDL_WindowHandle;

class reGame
{
public:
	double m_tickrate = 1.0 / 50.0;
	
	reGame() = default;
	~reGame();

	int Init();
	int Run();

	void DoInput();
	void Update();
	void Render();

	void Quit();

	bool IsRunning() const { return m_running; }

private:
	SDL_WindowHandle m_window = nullptr;
	reEngine::Impl* m_engineInstance = nullptr;
	bool m_running = false;

	reCamera m_camera = reCamera(70.f, 0.001f, 1000.0f);
};