#include "Game.hpp"

#define GLAD_GLES2_IMPLEMENTATION
#include "gles2.h"

#include "Math.h"

using namespace Enoki;

int SetupSDL(SDL_Window*& window)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Error initialising SDL : %s\n", SDL_GetError());
		return -1;
	}

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	printf("SDL successfully initialised!\n");

	window = SDL_CreateWindow("Ocean", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Window could not be created : %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	auto glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
		printf("glContext could not be created : %s\n", SDL_GetError());
		return -1;
	}

	gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
	printf("%s\n", (char*)glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(0);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	return 0;
}

int Game::Init()
{
	int result = SetupSDL(m_window);
	if (result != 0)
		return result;

	// set up engine components
	m_engineInstance = new Engine::Impl(
		Graphics::Renderer(m_window),
		Time(SDL_GetPerformanceCounter(), m_tickrate)
	);
	Engine::SetInstance(m_engineInstance);

	Engine::GetInput()->Reset();
	Engine::GetPhysicsSystem()->Init();

	RegisterBaseListeners();

	return result;
}

void Game::Dispose()
{
	if (m_engineInstance)
	{
		Engine::GetPhysicsSystem()->Dispose();
		Engine::SetInstance(nullptr);
		delete m_engineInstance;
	}
	
	m_window = nullptr;
	SDL_Quit();
}

int Game::Run()
{
	m_running = true;
	while (m_running)
	{
		Time* time = Engine::GetTime();
		time->UpdateDT();

		DoInput();
		while (time->accumulator > time->deltaTime)
		{
			if (m_updateEnabled)
			{
				PhysicsUpdate();
				Update();
				time->simTime += time->fixedDeltaTime;
			}

			time->accumulator -= time->fixedDeltaTime;
		}
		Render();
	}

	return 0;
}

void Game::DoInput()
{
	Input* input = Engine::GetInput();
	input->PreUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Quit();
			break;

		case SDL_KEYDOWN:
			input->ProcessKeyDown(event);
			break;

		case SDL_KEYUP:
			input->ProcessKeyUp(event);
			break;

		case SDL_MOUSEMOTION:
			input->ProcessMouseMotion(event);
			break;

		default:
			break;
		}
	}

	if (input->pause)
		m_updateEnabled = !m_updateEnabled;
}

void Game::Update()
{
	for (auto [guid, listener] : m_listeners)
	{
		if (!listener->m_begun)
		{
			listener->OnBegin();
			listener->m_begun = true;
		}

		listener->OnUpdate();
	}

	for (auto [guid, listener] : m_listeners)
	{
		listener->OnPostUpdate();
	}
}

void Game::PhysicsUpdate()
{
	auto* physics = Engine::GetPhysicsSystem();
	physics->Update();
}

void Game::Render()
{
	for (auto [guid, listener] : m_listeners)
	{
		listener->OnPreRender();
	}

	auto* physics = Engine::GetPhysicsSystem();
	physics->DebugRender(true, false);

	auto* renderer = Engine::GetRenderer();
	renderer->Render();

	for (auto [guid, listener] : m_listeners)
	{
		listener->OnPostRender();
	}
}

void Game::RegisterBaseListeners()
{
	RegisterListener(Guid<IListener>::CreateGUID(), (IListener*)&m_testListener);
}

void Game::RegisterListener(const Guid<IListener> guid, IListener* listener)
{
	SDL_assert(m_listeners.count(guid) == 0);
	m_listeners.emplace(guid, listener);
	listener->m_begun = false;
	listener->OnRegister();
}

void Game::DeregisterListener(const Guid<IListener> guid)
{
	SDL_assert(m_listeners.count(guid) > 0);
	m_listeners.at(guid)->OnDeregister();
	m_listeners.erase(guid);
}

void Game::Quit()
{
	m_running = false;
}
