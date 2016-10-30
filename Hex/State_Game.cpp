#include "State_Game.h"
#include "StateManager.h"


State_Game::State_Game(StateManager * stateManager)
	: BaseState(stateManager), hexBoard(4),
	layout(hex::LAYOUT_POINTY, sf::Vector2f(16, 16), sf::Vector2f(0, 0)),
	player1(sf::Color(52, 152, 219), 1, "PLAYER"), player2(sf::Color(231, 76, 60), 1, "AI")
{
}

State_Game::~State_Game()
{
}

void State_Game::OnCreate()
{

	hexes = hexBoard.getHexes();

	hexBoard.setRectangleShape();
	hexBoard.init(sf::Color(236, 240, 241), sf::Color::Black, layout);
	boardHeight = hexBoard.sideLength;
	boardWidth = boardHeight;

	auto hx1 = hexes->find({ hex::Hex(1, 1, -2) });
	auto hx2 = hexes->find({ hex::Hex(boardHeight, boardWidth, -boardHeight - boardWidth) });
	
	if (hx1 != hexes->end())
	{
		hx1->second.second.setFillColor(sf::Color(52, 152, 219));
		player1.addToList(hx1);
	}
	if (hx2 != hexes->end())
	{
		hx2->second.second.setFillColor(sf::Color(231, 76, 60));
		player2.addToList(hx2);
	}
		
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
		if (HandleMove(player1, player2))
		{// TODO: Make a player class with Colors and Points
			turn = !turn;
			std::cout << player1.getName() << " points score: " << player1.getPoints() << "\n";
			std::cout << player1.getName() << " number of items in list: " << player1.getList().size() << "\n\n";
			std::cout << player2.getName() << " points score: " << player2.getPoints() << "\n";
			std::cout << player2.getName() << " number of items in list: " << player2.getList().size() << "\n\n";
			std::cout << "======================================" << "\n";
		}
		//player1.getPoints();
		if (isGameOver(player2))
		{
			if (player2.getPoints() < player1.getPoints() + (boardHeight * boardWidth - (player1.getPoints() + player2.getPoints())))
			{
				std::cout << "GAME OVER " << player1.getName() << " WINS" << std::endl;
			}
			else
			{
				std::cout << "GAME OVER " << player2.getName() << " WINS" << std::endl;
			}
			
		}
	}
	else
	{
		if (HandleMove(player2, player1))
		{
			turn = !turn;
			std::cout << player2.getName() << " points score: " << player2.getPoints() << "\n";
			std::cout << player2.getName() << " number of items in list: " << player2.getList().size() << "\n\n";
			std::cout << player1.getName() << " points score: " << player1.getPoints() << "\n";
			std::cout << player1.getName() << " number of items in list: " << player1.getList().size() << "\n\n";
			std::cout << "======================================" << "\n";
		}

		if (isGameOver(player1))
		{
			if (player1.getPoints() < player2.getPoints() + (boardHeight * boardWidth - (player2.getPoints() + player1.getPoints())))
			{
				std::cout << "GAME OVER " << player2.getName() << " WINS" << std::endl;
				MainMenu(details);
			}
			else
			{
				std::cout << "GAME OVER " << player1.getName() << " WINS" << std::endl;
				MainMenu(details);
			}
		}
	}
}

bool State_Game::HandleMove(Player& player, Player& enemy)
{
	sf::Vector2i localPosition = stateManager->GetContext()->wind->GetEventManager()->GetMousePos(stateManager->GetContext()->wind->GetRenderWindow());
	

	auto hx = hexes->find(pixel_to_hex(layout, sf::Vector2f(localPosition.x, localPosition.y)));
	if (hx != hexes->end())
	{
		if (hx->second.first.getFillColor() == sf::Color(156, 155, 122) && hx->second.second.getFillColor() == sf::Color::Transparent)
		{
			hx->second.second.setFillColor(player.getColor());

			for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
			{
				itr2->second.first.setFillColor(sf::Color(236, 240, 241));
			}
			player.addPoints(1);
			player.addToList(hx);

			std::vector<hex::Hex> hxs = hx->first.range(1);
			for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
			{
				auto hex = hexes->find(*itr);
				if (hex != hexes->end() && hex->second.second.getFillColor() == enemy.getColor())
				{
					hex->second.second.setFillColor(player.getColor());
					
					player.addPoints(1);
					player.addToList(hex);
					enemy.extractPoints(1);
					enemy.removeFromList(hex);
				}

			}
			
			return true;
		}
		else if (hx->second.first.getFillColor() == sf::Color(255, 211, 147) && hx->second.second.getFillColor() == sf::Color::Transparent)
		{
			prevHex->second.second.setFillColor(sf::Color::Transparent);
			player.removeFromList(prevHex);
			hx->second.second.setFillColor(player.getColor());
			player.addToList(hx);

			for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
			{
				itr2->second.first.setFillColor(sf::Color(236, 240, 241));
			}

			std::vector<hex::Hex> hxs = hx->first.range(1);
			for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
			{
				auto hex = hexes->find(*itr);
				if (hex != hexes->end() && hex->second.second.getFillColor() == enemy.getColor())
				{
					hex->second.second.setFillColor(player.getColor());

					player.addPoints(1);
					player.addToList(hex);
					enemy.extractPoints(1);
					enemy.removeFromList(hex);
				}

			}
			return true;
		}
		for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
		{
			itr2->second.first.setFillColor(sf::Color(236, 240, 241));
		}

		if (hx->second.second.getFillColor() != player.getColor())
		{
			return false;
		}
		else
		{
			std::vector<hex::Hex> hxs = hx->first.range(2);
			for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
			{
				auto hex = hexes->find(*itr);
				if (hex != hexes->end())
				{
					if (hex->first.distance(hx->first) <= 1)
					{
						hex->second.first.setFillColor(sf::Color(156, 155, 122));
					}
					else
					{
						hex->second.first.setFillColor(sf::Color(255, 211, 147));
					}

				}
			}
			prevHex = hx;
		}
	}
	return false;
}

bool State_Game::isGameOver(Player& player)
{
	if (player.getPoints() == 0)
		return true;
	auto list = player.getList();



	for (auto elem : list)
	{
		std::vector<hex::Hex> hxs = elem->first.range(2);
		for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
		{
			auto hex = hexes->find(*itr);
			if (hex != hexes->end())
			{
				if (hex->second.second.getFillColor() == sf::Color::Transparent)
					return false;
			}
		}
	}
	return true;
}
