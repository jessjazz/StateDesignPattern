#pragma once
#include "GameObject.h"

struct PlatformArgs;

class Platform : public GameObject
{
public:
	Platform(Point2f pos, int width, int height);
	~Platform() {}

	void Update(GameState& gState) override;
	void Draw(GameState& gState) const override;
};

