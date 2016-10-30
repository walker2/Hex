#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "Board.h"
#include "Player.h"

using Hexes = std::unordered_map<hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>;
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
	bool HandleMove(Player& player, Player& enemy);
	bool isGameOver(Player& player);
private:
	int turn;
	int boardHeight;
	int boardWidth;
	Player player1;
	Player player2;
	int playerOneScore;
	int playerTwoScore;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f increment;

	Board hexBoard;
	hex::Layout layout;

	Hexes* hexes;
	std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>>>> prevHex;
};

