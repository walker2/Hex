#include "Board.h"

Board::Board(int boardLength)
	:sideLength(boardLength)
{
}

void Board::setRectangleShape()
{
	for (int q = 1; q <= sideLength; q++)
	{
		for (int r = 1; r <= sideLength; r++)
		{
			hexes.insert({ hex::Hex(q, r, -q - r),{ sf::ConvexShape(6), sf::CircleShape(8) } });
		}
	}
}
void Board::setCustomShape()
{
	hexes.insert({ hex::Hex(1, 1, -2),{ sf::ConvexShape(6), sf::CircleShape(8) } });
	hexes.insert({ hex::Hex(1, 2, -3),{ sf::ConvexShape(6), sf::CircleShape(8) } });
}


void Board::init(sf::Color hexColor, sf::Color outlineColor, hex::Layout& layout)
{
	for (auto itr = hexes.begin(); itr != hexes.end(); ++itr)
	{
		std::vector<sf::Vector2f> corners = itr->first.polygon_corners(layout);

		itr->second.first.setFillColor(hexColor);
		itr->second.first.setOutlineColor(outlineColor);
		itr->second.first.setOutlineThickness(2);
		itr->second.first.setOrigin(0, 0);
		itr->second.first.setPosition(itr->first.toPixel(layout).x, itr->first.toPixel(layout).y);

		itr->second.second.setOrigin(8, 8);
		itr->second.second.setPosition({ itr->second.first.getPosition().x * 2, itr->second.first.getPosition().y * 2 });
		itr->second.second.setFillColor({ 0, 0, 0, 0 });

		itr->second.first.setPoint(0, sf::Vector2f(corners[0].x, corners[0].y));
		itr->second.first.setPoint(1, sf::Vector2f(corners[1].x, corners[1].y));
		itr->second.first.setPoint(2, sf::Vector2f(corners[2].x, corners[2].y));
		itr->second.first.setPoint(3, sf::Vector2f(corners[3].x, corners[3].y));
		itr->second.first.setPoint(4, sf::Vector2f(corners[4].x, corners[4].y));
		itr->second.first.setPoint(5, sf::Vector2f(corners[5].x, corners[5].y));
	}
}

void Board::Render(sf::RenderWindow &window)
{
	for (auto itr = hexes.begin(); itr != hexes.end(); ++itr)
	{
		window.draw(itr->second.first);
		window.draw(itr->second.second);
	}
}
