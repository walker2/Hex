#pragma once
#include <vector>
#include "Hex.h"
#include <unordered_set>
#include <unordered_map>
#include <SFML\Graphics.hpp>

namespace std
{
	template <> struct hash<hex::Hex> {
		size_t operator()(const hex::Hex& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q);
			size_t hr = int_hash(h.r);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}


class Board
{
public:
	Board(int boardLength);
	~Board() {};
	std::unordered_map<hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>* getHexes() { return &hexes; };
	const unsigned int sideLength;

	void setRectangleShape();
	void init(sf::Color hexColor, sf::Color outlineColor,  hex::Layout& layout);
	void Render(sf::RenderWindow& l_window);
	void Update();
	
private:
	std::unordered_map<hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>> hexes;
};

