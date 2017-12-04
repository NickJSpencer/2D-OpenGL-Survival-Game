#include "Projectile.h"

/* This will update any of the important values for the projectile every tick */
void Projectile::update(float currentTime)
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
	else
	{
		pos.x += dir.x * speed * diffSysTime;
		pos.z -= dir.z * speed * diffSysTime;
	}
}

Projectile::Projectile(std::shared_ptr<Shape> shape, glm::vec3 pos, glm::vec3 dir, float speed, float timeAlive, float currentTime)
{
	this->pos = pos;
	this->dir = dir;
	this->speed = speed;
	maxTimeAlive = timeAlive;
	curTimeAlive = 0.0f;
	remTimeAlive = timeAlive;
	sprite = shape;
	prevSysTime = currentTime;
	currSysTime = currentTime;
	exists = true;
	radius = 0.25f;
}

Projectile::Projectile()
{
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	dir = glm::vec3(0.0f, 0.0f, 0.0f);
	speed = 0.0f;
	maxTimeAlive = 0.0f;
	curTimeAlive = 0.0f;
	remTimeAlive = 0.0f;
	sprite = NULL;
	exists = false;
	radius = 0.25f;
}

Projectile::~Projectile()
{
}
