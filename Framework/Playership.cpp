#include <iostream>;
#include "playership.h";
#include "game.h"

Playership::Playership()
{

}

Playership::~Playership()
{

}
void 
Playership::Process(float deltaTime) {
	// SS04.4: Generic position update, based upon velocity (and time).
	this->SetPositionX(this->m_velocityX * deltaTime + this->GetPositionX());
	this->SetPositionY(this->m_velocityY * deltaTime + this->GetPositionY());

	if (this->GetPositionX() >= Game::width - 29)
	{
		this->SetHorizontalVelocity(0);
	}
	else if (this->GetPositionX() <= 0)
	{
		this->SetHorizontalVelocity(0);

	}

	if (this->GetPositionY() >= Game::height)
	{
		this->SetVerticalVelocity(0);
	}
	else if (this->GetPositionY() <= 0)
	{
		this->SetVerticalVelocity(0);

	}
}




