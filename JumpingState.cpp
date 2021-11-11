#include "FallState.h"
#include "JumpingState.h"
#include "Player.h"

constexpr float MAX_JUMP_TIME = 10.0f;
constexpr int JUMP_HEIGHT = 10;
constexpr int JUMP_DISTANCE = 5;

PlayerState* JumpRightState::HandleInput(Player& player)
{
	m_jumpTime++;
	
	if (!Play::KeyDown(VK_SPACE) || m_jumpTime > MAX_JUMP_TIME)
	{
		return new FallRightState;
	}

	return nullptr;
}

void JumpRightState::StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const
{
	// Set sprite dimensions
	int spriteId = gState.sprites.jumpRight;
	player.SetHeight(Play::GetSpriteHeight(spriteId));
	player.SetWidth(Play::GetSpriteWidth(spriteId));

	Point2f currentPos = player.GetPosition();

	if (player.IsGrounded())
	{
		player.SetVelocity({ 0, player.GetVelocity().y - JUMP_HEIGHT });
	}
	
	if (!player.IsGrounded() && Play::KeyDown(VK_RIGHT))
	{
		player.SetVelocity({ JUMP_DISTANCE, player.GetVelocity().y });
	}

	HandleJumpCollision(player, map);
	HandleCoinPickup(player, gState);
	HandleGemPickup(player, gState);

	player.SetPosition(currentPos + player.GetVelocity());

	if (player.GetPosition().x > LEVEL_WIDTH - player.GetWidth())
	{
		player.SetPosition({ LEVEL_WIDTH - player.GetWidth(), player.GetPosition().y });
	}
	else if (player.GetPosition().x < 0)
	{
		player.SetPosition({ 0, player.GetPosition().y });
	}

	player.SetGrounded(false);
}

void JumpRightState::Enter(Player& player) const
{
	player.SetDrawState(State::STATE_JUMP);
	player.SetCrouching(false);
	Play::PlayAudio("jump");
}

void JumpRightState::HandleJumpCollision(Player& player, const std::vector<GameObject*>& map) const
{
	for (GameObject* p : map)
	{
		if (DetectCollision(&player, p, player.IsCrouching()) == UP && p->IsCollidable())
		{
			player.SetVelocity({ player.GetVelocity().x, player.GetVelocity().y * -1.f });
		}
		else if (DetectCollision(&player, p, player.IsCrouching()) == LEFT || DetectCollision(&player, p, player.IsCrouching()) == RIGHT && p->IsCollidable())
		{
			player.SetVelocity({ player.GetVelocity().x * -0.5f, player.GetVelocity().y });
		}
	}
}

PlayerState* JumpLeftState::HandleInput(Player& player)
{
	m_jumpTime++;

	if (!Play::KeyDown(VK_SPACE) || m_jumpTime > MAX_JUMP_TIME)
	{
		return new FallLeftState;
	}

	return nullptr;
}

void JumpLeftState::StateUpdate(Player& player, const std::vector<GameObject*>& map, GameState& gState) const
{
	// Set sprite dimensions
	int spriteId = gState.sprites.jumpLeft;
	player.SetHeight(Play::GetSpriteHeight(spriteId));
	player.SetWidth(Play::GetSpriteWidth(spriteId));

	Point2f currentPos = player.GetPosition();

	if (player.IsGrounded())
	{
		player.SetVelocity({ 0, player.GetVelocity().y - JUMP_HEIGHT });
	}

	if (!player.IsGrounded() && Play::KeyDown(VK_LEFT))
	{
		player.SetVelocity({ -JUMP_DISTANCE, player.GetVelocity().y });
	}

	HandleJumpCollision(player, map);
	HandleCoinPickup(player, gState);
	HandleGemPickup(player, gState);

	player.SetPosition(currentPos + player.GetVelocity());

	if (player.GetPosition().x > LEVEL_WIDTH - player.GetWidth())
	{
		player.SetPosition({ LEVEL_WIDTH - player.GetWidth(), player.GetPosition().y });
	}
	else if (player.GetPosition().x < 0)
	{
		player.SetPosition({ 0, player.GetPosition().y });
	}

	player.SetGrounded(false);
}

void JumpLeftState::Enter(Player& player) const
{
	player.SetDrawState(State::STATE_JUMP_LEFT);
	player.SetCrouching(false);
	Play::PlayAudio("jump");
}

void JumpLeftState::HandleJumpCollision(Player& player, const std::vector<GameObject*>& map) const
{
	for (GameObject* p : map)
	{
		if (DetectCollision(&player, p, player.IsCrouching()) == UP && p->IsCollidable())
		{
			player.SetVelocity({ player.GetVelocity().x, player.GetVelocity().y * -1.f });
		}
		else if (DetectCollision(&player, p, player.IsCrouching()) == LEFT || DetectCollision(&player, p, player.IsCrouching()) == RIGHT && p->IsCollidable())
		{
			player.SetVelocity({ player.GetVelocity().x * -0.5f, player.GetVelocity().y });
		}
	}
}
