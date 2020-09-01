// COMP710 GP 2D Framework 2020
#include "Playership.h"
#include "Enemy.h"
#include <list>
#include "Bullet.h"
#include <vector>
#include "Explosion.h"
#ifndef __GAME_H__
#define __GAME_H__

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();
	static float width;
	static float height;

	void MoveSpaceShipLeft();
	void MoveSpaceShipRight();
	void generateEnemy();
	void FireSpaceShipBullet();
	void SpawnEnemy(float x, float y);
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	__int64 m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
	Playership* m_Playership;
	Sprite* pPlayerSprite;

	Sprite* pEnemySprite;
	Enemy* m_Enemy;

	AnimatedSprite* pExplosion;
	Explosion* m_Explosion;

	std::vector<Enemy*> enemyList;
	std::vector<Bullet*> bulletList;
	std::vector<Explosion*> explosionList;

	Sprite* pBullet;
	Bullet* m_Bullet;

	// Game Entities:

	// SS04.4: Add a PlayerShip field. 

	// SS04.5: Add an alien enemy container field.

	// SS04.6: Add a bullet container field.

private:




	void generateExplosion(int x, int y);

};

#endif // __GAME_H__
