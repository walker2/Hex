#pragma once
#include "Board.h"
#include "Player.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <memory>
#include <algorithm>


class Game
{
public:
	Game();
	~Game() {};

	
	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
private:
	void RestartClock();

	Window window;
	StateManager stateManager;
	SharedContext context;
	sf::Clock clock;
	sf::Time elapsed;
	
};

