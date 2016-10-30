#include "State_Intro.h"
#include "StateManager.h"

State_Intro::State_Intro(StateManager * stateManager) : BaseState(stateManager){}

State_Intro::~State_Intro()
{
}

void State_Intro::OnCreate()
{
	timePassed = 0.0f;
	sf::Vector2u windowSize = stateManager->GetContext()->wind->GetRenderWindow()->getSize();

	introTexture.loadFromFile("intro.png");
	introSprite.setTexture(introTexture);
	introSprite.setOrigin(introTexture.getSize().x / 2.0f, introTexture.getSize().y / 2.0f);

	introSprite.setPosition(windowSize.x / 2.0f, 0);

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString({ " Press SPACE to continue " });
	text.setCharacterSize(15);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* eventManager = stateManager->GetContext()->eventManager;
	eventManager->AddCallback(StateType::Intro, "Intro_Continue", &State_Intro::Continue, this);

}

void State_Intro::OnDestroy()
{
	EventManager* eventManager = stateManager->GetContext()->eventManager;
	eventManager->RemoveCallback(StateType::Intro, "Intro_Continie");
}

void State_Intro::Activate()
{
}

void State_Intro::Deactivate()
{
}

void State_Intro::Update(const sf::Time & time)
{
	if (timePassed < 5.0f) { // Less than five seconds. 
		timePassed += time.asSeconds();  
		introSprite.setPosition(introSprite.getPosition().x,
			introSprite.getPosition().y + (48 * time.asSeconds()));  
	} 
}

void State_Intro::Draw()
{
	sf::RenderWindow* window = stateManager->GetContext()->wind->GetRenderWindow();

	window->draw(introSprite);
	if (timePassed >= 5.0f)
	{
		window->draw(text);
	}
}

void State_Intro::Continue(EventDetails * details)
{
	if (timePassed >= 5.0f)
	{
		stateManager->SwitchTo(StateType::MainMenu);
		stateManager->Remove(StateType::Intro);
	}
}
