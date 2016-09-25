#pragma once
#include "Board.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <memory>
#include <algorithm>


class Game
{
public:
	Game();
	~Game() {};

	
	void HandleInput();
	void Update();
	void Render();

	sf::Time GetElapsed();
	void RestartClock();

	void MakeMove(EventDetails* l_details);
	bool handlePlayerMove(sf::Color moveColor, sf::Color enemyColor);

	Window* GetWindow();


private:
	int turn;
	int boardHeight;
	int boardWidth;
	bool playersNumber;
	int playerOneScore;
	int playerTwoScore;


	Board hexBoard;
	Window window;
	
	std::unordered_map<hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>* hexes;
	hex::Layout layout;
};

