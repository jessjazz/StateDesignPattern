#pragma once
#include "PlayerState.h"

class Player;

class RunRightState : public PlayerState
{
public:
	RunRightState() {}

	PlayerState* HandleInput(Player& player) override;
	void StateUpdate(Player& player, std::vector<GameObject*> map) override;
};

class RunLeftState : public PlayerState
{
public:
	RunLeftState() {}

	PlayerState* HandleInput(Player& player) override;
	void StateUpdate(Player& player, std::vector<GameObject*> map) override;
};