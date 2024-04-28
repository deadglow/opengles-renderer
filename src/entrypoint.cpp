#include <stdio.h>

#include "reGame.hpp"

int main(int argc, char* argv[])
{
	// init SDL
	reGame game;

	int result = game.Init();
	if (result == 0)
	{
		result = game.Run();
	}

	SDL_Quit();

	return result;
}