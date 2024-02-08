#include <stdio.h>

#include "reEngine.hpp"
#include "reEngineImpl.hpp"
#include "reTime.hpp"
#include "reShaderManager.hpp"
#include "reCamera.hpp"
#include "reInput.hpp"
#include "reRenderer.hpp"

#include "SDL/SDL.h"

#define GLAD_GLES2_IMPLEMENTATION
#include "gles2.h"

#define MATH_3D_IMPLEMENTATION
#include "math_3d.h"

static bool RUNNING;
static double TICKRATE = 1.0 / 50.0;


const float FOV = 70;

const char* DEFAULT_VS = "shader";
const char* DEFAULT_FS = "shader";

reCamera s_camera(FOV, 0.0001f, 1000.0f);

static void RequestQuit()
{
	RUNNING = false;
}

static void ForceQuit()
{
	RequestQuit();
	SDL_Quit();
}

static void ProcessInput()
{
	SDL_Event event;

	reInput* input = reEngine::GetInput();

	input->PreUpdate();
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			RequestQuit();
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

static void Update()
{
}

void DoInputStuff()
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
	
	s_camera.Rotate(vec3(0, 1, 0), angle * camInput.x);
	s_camera.Rotate(s_camera.GetRight(), angle * -camInput.y);

	vec3_t right = v3_muls(s_camera.GetRight(), moveInput.x);
	vec3_t up = v3_muls(vec3(0, 1, 0), moveInput.y);
	vec3_t forward = v3_muls(s_camera.m_forward, -moveInput.z);

	vec3_t deltaXZ = v3_norm(v3_add(right, forward));
	deltaXZ = v3_muls(deltaXZ, speed);
	up = v3_muls(up, speed);

	s_camera.Translate(v3_add(deltaXZ, up));
}

static void Render(SDL_Window* window)
{
	DoInputStuff();
	reGraphics::DoRenderStuff(window, s_camera);
}

SDL_Window* SetupSDL(int& result)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Error initialising SDL : %s\n", SDL_GetError());
		result = -1;
		return nullptr;
	}

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);

	printf("SDL successfully initialised!\n");

	auto window = SDL_CreateWindow("Ocean", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Window could not be created : %s\n", SDL_GetError());
		ForceQuit();
		result = -1;
		return nullptr;
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
		ForceQuit();
		result = -1;
		return nullptr;
	}

	gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
	printf("%s\n", (char*)glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(0);

	result = 0;
	return window;
}

int main(int argc, char* argv[])
{
	int result; auto window = SetupSDL(result);
	if (result != 0)
	{
		return result;
	}

	reEngine::Impl* engineInstance = new reEngine::Impl(
		reTime(SDL_GetPerformanceCounter(), TICKRATE)
	);
	reEngine::SetInstance(engineInstance);

	// reset input
	reEngine::GetInput()->Reset();

	reGraphics::reShaderManager* shaderMananager = reEngine::GetShaderManager();
	const reGraphics::reShaderProgram& shader = shaderMananager->LoadShaderProgramFromFiles("program", DEFAULT_VS, DEFAULT_FS);
	shaderMananager->UseShader(shader);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shader.program, "a_position");
    glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	RUNNING = true;
	while (RUNNING)
	{
		reTime* time = reEngine::GetTime();
		time->DTUpdate();

		ProcessInput();
		while (time->accumulator > time->deltaTime)
		{
			Update();
			time->IncrementSimTime();
		}
		
		Render(window);
	}

	delete engineInstance;

	SDL_Quit();

	return 0;
}