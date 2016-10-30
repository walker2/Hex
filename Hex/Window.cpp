#include "Window.h"


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
	window.close();
}

void Window::BeginDraw()
{
	window.clear(sf::Color::Black);
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
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			eventManager.setFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			eventManager.setFocus(true);
		}
		eventManager.HandleEvent(event);
	}

	eventManager.Update();
}

bool Window::isDone()
{
	return m_isDone;
}

bool Window::isFullscreen()
{
	return m_isFullscreen;
}

bool Window::isFocused()
{
	return m_isFocused;
}

void Window::ToggleFullscreen(EventDetails * details)
{
	m_isFullscreen = !m_isFullscreen;
	window.close();
	Create();
}

void Window::Close(EventDetails * details)
{
	m_isDone = true;
}

sf::Vector2u Window::GetWindowSize()
{
	return windowSize;
}

sf::RenderWindow* Window::GetRenderWindow()
{
	return &window;
}

EventManager * Window::GetEventManager()
{
	return &eventManager;
}

void Window::Setup(const std::string & title, const sf::Vector2u & size)
{
	windowTitle = title;
	windowSize = size;
	m_isFullscreen = false;
	m_isDone = false;

	eventManager.AddCallback(StateType(0),"Fullscreen_toggle", &Window::ToggleFullscreen, this);
	eventManager.AddCallback(StateType(0),"Window_close", &Window::Close, this);

	Create();
}

void Window::Create()
{
	sf::Uint32 style = sf::Style::Default;
	if (m_isFullscreen) { style = sf::Style::Fullscreen; }

	window.create({ windowSize.x, windowSize.y, 32 }, windowTitle, style);
}
