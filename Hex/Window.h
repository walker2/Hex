#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "EventManager.h"

class Window
{
public:
	Window();
	Window(const std::string& title, const sf::Vector2u size);
	~Window();

	void BeginDraw(); // Clear the window
	void EndDraw(); // Display the changes

	void Update();

	bool isDone();
	bool isFullscreen();
	bool isFocused();

	void ToggleFullscreen(EventDetails* details);
	void Close(EventDetails* details = nullptr);

	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();
	EventManager* GetEventManager();
	

private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Create();

	sf::RenderWindow window;
	EventManager eventManager;
	sf::Vector2u windowSize;
	std::string windowTitle;

	bool m_isDone;
	bool m_isFullscreen;
	bool m_isFocused;
};

