#pragma once

#include "CommonTypes.hpp"
#include "SDL/SDL.h"

namespace Enoki
{
	struct Time
	{
		uint64 sdl_now = 0;
		uint64 sdl_last = 0;
		SecondsF64 now = SecondsF64::zero();
		SecondsF64 deltaTime = SecondsF64::zero();

		MillisecondsU64 fixedDeltaTime = MillisecondsU64(1000 / 50);

		double timescale = 1.0;
		bool paused = false;

		SecondsF64 accumulator = SecondsF64::zero();

		MillisecondsU64 simTime = SecondsU64::zero();

		Time(uint64 now_init, double deltaTime_init) : sdl_now(now_init), deltaTime(deltaTime_init) {}

		void UpdateDT()
		{
			sdl_last = sdl_now;
			sdl_now = SDL_GetPerformanceCounter();
			deltaTime = SecondsF64((sdl_now - sdl_last) / (double)SDL_GetPerformanceFrequency());
			deltaTime *= paused? 0.0 : timescale;
			accumulator += deltaTime;

			now = SecondsF64(SDL_GetTicks64() / 1000.0);
		}

		void Tick()
		{

		}
	};
}
