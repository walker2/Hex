#include <iostream>
#include "Game.h"

int main(int argc, char* argv[])
{
	std::cout << "Hexxagon v0" << std::endl;
	Game game;
	

	while (!game.GetWindow()->isDone())
	{
		game.Update();
		game.Render();
		game.HandleInput();
	}
	

	return 0;
}