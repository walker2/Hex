#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>


enum class EventType //Enumerator for all the event types
{
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo
{
	EventInfo() { code = 0; }
	EventInfo(int event) { code = event; }
	union 
	{
		int code;
	};
};

struct EventDetails
{
	EventDetails(const std::string& bindName) : name(bindName) {};
	std::string name;

	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode;

	void Clear()
	{
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		mouse = sf::Vector2i(0, 0);
		mouseWheelDelta = 0;
		keyCode = -1;
	}
};

using Events = std::vector<std::pair<EventType, EventInfo>>; // Container for holding event information

struct Binding // Struct for holding all event information including vector Events
{
	Binding(const std::string& _name) : name(_name), details(_name), c(0) {};
	void BindEvent(EventType type, EventInfo info = EventInfo()) // Method that takes type and info information and add it to the vector
	{
		events.emplace_back(type, info);
	}

	Events events;
	std::string name;
	int c; // Counter for ongoing events

	EventDetails details;

};

using Bindings = std::unordered_map<std::string, Binding*>; // Container for storing Bindings
// Using unordered map ensures that there's only one Binding per action
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>; // We define callback container which stores string key and C++11 function wrapper

enum class StateType;

using Callbacks = std::unordered_map<StateType, CallbackContainer>; 

class EventManager
{
public:
	EventManager();
	~EventManager();

	bool addBinding(Binding *binding);
	bool removeBinding(std::string name);

	void setFocus(const bool& focus);
	void SetCurrentState(StateType state);

	template<class T>
	bool AddCallback(StateType state, const std::string& name, void(T::*func)(EventDetails*), T* instance)
	{
		auto itr = callbacks.emplace(state, CallbackContainer()).first;
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return itr->second.emplace(name, temp).second;
	}

	bool RemoveCallback(StateType state, const std::string& name) {
		auto itr = callbacks.find(state);
		if (itr == callbacks.end()) 
			return false; 

		auto itr2 = itr->second.find(name);
		if (itr2 == itr->second.end()) 
			return false; 

		itr->second.erase(name);
		return true;
	}

	void HandleEvent(sf::Event& event);
	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* wind = nullptr)
	{
		return (wind ? sf::Mouse::getPosition(*wind) : sf::Mouse::getPosition());
	}
private:
	void loadBindings();

	StateType currentState;
	Bindings bindings;
	Callbacks callbacks;
	bool hasFocus;
};