#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "Board.h"
#include "Player.h"

class State_Game : public BaseState
{
public:
	State_Game(StateManager* stateManager);
	~State_Game();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MainMenu(EventDetails* details);
	void Pause(EventDetails* details);

	void MakeMove(EventDetails* details);
	bool HandleMove(sf::Color moveColor, sf::Color enemyColor);
private:
	int turn;
	int boardHeight;
	int boardWidth;
	bool playersNumber;
	int playerOneScore;
	int playerTwoScore;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f increment;

	Board hexBoard;
	hex::Layout layout;

	std::unordered_map<hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>* hexes;
};

