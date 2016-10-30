#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include <unordered_map>
#include "Hex.h"

class Player 
{
public:
	Player(sf::Color _color, int _points, std::string _name) : color(_color), points(_points), name(_name) {};

	sf::Color getColor() { return color; }
	int getPoints() { return points; }
	std::string getName() { return name; }

	void addPoints(int num) { points += num; }
	void extractPoints(int num) { points -= num; }

	std::list<std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>>>>>& 
		getList()
	{
		return hexesList;
	}
	void addToList(
		std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>>>>& it) { hexesList.push_back(it); };
	void removeFromList(
		std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>>>>& it) { hexesList.remove(it); };
private:
	sf::Color color;
	int points;
	std::string name;
	std::list<std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const hex::Hex, std::pair<sf::ConvexShape, sf::CircleShape>>>>>> hexesList;

};
