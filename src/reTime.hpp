#pragma once

#include "SDL/SDL.h"

struct reTime
{
	uint64_t now = 0;
	uint64_t last = 0;
	double time = 0;
	double deltaTime = 0;

	double accumulator = 0;
	double simTime = 0;

	double tickrate;

	reTime(uint64_t now_init, double tickrate_init) : now(now_init), tickrate(tickrate_init) {}

	void DTUpdate()
	{
		last = now;
		now = SDL_GetPerformanceCounter();
		deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());
		accumulator += deltaTime;

		time = (double)SDL_GetTicks64() * 1000.0;
	}

	void IncrementSimTime()
	{
		accumulator -= tickrate;
		simTime += tickrate;
	}
};
