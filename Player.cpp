#include "Door.h"
#include "IdleState.h"
#include "Player.h"
#include "PlayerState.h"

constexpr float RESPAWN_DELAY = 0.5f;

Player::Player(Point2f pos, Vector2f gravity, int speed, int lives)
	: GameObject(pos),
	m_speed(speed),
	m_gravity(gravity),
	m_lives(lives)
{
	SetPosition(pos);
	SetVelocity({ 0,0 });
	SetType(OBJ_PLAYER);
	SetDrawOrder(1);
	m_pCurrentState = new IdleState(RIGHT);
}

Player::~Player() {}

void Player::Update(GameState& gState)
{
	// Handle state animation
	if (m_animSpeed > 0)
	{
		m_framePos += m_animSpeed;
		if (m_framePos > 1.0f)
		{
			m_frame++;
			m_framePos -= 1.0f;
		}
	}

	// Handle player lifecycle and state machine
	if (!b_isDead)
	{
		PlayerState* pState = m_pCurrentState->HandleInput(*this); // HandleInput returns a new state if input changes the movement type or nullptr if not

		if (pState != nullptr)
		{
			delete m_pCurrentState; // Delete the current state to avoid memory leak
			m_pCurrentState = pState; // Assign state returned by HandleInput to CurrentState
			
			m_pCurrentState->Enter(*this);
		}

		m_pCurrentState->StateUpdate(*this, gState.s_vMap, gState);
	}
	else if (b_isDead && m_lives > 0)
	{
		HandleLifeLost(gState);
	}

	// Handle new level if the player goes through the level end door
	std::vector<GameObject*> doors;
	GameObject::GetObjectList(GameObject::Type::OBJ_DOOR, doors);

	for (GameObject* door : doors)
	{
		if (DetectCollision(this, door, IsCrouching()))
		{
			Door* d = static_cast<Door*>(door);
			if (Play::KeyPressed(VK_UP) && d->GetState() == Door::State::OPEN)
			{
				HandleNewLevel(gState, this);
			}
		}
	}
	// Set camera target to focus on centre of player
	gState.cameraTarget = Point2f(GetPosition().x + GetWidth() / 2.f, GetPosition().y + GetHeight() / 2.f);
}

void Player::Draw(GameState& gState) const
{
	switch (m_state)
	{
	case State::STATE_IDLE:
		Play::DrawSprite(gState.sprites.idleRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(5.f * gState.time));
		break;
	case State::STATE_IDLE_LEFT:
		Play::DrawSprite(gState.sprites.idleLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(5.f * gState.time));
		break;
	case State::STATE_RUN_RIGHT:
		Play::DrawSprite(gState.sprites.runRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(12.f * gState.time));
		break;
	case State::STATE_RUN_LEFT:
		Play::DrawSprite(gState.sprites.runLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(12.f * gState.time));
		break;
	case State::STATE_JUMP:
		Play::DrawSprite(gState.sprites.jumpRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, 1);
		break;
	case State::STATE_JUMP_LEFT:
		Play::DrawSprite(gState.sprites.jumpLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, 1);
		break;
	case State::STATE_CROUCH:
		Play::DrawSprite(gState.sprites.crouchRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(5.f * gState.time));
		break;
	case State::STATE_CROUCH_LEFT:
		Play::DrawSprite(gState.sprites.crouchLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(5.f * gState.time));
		break;
	case State::STATE_DASH_RIGHT:
		Play::DrawSprite(gState.sprites.dashRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(10.f * gState.time));
		break;
	case State::STATE_DASH_LEFT:
		Play::DrawSprite(gState.sprites.dashLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(10.f * gState.time));
		break;
	case State::STATE_CRAWL_RIGHT:
		Play::DrawSprite(gState.sprites.crawlRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(10.f * gState.time));
		break;
	case State::STATE_CRAWL_LEFT:
		Play::DrawSprite(gState.sprites.crawlLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(10.f * gState.time));
		break;
	case State::STATE_SLIDE_RIGHT:
		Play::DrawSprite(gState.sprites.slideRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(10.f * gState.time));
		break;
	case State::STATE_SLIDE_LEFT:
		Play::DrawSprite(gState.sprites.slideLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, (int)(10.f * gState.time));
		break;
	case State::STATE_FALL_RIGHT:
		Play::DrawSprite(gState.sprites.fallRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, 1);
		break;
	case State::STATE_FALL_LEFT:
		Play::DrawSprite(gState.sprites.fallLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, 1);
		break;
	case State::STATE_SMASH_RIGHT:
		Play::DrawSprite(gState.sprites.groundPoundRight, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, m_frame);
		break;
	case State::STATE_SMASH_LEFT:
		Play::DrawSprite(gState.sprites.groundPoundLeft, { m_pos.x - gState.camera.x, m_pos.y - gState.camera.y }, m_frame);
		break;
	default:
		break;
	}
}

Player* Player::CreatePlayer(Point2f pos, Vector2f gravity, int speed, int lives)
{
	return new Player(pos, gravity, speed, lives);
}

void Player::HandleLifeLost(GameState& gState)
{
	if (gState.time > m_deathTime + RESPAWN_DELAY)
	{
		m_pos = { gState.originalPlayerPos };
		b_isDead = false;
		SetDrawState(State::STATE_IDLE);
		delete m_pCurrentState; // Delete the current state before assigning new state to avoid memory leak
		m_pCurrentState = new IdleState(RIGHT);
		b_onGround = true;
	}
}

void Player::HandleNewLevel(GameState& gState, GameObject* player)
{
	Play::PlayAudio("success");
	SetVelocity({ 0, 200 });
	SetPosition(player->GetPosition() + player->GetVelocity());
	ResetCoinCount();
	gState.levelEnd = true;
}