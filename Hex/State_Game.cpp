#include "State_Game.h"
#include "StateManager.h"


State_Game::State_Game(StateManager * stateManager)
	: BaseState(stateManager), hexBoard(Player::getBoardSize()),
	layout(hex::LAYOUT_POINTY, sf::Vector2f(16, 16), sf::Vector2f(0, 0)),
	playersNumber(Player::getPlayersNumber()) 
{
}

State_Game::~State_Game()
{
}

void State_Game::OnCreate()
{

	hexes = hexBoard.getHexes();
	if (!playersNumber)
	{
		std::cout << "Single player game mode is not implemented yet" << std::endl;
		return;
	}

	hexBoard.setRectangleShape();
	hexBoard.init(sf::Color::White, sf::Color::Black, layout);
	boardHeight = hexBoard.sideLength;
	boardWidth = boardHeight;

	hexes->begin()->second.second.setFillColor(sf::Color::Red);
	auto hx = hexes->find(hex::pixel_to_hex(layout, sf::Vector2f(64, 64)));
	if (hx != hexes->end())
		hx->second.second.setFillColor(sf::Color::Blue);
	playerOneScore = playerTwoScore = 1;

	EventManager* eventManager = stateManager->GetContext()->eventManager;
	eventManager->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	eventManager->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);
	eventManager->AddCallback(StateType::Game, "Mouse_Left", &State_Game::MakeMove, this);
}

void State_Game::OnDestroy()
{
	EventManager* eventManager = stateManager->GetContext()->eventManager;
	eventManager->RemoveCallback(StateType::Game, "Key_Escape");
	eventManager->RemoveCallback(StateType::Game, "Key_P");
	eventManager->RemoveCallback(StateType::Game, "Mouse_Left");
}

void State_Game::Activate()
{
}

void State_Game::Deactivate()
{
}

void State_Game::Update(const sf::Time & time)
{
}

void State_Game::Draw()
{
	hexBoard.Render(*(stateManager->GetContext()->wind->GetRenderWindow()));
}

void State_Game::MainMenu(EventDetails * details)
{
	stateManager->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails * details)
{
	stateManager->SwitchTo(StateType::Paused);
}

void State_Game::MakeMove(EventDetails * details)
{
	static bool turn = 0;
	if (!turn)
	{
		if (HandleMove(sf::Color::Red, sf::Color::Blue)) // TODO: Make a player class with Colors and Points
			turn = !turn;
	}
	else
	{
		if (HandleMove(sf::Color::Blue, sf::Color::Red))
			turn = !turn;
	}
}

bool State_Game::HandleMove(sf::Color moveColor, sf::Color enemyColor)
{
	sf::Vector2i localPosition = stateManager->GetContext()->wind->GetEventManager()->GetMousePos(stateManager->GetContext()->wind->GetRenderWindow());

	auto hx = hexes->find(pixel_to_hex(layout, sf::Vector2f(localPosition.x, localPosition.y)));
	if (hx != hexes->end())
	{
		if (hx->second.first.getFillColor() == sf::Color(0, 128, 0, 128) && hx->second.second.getFillColor() == sf::Color::Transparent)
		{
			hx->second.second.setFillColor(moveColor);
			turn = 1;
			for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
			{
				itr2->second.first.setFillColor(sf::Color::White);
			}
			playerOneScore++;

			std::vector<hex::Hex> hxs = hx->first.range(1);
			for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
			{
				auto hex = hexes->find(*itr);
				if (hex != hexes->end() && hex->second.second.getFillColor() == enemyColor)
				{
					hex->second.second.setFillColor(moveColor);
					playerOneScore++;
					playerTwoScore--;
				}

			}
			std::cout << "Player 1 score: " << playerOneScore << "\n";
			return true;
		}
		for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
		{
			itr2->second.first.setFillColor(sf::Color::White);
		}

		if (hx->second.second.getFillColor() != moveColor)
		{
			return false;
		}
		else
		{
			std::vector<hex::Hex> hxs = hx->first.range(1);
			for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
			{
				auto hex = hexes->find(*itr);
				if (hex != hexes->end())
					hex->second.first.setFillColor(sf::Color(0, 128, 0, 128));
			}
		}
	}
	return false;
}
