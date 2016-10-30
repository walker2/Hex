#pragma once
#include "Window.h"
#include "EventManager.h"

struct SharedContext // Structure to keep around pointers to main window and event manager
{
	SharedContext()
		:wind(nullptr),
		eventManager(nullptr) {}
	Window* wind;
	EventManager* eventManager;
};