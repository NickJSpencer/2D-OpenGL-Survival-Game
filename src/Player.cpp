#include "Player.h"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#define SPEED_BOOST 0
#define PROJECTILE_BOOST 1
#define PROJECTILE_RANDOMIZER 2

/* This will update any of the important values for the player every tick */
void Player::update(float curX, float curY, float posX, float posZ, int width, int height, float sysTime, float dimX, float dimZ)
{

	pos.x = posX;
	pos.z = posZ;

	dir.x = curX / width - 0.5;
	dir.z = -curY / height + 0.5;

	if (dir.x != 0.0 || dir.y != 0.0 || dir.z != 0.0)
	{
		dir = normalize(dir);
	}

	angle = getAngle();

	prevSysTime = currSysTime;
	currSysTime = sysTime;
	diffSysTime = currSysTime - prevSysTime;

	coolDown -= diffSysTime;
	autoFireCoolDown -= diffSysTime;

	dashDuration += diffSysTime;
	dashCoolDown -= diffSysTime;

	if (dashDuration > dashMaxDuration)
	{
		speed = normalSpeed;
	}
}

/* gets the angle that the player is currently facing */
float Player::getAngle()
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

void Player::shoot(std::vector<Projectile> *projectiles)
{
	if (coolDown > 0)
	{
		return;
	}

	coolDown = coolDownInterval;

	projectiles->push_back(Projectile(sprite, pos, normalize(dir), 30.0, 1.0, currSysTime));
}



/* Initializes the player */
Player::Player(std::shared_ptr<Shape> shape, float sysTime)
{
	pos = vec3(0.0f, 0.0f, 0.0f);
	dir = vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	speed = 1.0f;
	radius = 0.9f;
	coolDown = 0.0f;
	coolDownInterval = 0.50f;
	sprite = shape;
	currSysTime = sysTime;

	autoFire = false;
	autoFireInterval = 1.0f;
	autoFireCoolDown = 1.0f;

	dashCoolDown = 0.0f;
	dashCoolDownInterval = 2.0f;
	dashDuration = 0.0f;
	dashMaxDuration = 0.20f;
	normalSpeed = speed;
}

void Player::dash()
{
	if (dashCoolDown < 0 && speed == normalSpeed)
	{
		speed *= 2.5f;
		dashCoolDown = dashCoolDownInterval;
		dashDuration = 0.0f;
	}
	
}

/* never call this. The player can never be drawn :( */
Player::Player()
{
	pos = vec3(0.0f, 0.0f, 0.0f);
	dir = vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	speed = 1.0f;
	radius = 1.0f;
	coolDown = 0.0f;
	coolDownInterval = 0.5f;
}

Player::~Player()
{
}

