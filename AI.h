#pragma once
#include "Car.h"
class AI : public Car
{
public:
	AI();
	AI(Car * player);
	~AI();
	void aiTick();
private:
	int milestones[10];
	int aiTickCount;
	Car * player;
};

