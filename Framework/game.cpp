// COMP710 GP 2D Framework 2020

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>

// Static Members:
Game* Game::sm_pInstance = 0;
float Game::width = 800.0f;
float Game::height = 600.0f;
Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
	: m_pBackBuffer(0)
	, m_pInputHandler(0)
	, m_looping(true)
	, m_executionTime(0)
	, m_elapsedSeconds(0)
	, m_frameCount(0)
	, m_FPS(0)
	, m_numUpdates(0)
	, m_lastTime(0)
	, m_lag(0)
	, m_Playership()
	, pPlayerSprite()
	, m_Enemy()
	, pEnemySprite()
{

}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool
Game::Initialise()
{

	m_pBackBuffer = new BackBuffer();

	if (!m_pBackBuffer->Initialise(this->width, this->height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0xCC, 0xCC, 0xCC);

	// SS04.4: Load the player ship sprite.
	// For example: Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	// But pPlayerSprite should be a member of the Game class...
	// ...not just a local variable of this function!
	m_Playership = new Playership();
	pPlayerSprite = new Sprite();

	// SS04.4: Create the player ship instance.
	pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	m_Playership->Initialise(pPlayerSprite);
	m_Playership->SetPositionX(400);
	m_Playership->SetPositionY(500);

	// SS04.5: Spawn four rows of 14 alien enemies.
	generateEnemy();


	// SS04.6: Fill the container with these new enemies.
	pBullet = new Sprite();
	pBullet = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");

	pExplosion = new AnimatedSprite();
	pExplosion = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");

	m_pBackBuffer->clearSprite();
	return (true);
}

bool
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (m_looping)
	{
		Uint64 current = SDL_GetPerformanceCounter();

		float deltaTime = (current - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());

		m_lastTime = current;

		m_executionTime += deltaTime;

		Process(deltaTime);

		m_lag += deltaTime;

		int innerLag = 0;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
			++innerLag;
		}


		m_pInputHandler->~InputHandler();
		Draw(*m_pBackBuffer);
	}

	return (m_looping);
}

void
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:

	// SS04.5: Process each alien enemy in the container.
	for (Enemy* enemy : enemyList)
	{
		enemy->Process(deltaTime);
	}
	// SS04.6: Process each bullet in the container.
	for (Bullet* bullet : bulletList)
	{
		bullet->Process(deltaTime);
	}

	// SS04.4: Update player...
	m_Playership->Process(deltaTime);


	int collideX, collideY;
	
	// SS04.6: Check for bullet vs alien enemy collisions...
	// SS04.6: For each bullet
	// SS04.6: For each alien enemy
	// SS04.6: Check collision between two entities.
	for (Bullet* bullet : bulletList)
	{
		bool isCollide = false;
		for (Enemy* enemy : enemyList)
		{
			isCollide = bullet->IsCollidingWith(*enemy);
			if (isCollide) {
				generateExplosion(bullet->GetPositionX(), bullet->GetPositionY());
				bulletList.erase(std::find(bulletList.begin(), bulletList.end() - 1, bullet));
				enemyList.erase(std::find(enemyList.begin(), enemyList.end() - 1, enemy));
				bullet->~Bullet();
				enemy->~Enemy();

						//char buffer[64];
						//sprintf(buffer, "bullet position x: %f", bullet->GetPositionX());
						//LogManager::GetInstance().Log(buffer);

						// buffer[64];
						//sprintf(buffer, "bullet position y: %f", bullet->GetPositionY());
						//LogManager::GetInstance().Log(buffer);

						// buffer[64];
						//sprintf(buffer, "enemy position x: %f", enemy->GetPositionX());
						//LogManager::GetInstance().Log(buffer);

						//buffer[64];
						//sprintf(buffer, "enemy position y: %f", enemy->GetPositionY());
						//LogManager::GetInstance().Log(buffer);	

				continue;

			}

			//char buffer[64];
			//sprintf(buffer, "%p", m_Enemy);
			//LogManager::GetInstance().Log(buffer);
		}
		//char buffer[64];
		//sprintf(buffer, "bulletList size: %d", bulletList.size());
		//LogManager::GetInstance().Log(buffer);

		if (bullet->GetPositionY() < 0)
		{
			//char buffer[64];
			//sprintf(buffer, "bullet position of height: %f", bullet->GetPositionY());
			//LogManager::GetInstance().Log(buffer);
			bulletList.erase(std::find(bulletList.begin(), bulletList.end() - 1, bullet));
		}
	}


	for (Explosion* explosion : explosionList)
	{
		explosion->Process(deltaTime);
		if (explosion->IsPaused())
		{
			explosion->~Explosion();
		}
	}
	// SS04.6: If collided, destory both and spawn explosion.

	// SS04.6: Remove any dead bullets from the container...

	// SS04.6: Remove any dead enemy aliens from the container...

	// SS04.6: Remove any dead explosions from the container...

}

void
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// SS04.5: Draw all enemy aliens in container...
	for (Enemy* enemy : enemyList)
	{
		enemy->Draw(backBuffer);
	}


	// SS04.6: Draw all bullets in container...
	for (Bullet* bullet : bulletList)
	{
		bullet->Draw(backBuffer);
	}
	// SS04.4: Draw the player ship...
	m_Playership->Draw(backBuffer);


	for (Explosion* explosion : explosionList)
	{
		explosion->Draw(backBuffer);
	}


	//m_pBackBuffer->SetTextColour(255, 0, 0);
	//m_pBackBuffer->DrawText(100, 100, "hello123123213");
	backBuffer.Present();
}

void
Game::Quit()
{
	m_looping = false;
}

void
Game::MoveSpaceShipLeft()
{
	m_Playership->SetHorizontalVelocity(-100);
}
void
Game::MoveSpaceShipRight()
{
	m_Playership->SetHorizontalVelocity(+100);
}

void
Game::generateEnemy()
{
	float xb = 40;
	float yb = 30;
	float width = 50;
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 15; c++)
		{
			SpawnEnemy(xb, yb);
			xb += width;
		}
		xb = 40;
		yb += width;
	}
}
// SS04.3: Add the method to tell the player ship to move right...

// SS04.6: Space fires a Bullet in game.
void
Game::FireSpaceShipBullet()
{
	// SS04.6: Load the player bullet sprite.   

	// SS04.6: Create a new bullet object.
	m_Bullet = new Bullet();
	m_Bullet->Initialise(pBullet);
	m_Bullet->SetPositionX(this->m_Playership->GetPositionX());
	m_Bullet->SetPositionY(this->m_Playership->GetPositionY());

	// SS04.6: Set the bullets vertical velocity.
	m_Bullet->SetVerticalVelocity(400.0f);
	// SS04.6: Add the new bullet to the bullet container.
	bulletList.push_back(m_Bullet);
}
void::
Game::generateExplosion(int x, int y) {
	m_Explosion = new Explosion();
	m_Explosion->Initialise(*pExplosion->GetTexture());
	m_Explosion->SetFrameSpeed(0.1f);
	m_Explosion->SetLooping(false);
	m_Explosion->SetX(x);
	m_Explosion->SetY(y);
	explosionList.push_back(m_Explosion);

}


// SS04.5: Spawn a Enemy in game.
void
Game::SpawnEnemy(float x, float y)
{
	// SS04.5: Load the alien enemy sprite file.
	pEnemySprite = new Sprite();

	pEnemySprite = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");

	// SS04.5: Create a new Enemy object. 
	m_Enemy = new Enemy();
	m_Enemy->Initialise(pEnemySprite);
	m_Enemy->SetPositionX(x);
	m_Enemy->SetPositionY(y);
	// SS04.5: Add the new Enemy to the enemy container.

	//char buffer[64];
	//sprintf(buffer, "%p", m_Enemy);
	//LogManager::GetInstance().Log(buffer);
	enemyList.push_back(m_Enemy);
}
