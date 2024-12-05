#include <stdio.h>

#include "Game.hpp"

int main(int argc, char* argv[])
{
	Enoki::Game game;

	int result = game.Init();
	if (result == 0)
	{
		result = game.Run();
	}
	else
	{
		printf("Game failed to init, error result: %i", result);
	}
	game.Dispose();

	return result;
}