#pragma once
#include <vector>
#include <unordered_map>
#include "SharedContext.h"
#include "BaseState.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"

enum class StateType { Intro = 1, MainMenu, Game, Paused, GameOver, Credits };

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;

using TypeContainer = std::vector<StateType>;

using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager
{
public:
	StateManager(SharedContext* l_shared); // Constructor that gets
	~StateManager();

	void Update(const sf::Time& time); // Standart Update and Draw methods
	void Draw();

	void ProccessRequests();

	SharedContext* GetContext();

	bool HasState(const StateType& type);

	void SwitchTo(const StateType& type); // Changes the current state to said type
	void Remove(const StateType& type); // Removes state from state stack

private:
	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	template <class T>
	void RegisterState(const StateType& type)
	{
		// Maps the type in the map to a function, that returns a pointer to newly allocated memory
		stateFactory[type] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	SharedContext* shared;
	StateContainer states;
	TypeContainer toRemove;
	StateFactory stateFactory;
};

