#include "EventManager.h"



EventManager::EventManager() : hasFocus(true)
{
	loadBindings(); // Call private method to load all bindings from a file
}


EventManager::~EventManager()
{
	for (auto &itr : bindings)
	{
		delete itr.second; // De-alloate the dynamic memory
		itr.second = nullptr;
	}
}

bool EventManager::addBinding(Binding * binding)
{
	if (bindings.find(binding->name) != bindings.end()) // Checking if binding container already has a binding with the same name
		return false;

	return bindings.emplace(binding->name, binding).second;
}

bool EventManager::removeBinding(std::string name)
{
	auto itr = bindings.find(name);
	if (itr == bindings.end())
		return false;

	delete itr->second;
	bindings.erase(itr);
	return true;
}

void EventManager::setFocus(const bool & focus)
{
	hasFocus = focus;
}

void EventManager::HandleEvent(sf::Event & event)
{
	// Handling all SFML events
	for (auto &b_itr : bindings) // Iterate through all bindings
	{
		Binding* bind = b_itr.second;
		for (auto &e_itr : bind->events) // Iterate through all events
		{
			EventType sfmlEvent = (EventType)event.type;
			if (e_itr.first != sfmlEvent)
				continue;
			
			//Checking if it's keyboard or mouse event, because it involves checking for specific keys

			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp)
			{
				if (e_itr.second.code == event.key.code)
				{
					// Checking if key code matches the code of our binding event
					
					if (bind->details.keyCode != -1)
					{
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c); // Increase count
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp)
			{
				if (e_itr.second.code == event.mouseButton.button)
				{
					// Checking if key code matches the code of our binding event
				
					bind->details.mouse.x = event.mouseButton.x;
					bind->details.mouse.y = event.mouseButton.y;
					if (bind->details.keyCode != -1)
					{
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c); 	// Increase count
					break;
				}
			}
			else
			{
				// No need for additional checking
				if (sfmlEvent == EventType::MouseWheel)
				{
					bind->details.mouseWheelDelta = event.mouseWheel.delta;
				}
				else if (sfmlEvent == EventType::WindowResized)
				{
					bind->details.size.x = event.size.width;
					bind->details.size.y = event.size.height;
				}
				else if(sfmlEvent == EventType::TextEntered)
				{
					bind->details.textEntered = event.text.unicode;
				}
				++(bind->c);
			}
		}
	}
}

void EventManager::Update()
{
	if (!hasFocus)
		return;

	for (auto &b_itr : bindings) // Iterate through all bindings
	{
		Binding *bind = b_itr.second;
		for (auto &e_itr : bind->events) // Iterate through all events
		{
			switch (e_itr.first)
			{
			case (EventType::Keyboard) :
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.code)))
				{
					if (bind->details.keyCode != -1)
					{
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
			break;

			case (EventType::Mouse):
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.code)))
					{
						if (bind->details.keyCode != -1)
						{
							bind->details.keyCode = e_itr.second.code;
						}
						++(bind->c);
					}
			break;

			}

		}

		if (bind->events.size() == bind->c)
		{
			auto callItr = callbacks.find(bind->name);
			if (callItr != callbacks.end())
			{
				callItr->second(&bind->details);
			}
		}
		bind->c = 0;
		bind->details.Clear();
	}
}

void EventManager::loadBindings()
{
	std::string delimiter = ":";

	std::ifstream binds;
	binds.open("keys.cfg"); // Open file with key configuration
	if (!binds.is_open())
	{
		std::cout << "FAILED TO LOAD KEYS.CFG" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(binds, line)) // Read through every line of the file
	{
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		Binding* bind = new Binding(callbackName); // Allocate new binding on the heap
		while (!keystream.eof()) // Loop through the line to the end of it
		{
			std::string keyval;
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos)
			{
				delete bind;
				bind = nullptr;
				break;
			}
			// Converting from string to integer values
			EventType type = EventType(stoi(keyval.substr(start, end - start))); 
			int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			EventInfo eventInfo;
			eventInfo.code = code;

			bind->BindEvent(type, eventInfo);
		}

		if (!addBinding(bind))
			delete bind;
		bind = nullptr;
	}
	binds.close(); // Close the file
}
