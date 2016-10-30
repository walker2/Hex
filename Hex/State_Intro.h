#pragma once
#include "BaseState.h"
#include "EventManager.h"
class State_Intro : public BaseState
{
public:
	State_Intro(StateManager* stateManager);
	~State_Intro();
	
	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void Continue(EventDetails* details);
private:
	sf::Texture introTexture;
	sf::Sprite introSprite;
	sf::Font font;
	sf::Text text;
	
	float timePassed;
};

