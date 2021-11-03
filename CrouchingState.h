#pragma once
#include "PlayerState.h"

class Player;

class CrouchRightState : public PlayerState
{
public:
	CrouchRightState() {}

	PlayerState* HandleInput(Player& player) override;
	void StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const override;
	void Enter(Player& player) const override;
};

class CrouchLeftState : public PlayerState
{
public:
	CrouchLeftState() {}

	PlayerState* HandleInput(Player& player) override;
	void StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const override;
	void Enter(Player& player) const override;
};

