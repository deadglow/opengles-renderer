#pragma once
#include "SDL/SDL.h"

struct reInput
{
	struct Axis
	{
		unsigned char pos : 1;
		unsigned char neg : 1;

		char Value() const
		{
			return pos - neg;
		}
	};
	
	Axis camXAxis;
	Axis camYAxis;
	Axis moveXAxis;
	Axis moveYAxis;
	Axis moveZAxis;
	bool reloadShader : 1;

	void PreUpdate()
	{
		reloadShader = false;
	}

	void ProcessKeyDown(const SDL_Event& event)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_d:
				moveXAxis.pos = 1;
				break;
			case SDLK_a:
				moveXAxis.neg = 1;
				break;
			case SDLK_e:
				moveYAxis.pos = 1;
				break;
			case SDLK_q:
				moveYAxis.neg = 1;
				break;
			case SDLK_w:
				moveZAxis.pos = 1;
				break;
			case SDLK_s:
				moveZAxis.neg = 1;
				break;
			case SDLK_RIGHT:
				camXAxis.pos = 1;
				break;
			case SDLK_LEFT:
				camXAxis.neg = 1;
				break;
			case SDLK_UP:
				camYAxis.pos = 1;
				break;
			case SDLK_DOWN:
				camYAxis.neg = 1;
				break;

			case SDLK_r:
				reloadShader = true;
				break;
		}
	}

	void ProcessKeyUp(const SDL_Event& event)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_d:
				moveXAxis.pos = 0;
				break;
			case SDLK_a:
				moveXAxis.neg = 0;
				break;
			case SDLK_e:
				moveYAxis.pos = 0;
				break;
			case SDLK_q:
				moveYAxis.neg = 0;
				break;
			case SDLK_w:
				moveZAxis.pos = 0;
				break;
			case SDLK_s:
				moveZAxis.neg = 0;
				break;
			case SDLK_RIGHT:
				camXAxis.pos = 0;
				break;
			case SDLK_LEFT:
				camXAxis.neg = 0;
				break;
			case SDLK_UP:
				camYAxis.pos = 0;
				break;
			case SDLK_DOWN:
				camYAxis.neg = 0;
				break;
		}
	}

	void Reset()
	{
		memset(this, 0, sizeof(this));
	}
};
