#pragma once
#include "Enemy.h"
#include "Player.h"
#include "PowerUp.h"
class Game
{
public:
	Game();
	Game(std::shared_ptr<Shape> shape, float sysTime);
	~Game();

	float currSysTime;
	float prevSysTime;
	float diffSysTime;

	float enemyCoolDown;
	float enemyCoolDownInterval;
	float enemySpeed;

	float changeCoolDownIntervalInterval;

	float powerUpCoolDown;
	float powerUpCoolDownInterval;

	std::shared_ptr<Shape> sprite;

	void generateEnemies(std::vector<Enemy> *enemies, Player player, float sysTime);
	void generatePowerUps(std::vector<PowerUp> *powerUp, Player player, float sysTime);

};

