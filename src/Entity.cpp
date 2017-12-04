#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

bool Entity::canDelete()
{
	return true;
}

/* Draws the entity to the screen based on the stored sprite */
void Entity::draw(std::shared_ptr<Program> prog)
{
	sprite->draw(prog);
}

void Entity::move()
{
	return;
}

bool Entity::collide(Entity entity)
{
	float dist = distance(entity.pos, pos);
	if (dist <= radius + entity.radius)
	{
		return true;
	}
	return false;
}