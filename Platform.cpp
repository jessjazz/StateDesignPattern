#include "Platform.h"

Platform::Platform(Point2f pos, int width, int height)
	: GameObject(pos)
{
	SetPosition(pos);
	SetType(OBJ_PLATFORM);
	SetDrawOrder(2);
	SetHeight(height);
	SetWidth(width);
}

void Platform::Update(GameState& gState)
{
}

void Platform::Draw(GameState& gState) const
{
	Play::DrawRect(m_pos, { m_pos.x + m_width, m_pos.y + m_height }, Play::cGrey, true);
}