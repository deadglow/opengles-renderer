#include "reGame.hpp"

#define GLAD_GLES2_IMPLEMENTATION
#include "gles2.h"

#include "reMath.h"

#include "reTransform.h"

using namespace reGraphics;

#pragma region Temp
void TempInputStuff(reCamera& camera)
{
	reInput* input = reEngine::GetInput();
	reTime* time = reEngine::GetTime();
	if (input->reloadShader)
		reEngine::GetShaderManager()->ReloadAllShaders();

	vec2_t camInput = vec2(input->camXAxis.Value(), input->camYAxis.Value());
	vec3_t moveInput = vec3(input->moveXAxis.Value(), input->moveYAxis.Value(), input->moveZAxis.Value());

	float angle = 70;
	float speed = 10;

	angle *= M_DEG2RAD * time->deltaTime;
	speed *= time->deltaTime;
	
	camera.Rotate(vec3(0, 1, 0), angle * camInput.x);
	camera.Rotate(camera.GetRight(), angle * -camInput.y);

	vec3_t right = v3_muls(camera.GetRight(), moveInput.x);
	vec3_t up = v3_muls(vec3(0, 1, 0), moveInput.y);
	vec3_t forward = v3_muls(camera.m_forward, -moveInput.z);

	vec3_t deltaXZ = v3_norm(v3_add(right, forward));
	deltaXZ = v3_muls(deltaXZ, speed);
	up = v3_muls(up, speed);

	camera.Translate(v3_add(deltaXZ, up));
}

void TempModelStuff(reCamera& camera)
{
	auto* renderer = reEngine::GetRenderer();
	renderer->m_camera = &camera;
	
	auto* modelManager = reEngine::GetModelManager();
	auto modelGuid = *modelManager->GetModelIDByName("monkie");
	reTransform t;
	t.Reset();
	renderer->AddModelToRender(reModelInst(modelGuid), t.ComputeMatrix());
}
#pragma endregion

reGame::~reGame()
{
	if (m_engineInstance)
	{
		reEngine::SetInstance(nullptr);
		delete m_engineInstance;
	}
	
	m_window = nullptr;
	SDL_Quit();
}

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

	// reset input
	reEngine::GetInput()->Reset();

	// TEMP
	auto* modelManager = reEngine::GetModelManager();
	auto& model = modelManager->LoadModel("monkie");
	modelManager->GPULoad(model.m_guid);

	return result;
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
			Update();
			time->Tick();
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
}

void reGame::Update()
{
}

void reGame::Render()
{
	TempInputStuff(m_camera);
	TempModelStuff(m_camera);

	auto* renderer = reEngine::GetRenderer();
	renderer->Render();
}

void reGame::Quit()
{
	m_running = false;
}
