#include "reGame.hpp"

#define GLAD_GLES2_IMPLEMENTATION
#include "gles2.h"

#include "reMath.h"

using namespace reGraphics;

int SetupSDL(SDL_WindowHandle& window)
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

	return 0;
}

int reGame::Init()
{
	int result = SetupSDL(m_window);
	if (result != 0)
		return result;

	// set up engine components
	m_engineInstance = new reEngine::Impl(
		reRenderer(m_window),
		reTime(SDL_GetPerformanceCounter(), m_tickrate)
	);
	reEngine::SetInstance(m_engineInstance);

	reEngine::GetInput()->Reset();
	reEngine::GetPhysicsSystem()->Init();

	RegisterBaseListeners();

	return result;
}

void reGame::Dispose()
{
	if (m_engineInstance)
	{
		reEngine::GetPhysicsSystem()->Dispose();
		reEngine::SetInstance(nullptr);
		delete m_engineInstance;
	}
	
	m_window = nullptr;
	SDL_Quit();
}

int reGame::Run()
{
	m_running = true;
	while (m_running)
	{
		reTime* time = reEngine::GetTime();
		time->UpdateDT();

		DoInput();
		while (time->accumulator > time->deltaTime)
		{
			if (m_updateEnabled)
			{
				PhysicsUpdate();
				Update();
				time->simTime += time->tickrate;
			}

			time->accumulator -= time->tickrate;
		}
		Render();
	}

	return 0;
}

void reGame::DoInput()
{
	reInput* input = reEngine::GetInput();
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

		default:
			break;
		}
	}

	if (input->pause)
		m_updateEnabled = !m_updateEnabled;
}

void reGame::Update()
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

void reGame::PhysicsUpdate()
{
	auto* physics = reEngine::GetPhysicsSystem();
	physics->Update();
}

void reGame::Render()
{
	for (auto [guid, listener] : m_listeners)
	{
		listener->OnPreRender();
	}

	auto* physics = reEngine::GetPhysicsSystem();
	physics->DebugRender(true, false);

	auto* renderer = reEngine::GetRenderer();
	renderer->Render();

	for (auto [guid, listener] : m_listeners)
	{
		listener->OnPostRender();
	}
}

void reGame::RegisterBaseListeners()
{
	RegisterListener(reGuid<reIListener>::CreateGUID(), (reIListener*)&m_testListener);
}

void reGame::RegisterListener(const reGuid<reIListener> guid, reIListener* listener)
{
	SDL_assert(m_listeners.count(guid) == 0);
	m_listeners.emplace(guid, listener);
	listener->m_begun = false;
	listener->OnRegister();
}

void reGame::DeregisterListener(const reGuid<reIListener> guid)
{
	SDL_assert(m_listeners.count(guid) > 0);
	m_listeners.at(guid)->OnDeregister();
	m_listeners.erase(guid);
}

void reGame::Quit()
{
	m_running = false;
}
