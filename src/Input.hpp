#pragma once
#include "SDL/SDL.h"

namespace Enoki
{
	
	struct Input
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
		bool pause : 1;
		vec2_t mouseAxes;

		void PreUpdate()
		{
			reloadShader = false;
			pause = false;
			mouseAxes = v2_zero;
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
					camYAxis.pos = 1;
					break;
				case SDLK_LEFT:
					camYAxis.neg = 1;
					break;
				case SDLK_UP:
					camXAxis.pos = 1;
					break;
				case SDLK_DOWN:
					camXAxis.neg = 1;
					break;

				case SDLK_r:
					reloadShader = true;
					break;

				case SDLK_p:
					pause = true;
					break;

				default:
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
					camYAxis.pos = 0;
					break;
				case SDLK_LEFT:
					camYAxis.neg = 0;
					break;
				case SDLK_UP:
					camXAxis.pos = 0;
					break;
				case SDLK_DOWN:
					camXAxis.neg = 0;
					break;
			}
		}

		void ProcessMouseMotion(const SDL_Event& event)
		{
			mouseAxes = vec2(event.motion.xrel, event.motion.yrel);
		}

		void Reset()
		{
			memset(this, 0, sizeof(this));
			mouseAxes = v2_zero;
		}
	};
}
