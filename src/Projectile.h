#pragma once
#include "Shape.h"
#include "Entity.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projectile : public Entity
{
public:
	Projectile(std::shared_ptr<Shape> shape, glm::vec3 pos, glm::vec3 dir, float speed, float timeAlive, float currentTime);
	Projectile();
	~Projectile();

	bool exists;

	//float speed;
	float maxTimeAlive;
	float curTimeAlive;
	float remTimeAlive;

	void update(float currentTime);
	//void draw(std::shared_ptr<Program> prog);
};

