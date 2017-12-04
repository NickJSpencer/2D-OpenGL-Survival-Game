#pragma once
#include "Shape.h"
#include "Entity.h"
#include "Projectile.h"
//#include "PowerUp.h"
#include <glm/gtc/type_ptr.hpp>
class Player : public Entity
{
public:
	Player();
	Player(std::shared_ptr<Shape> sprite, float sysTime);
	~Player();

	void update(float curX, float curY, float posX, float posZ, int width, int height, float sysTime, float dimX, float dimZ);
	//void draw(std::shared_ptr<Program> prog);

	void shoot(std::vector<Projectile> *projectiles);
	void dash();

	int autoFire;
	float autoFireInterval;
	float autoFireCoolDown;


	float coolDown;
	float coolDownInterval;


	float dashCoolDown;
	float dashCoolDownInterval;
	float dashDuration;
	float dashMaxDuration;
	float normalSpeed;

private:
	float getAngle();

};

