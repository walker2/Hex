#include "StateManager.h"




StateManager::StateManager(SharedContext * _shared)
	: shared(_shared)
{
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	//RegisterState<State_Paused>(StateType::Paused);
}

StateManager::~StateManager()
{
	for (auto &itr : states)
	{
		itr.second->OnDestroy();
		delete itr.second;
	}
}


void StateManager::Update(const sf::Time & time)
{
	if (states.empty()) // Check if container has at least 1 state
		return;
	if (states.back().second->isTranscendent() && states.size() > 1) 
	// Check transcendency to determine if the top state allows others method .Update()
	{
		auto itr = states.end();
		while (itr != states.begin())
		{
			if (itr != states.end())
			{
				if (!itr->second->isTranscendent())
					break;
			}
			--itr;
		}
		for (; itr != states.end(); ++itr)
		{
			itr->second->Update(time);
		}
	} 
	else
	{
		states.back().second->Update(time);
	}
}

void StateManager::Draw()
{
	if (states.empty()) // Check if container has at least 1 state
		return;
	if (states.back().second->isTransparent() && states.size() > 1)
		// Check transparency
	{
		auto itr = states.end(); // Iterate through the stack backwards to properly render states at top of each other
		while (itr != states.begin())
		{
			if (itr != states.end())
			{
				if (!itr->second->isTransparent())
					break;
			}
			--itr;
		}
		for (; itr != states.end(); ++itr)
		{
			itr->second->Draw();
		}
	}
	else
	{
		states.back().second->Draw();
	}
}

void StateManager::ProccessRequests()
{
	while (toRemove.begin() != toRemove.end())
	{
		RemoveState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

SharedContext * StateManager::GetContext()
{
	return shared;
}

bool StateManager::HasState(const StateType & type)
{
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == type) {
			auto removed = std::find(toRemove.begin(), toRemove.end(), type);
			if (removed == toRemove.end()) 
			 return true; 

			return false;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType & type)
{
	shared->eventManager->SetCurrentState(type);
	for (auto itr = states.begin(); itr != states.end(); ++itr)
	{
		if (itr->first == type)
		{
			states.back().second->Deactivate();
			StateType tmp_type = itr->first; // Save the info before calling .erase()
			BaseState* tmp_state = itr->second;
			states.erase(itr); // Iterators invalidated but we don't need them any more
			states.emplace_back(tmp_type, tmp_state);
			tmp_state->Activate(); 
			return;
		}
	}

	if (!states.empty()) { states.back().second->Deactivate(); }
	CreateState(type);
	states.back().second->Activate();
}

void StateManager::Remove(const StateType & type)
{
	toRemove.push_back(type); // Pushes type to vector for later removal 
}

void StateManager::CreateState(const StateType & type)
{
	auto newState = stateFactory.find(type);
	if (newState == stateFactory.end()) // Check if we can create thath type
		return;
	BaseState* state = newState->second(); // Invoking second element of unordered_map which is std::funciton and returns pointer to new created state
	states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType & type)
{
	for (auto itr = states.begin(); itr != states.end(); ++itr)
	{
		if (itr->first == type)
		{
			itr->second->OnDestroy();
			delete itr->second;
			states.erase(itr);
			return; 
		}
	}
}
