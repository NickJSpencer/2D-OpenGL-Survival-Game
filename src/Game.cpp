#include "Game.h"



Game::Game()
{
}

Game::Game(std::shared_ptr<Shape> shape, float sysTime)
{
	currSysTime = sysTime;
	prevSysTime = sysTime;

	enemyCoolDown = 1.0f;
	enemyCoolDownInterval = 1.0f;
	changeCoolDownIntervalInterval = 1.0f;
	enemySpeed = 2.5f;

	powerUpCoolDownInterval = 10.0f;
	powerUpCoolDown = 1.0f;

	sprite = shape;
}

Game::~Game()
{
}


void Game::generateEnemies(std::vector<Enemy> *enemies, Player player, float sysTime)
{
	prevSysTime = currSysTime;
	currSysTime = sysTime;
	diffSysTime = currSysTime - prevSysTime;

	enemyCoolDown -= diffSysTime;
	changeCoolDownIntervalInterval -= diffSysTime;

	if (enemyCoolDown <= 0.0f)
	{
		enemyCoolDown = enemyCoolDownInterval;

		enemies->push_back(Enemy(sprite, currSysTime, player, enemySpeed));

		enemySpeed += .01f;
	}
	if (changeCoolDownIntervalInterval <= 0)
	{
		enemyCoolDownInterval *= .99;
		changeCoolDownIntervalInterval = 1.0f;
	}
}


void Game::generatePowerUps(std::vector<PowerUp> *powerUp, Player player, float sysTime)
{
	powerUpCoolDown -= diffSysTime;

	if (powerUpCoolDown <= 0.0f)
	{
		powerUpCoolDown = powerUpCoolDownInterval;

		powerUp->push_back(PowerUp(sprite, player, currSysTime, 20.0, 20.0));
	}
}