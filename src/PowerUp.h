#pragma once
#include "Entity.h"
#include "Player.h"

class PowerUp : public Entity
{
public:
	PowerUp();
	PowerUp(std::shared_ptr<Shape> shape, Player player, float currentTime, float xBounds, float yBounds);
	~PowerUp();

	float maxTimeAlive;
	float curTimeAlive;
	float remTimeAlive;

	int type;

	void update(Player player, float currentTime);

	void usePowerUp(Player *player);
	//void move() override;
};

