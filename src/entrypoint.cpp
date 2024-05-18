#include <stdio.h>

#include "reGame.hpp"

int main(int argc, char* argv[])
{
	reGame game;

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