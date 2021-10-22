#include "DashState.h"
#include "FallState.h"
#include "RunningState.h"
#include "Player.h"

constexpr float MAX_DASH = 10.0f;

PlayerState* DashRightState::HandleInput(Player& player)
{
	m_dashTime++;

	if (m_dashTime > MAX_DASH)
	{
		player.SetDrawState(State::STATE_RUN_RIGHT);
		return new RunRightState;
	}

	if (!player.IsGrounded())
	{
		player.SetDrawState(State::STATE_FALL_RIGHT);
		return new FallRightState;
	}

	return nullptr;
}

void DashRightState::StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const
{
	int spriteId = gState.sprites.dashRight;
	player.SetHeight(Play::GetSpriteHeight(spriteId));
	player.SetWidth(Play::GetSpriteWidth(spriteId));

	int speed = player.GetSpeed() * 2;

	HandleCollision(player, map, speed, RIGHT);
}

PlayerState* DashLeftState::HandleInput(Player& player)
{
	m_dashTime++;

	if (m_dashTime > MAX_DASH)
	{
		player.SetDrawState(State::STATE_RUN_LEFT);
		return new RunLeftState;
	}

	if (!player.IsGrounded())
	{
		player.SetDrawState(State::STATE_FALL_LEFT);
		return new FallLeftState;
	}

	return nullptr;
}

void DashLeftState::StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const
{
	int spriteId = gState.sprites.dashLeft;
	player.SetHeight(Play::GetSpriteHeight(spriteId));
	player.SetWidth(Play::GetSpriteWidth(spriteId));

	int speed = player.GetSpeed() * 2;

	HandleCollision(player, map, speed, LEFT);
}