#pragma once
#include "PlayerState.h"

class Player;

class CrawlState : public PlayerState
{
public:
	CrawlState(int direction)
		: m_direction(direction)
	{}
	// Change states based on used input or certain player conditions
	PlayerState* HandleInput(Player& player) override;
	// Update position, handle collision and pickups
	void StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const override;
	// Set sprite and any state specific data
	void Enter(Player& player) const override;

protected:
	int m_direction;
};

