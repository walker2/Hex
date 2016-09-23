#include "Wndow.h"


Window::Window()
{
	Setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string & title, const sf::Vector2u size)
{
	Setup(title, size);
}

Window::~Window()
{
	Destroy();
}

void Window::BeginDraw()
{
	window.clear(sf::Color(200, 200, 200));
}

void Window::EndDraw()
{
	window.display();
}


void Window::Update()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_isDone = true;
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
		{
			ToggleFullscreen();
		}
	}
}

bool Window::isDone()
{
	return m_isDone;
}

bool Window::isFullscreen()
{
	return m_isFullscreen;
}

sf::Vector2u Window::GetWindowSize()
{
	return windowSize;
}

sf::RenderWindow* Window::GetRenderWindow()
{
	return &window;
}

sf::Vector2i Window::getMousePosition()
{
	return sf::Mouse::getPosition(window);
}

void Window::ToggleFullscreen()
{
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

void Window::Draw(sf::Drawable & drawable)
{
	window.draw(drawable);
}

void Window::Setup(const std::string & title, const sf::Vector2u & size)
{
	windowTitle = title;
	windowSize = size;
	m_isFullscreen = false;
	m_isDone = false;
	Create();
}

void Window::Destroy()
{
	window.close();
}

void Window::Create()
{
	auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	window.create({ windowSize.x, windowSize.y, 32 }, windowTitle, style);
}
