#pragma once

#include "SDL/SDL.h"

struct reTime
{
	uint64_t sdl_now = 0;
	uint64_t sdl_last = 0;
	double now = 0;
	double deltaTime = 0;

	double timescale = 1.0;
	bool paused = false;

	double accumulator = 0;
	double simTime = 0;

	double tickrate;

	reTime(uint64_t now_init, double tickrate_init) : sdl_now(now_init), tickrate(tickrate_init) {}

	void UpdateDT()
	{
		sdl_last = sdl_now;
		sdl_now = SDL_GetPerformanceCounter();
		deltaTime = (double)((sdl_now - sdl_last) / (double)SDL_GetPerformanceFrequency());
		deltaTime *= paused? 0.0 : timescale;
		accumulator += deltaTime;

		now = (double)SDL_GetTicks64() / 1000.0;
	}

	void Tick()
	{

	}
};
