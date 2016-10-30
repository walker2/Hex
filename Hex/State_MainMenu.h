#pragma once
#include "BaseState.h"
#include "EventManager.h"
class State_MainMenu : public BaseState
{
public:
	State_MainMenu(StateManager* stateManager);
	~State_MainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MouseClick(EventDetails* details);
private:
	sf::Font font;
	sf::Text text;

	sf::Vector2f buttonSize;
	sf::Vector2f buttonPos;
	unsigned int buttonPadding;

	sf::RectangleShape rects[3];
	sf::Text labels[3];
};

