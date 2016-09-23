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


}


void Game::HandleInput()
{
	int i = 0;
	sf::Vector2i localPosition = window.getMousePosition();

	hexes->begin()->second.second.setFillColor(sf::Color::Red);
	auto hx = hexes->find(hex::pixel_to_hex(layout, sf::Vector2f(64, 64)));
	if (hx != hexes->end())
		hx->second.second.setFillColor(sf::Color::Blue);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		auto hx = hexes->find(pixel_to_hex(layout, sf::Vector2f(localPosition.x, localPosition.y)));
		if (hx != hexes->end())
		{
			if (hx->second.first.getFillColor() == sf::Color(0, 128, 0, 128))
			{
				hx->second.second.setFillColor(sf::Color::Red);
				turn = false;
			}
			for (auto itr2 = hexes->begin(); itr2 != hexes->end(); ++itr2)
			{
				itr2->second.first.setFillColor(sf::Color::White);
			}

			if (hx->second.second.getFillColor() != sf::Color::Red)
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

Window * Game::GetWindow()
{
	return &window;
}
