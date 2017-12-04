#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <glm/gtc/type_ptr.hpp>

/* This will update any of the important values for the player every tick */
void Enemy::update(Player player, float currentTime)
{
	prevSysTime = currSysTime;
	currSysTime = currentTime;
	diffSysTime = currSysTime - prevSysTime;

	dir.x = player.pos.x - pos.x;
	dir.z = -player.pos.z + pos.z;
	if (dir.x != 0.0 || dir.y != 0.0 || dir.z != 0.0)
	{
		dir = normalize(dir);
	}

	move();

	angle = getAngle();

	//if (collision(proj))
	//{
	//	exists = false;
	//}
}

void Enemy::move()
{


	pos.x += dir.x * speed * diffSysTime;
	pos.z -= dir.z * speed * diffSysTime;
}

bool Enemy::collision(Projectile *proj)
{
	if (proj->exists)
	{
		float dist = distance(proj->pos, pos);
		if (dist <= radius + proj->radius)
		{
			proj->exists = false;
			return true;
		}
	}
	return false;
}

/* gets the angle that the player is currently facing */
float Enemy::getAngle()
{
	/* do the pathagorean theorem to find hypotenuse and then law of sines to get angle */

	float h = sqrt(pow(dir.x, 2) + pow(dir.z, 2));
	float agl = asin(dir.z / h);

	/* if the cursor is in the left half of the screen, the angle will be opposite */
	if (dir.x < 0)
	{
		agl = -agl;
	}

	return agl;
}

Enemy::Enemy(std::shared_ptr<Shape> shape, float currentTime, Player player, float speed)
{
	float x = player.pos.x;
	float z = player.pos.z;
	//srand(static_cast <unsigned> (time(0)));
	while (glm::distance(player.pos, glm::vec3(x, 0.0, z)) <= 10.0)
	{
		x = -30.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (60.0)));
		z = -30.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (60.0)));
	}

	pos = glm::vec3(x, 0.0f, z);
	dir = glm::vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	this->speed = speed;
	radius = 1.0f;
	sprite = shape;
	currSysTime = currentTime;
	prevSysTime = currentTime;
	exists = true;
}

Enemy::Enemy()
{
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	dir = glm::vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	speed = 2.5f;
	radius = 1.0f;
	currSysTime = 0.0f;
	prevSysTime = 0.0f;
	exists = false;
}


Enemy::~Enemy()
{
}
