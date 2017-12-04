#pragma once
#include "Shape.h"
#include "Player.h"
#include "Projectile.h"
#include "Entity.h"
#include <glm/gtc/type_ptr.hpp>
class Enemy : public Entity
{
public:
	Enemy();
	Enemy(std::shared_ptr<Shape> sprite, float currentTime, Player player, float speed);
	~Enemy();

	void update(Player player, float currentTime);
	void move() override;
	//bool collide(Entity entity);

	//void draw(std::shared_ptr<Program> prog);

	//std::shared_ptr<Shape> sprite;

	//glm::vec3 pos;
	//glm::vec3 dir;
	//float angle;
	//float speed;
	//float radius;
	//bool exists;

	//float prevSysTime;
	//float currSysTime;

private:
	bool collision(Projectile *proj);
	float getAngle();

};

