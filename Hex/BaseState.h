#pragma once
#include <SFML/Graphics.hpp>

class StateManager; // Forward declaration for state manager

class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager* _stateManager) : stateManager(_stateManager), transparent(false), transcendent(false) {};
	virtual ~BaseState() {};

	virtual void OnCreate() = 0; // Pure virtual functions
	virtual void OnDestroy() = 0; // Invoked when pushed to the stack

	virtual void Activate() = 0; // Called when state in the top of the stack
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void setTransparent(const bool& _transparent) { transparent = _transparent; };
	bool isTransparent() const { return transparent; };
	void setTranscendent(const bool& _trascendence) { transcendent = _trascendence; };
	bool isTranscendent() const { return transcendent; };
	StateManager* getStateManager() { return stateManager; };

protected:
	StateManager* stateManager;
	bool transparent;
	bool transcendent;
};