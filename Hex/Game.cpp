#include "Game.h"




Game::Game()
	: window("HEXX", sf::Vector2u(800, 600)), stateManager(&context)
{
	clock.restart();
	srand(time(nullptr));

	context.wind = &window;
	context.eventManager = window.GetEventManager();
	stateManager.SwitchTo(StateType::Intro);
}


void Game::Update()
{
	window.Update();
	stateManager.Update(elapsed);
}

void Game::Render()
{
	window.BeginDraw();

	stateManager.Draw();

	window.EndDraw();
}

void Game::LateUpdate()
{
	stateManager.ProccessRequests();
	RestartClock();
}

sf::Time Game::GetElapsed()
{
	return clock.getElapsedTime();
}

void Game::RestartClock()
{
	elapsed = clock.restart();
}

Window * Game::GetWindow()
{
	return &window;
}
