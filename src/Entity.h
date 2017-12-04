#pragma once
#include "Shape.h"
//#include "Player.h"
//#include "Projectile.h"
//#include "Enemy.h"
#include <glm/gtc/type_ptr.hpp>

class Entity
{
public:
	Entity();
	//Entity(std::shared_ptr<Shape> sprite, float currentTime);
	~Entity();

	//static void update(Player *player, std::vector<Enemy> *enemies, std::vector<Projectile> *projectiles,  float currentTime);
	virtual void draw(std::shared_ptr<Program> prog);
	virtual void move();
	virtual bool canDelete();
	bool collide(Entity entity);

	std::shared_ptr<Shape> sprite;

	glm::vec3 pos;
	glm::vec3 dir;
	float angle;
	float speed;
	float radius;
	bool exists;

	float prevSysTime;
	float currSysTime;
	float diffSysTime;
};

