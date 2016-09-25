#include "Game.h"




Game::Game()
	: window("HEXX", sf::Vector2u(800, 600)), layout(hex::LAYOUT_POINTY, sf::Vector2f(16, 16), sf::Vector2f(0, 0))
	, playersNumber(Player::getPlayersNumber()), hexBoard(Player::getBoardSize())
{

	hexes = hexBoard.getHexes();
	if (!playersNumber)
	{
		std::cout << "Single player game mode is not emplemented yet" << std::endl;
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

	window.GetEventManager()->addCallback("Move", &Game::MakeMove, this);

}


void Game::HandleInput()
{
	
}

void Game::Update()
{
	window.Update();
}

void Game::Render()
{
	window.BeginDraw();

	hexBoard.Render(*window.GetRenderWindow());


	window.EndDraw();
}

void Game::MakeMove(EventDetails * l_details)
{
	static bool turn = 0;
	if (!turn)
	{
		if (handlePlayerMove(sf::Color::Red, sf::Color::Blue)) // TODO: Make a player class with Colors and Points
			turn = !turn;
	}
	else
	{
		if (handlePlayerMove(sf::Color::Blue, sf::Color::Red))
			turn = !turn;
	}

		
		/*	else
			{
				auto hx = hexes->find(pixel_to_hex(layout, sf::Vector2f(localPosition.x, localPosition.y)));
				if (hx != hexes->end())
				{
					if (hx->second.first.getFillColor() == sf::Color(0, 128, 0, 128) && hx->second.second.getFillColor() == sf::Color::Transparent)
					{
						hx->second.second.setFillColor(sf::Color::Blue);
						turn = 0;
						for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
						{
							itr2->second.first.setFillColor(sf::Color::White);
						}
						playerTwoScore++;
						std::vector<hex::Hex> hxs = hx->first.range(1);
						for (auto itr = hxs.begin(); itr != hxs.end(); ++itr)
						{
							auto hex = hexes->find(*itr);
							if (hex != hexes->end() && hex->second.second.getFillColor() == sf::Color::Red)
							{
								hex->second.second.setFillColor(sf::Color::Blue);
								playerTwoScore++;
								playerOneScore--;
							}

						}
						std::cout << "Player 2 score: " << playerTwoScore << "\n";
						return;
					}
					for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
					{
						itr2->second.first.setFillColor(sf::Color::White);
					}

					if (hx->second.second.getFillColor() != sf::Color::Blue)
					{
						return;
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
			}
		}*/
	//}
}

bool Game::handlePlayerMove(sf::Color moveColor, sf::Color enemyColor)
{
	sf::Vector2i localPosition = window.GetEventManager()->GetMousePos(window.GetRenderWindow());

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

Window * Game::GetWindow()
{
	return &window;
}
