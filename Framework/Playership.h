#pragma once
#ifndef __PLAYER_SHIP_H__
#define __PLAYER_SHIP_H__

#include "entity.h"

class Playership : public Entity {
public:
	Playership();
	~Playership();

	void Process(float deltaTime);


};
#endif // !__PLAYER_SHIP_H__
