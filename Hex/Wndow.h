
#include <string>
#include "SFML\Graphics.hpp"
#pragma once
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
	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();

	sf::Vector2i getMousePosition();
	void ToggleFullscreen();

	void Draw(sf::Drawable& drawable);

private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Destroy();
	void Create();

	sf::RenderWindow window;
	sf::Vector2u windowSize;
	std::string windowTitle;
	bool m_isDone;
	bool m_isFullscreen;
};

