#include "PowerUp.h"

#define SPEED_BOOST 0
#define PROJECTILE_BOOST 1
#define PROJECTILE_RANDOMIZER 2
#define DASH_BOOST 3

PowerUp::PowerUp()
{
}


PowerUp::~PowerUp()
{
}

PowerUp::PowerUp(std::shared_ptr<Shape> shape, Player player, float currentTime, float xBounds, float zBounds)
{
	float x = player.pos.x;
	float z = player.pos.z;

	while (glm::distance(player.pos, glm::vec3(x, 0.0, z)) <= 10.0)
	{
		x = -xBounds + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * xBounds)));
		z = -zBounds + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * zBounds)));
	}

	pos = glm::vec3(x, 0.0f, z);

	angle = 0.0f;
	speed = 0.0f;
	radius = 1.0f;
	sprite = shape;
	currSysTime = currentTime;
	prevSysTime = currentTime;
	exists = true;

	type = rand() % 4;
}

void PowerUp::update(Player player, float currentTime)
{

	prevSysTime = currSysTime;
	currSysTime = currentTime;
	diffSysTime = currSysTime - prevSysTime;

	curTimeAlive += diffSysTime;
	remTimeAlive = maxTimeAlive - curTimeAlive;

	if (remTimeAlive < 0)
	{
		exists = false;
	}
}

void PowerUp::usePowerUp(Player *player)
{
	if (type == SPEED_BOOST)
	{
		if (speed < 1.8)
		{
			player->normalSpeed += 0.2;
		}
	}
	else if (type == PROJECTILE_BOOST)
	{
		player->coolDownInterval /= 1.5;
	}
	else if (type == PROJECTILE_RANDOMIZER)
	{
		player->autoFire = true;
		player->autoFireInterval /= 1.5;
	}
	else if (type == DASH_BOOST)
	{
		player->dashCoolDownInterval /= 2.0;
	}
}