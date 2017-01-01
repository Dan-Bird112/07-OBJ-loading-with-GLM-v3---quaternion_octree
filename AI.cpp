#include "AI.h"

AI::AI() : Car()
{
	aiTickCount = 0;
	crashed = false;
}


AI::~AI()
{
}

void AI::aiTick()
{


	this->tick();
	if (aiTickCount < 2) {
		crashed = true;
	}
	if (crashed) {
		if (aiTickCount < 100) {
			noSteer();
			this->upKey();
		}
		else if (aiTickCount < 135) {
			upKey();
			rightKey();
		}
		else if (aiTickCount < 250) {
			upKey();
			noSteer();
		}
		else if (aiTickCount < 262) {
			upKey();
			rightKey();
		}
		else if (aiTickCount < 420) {
			upKey();
			noSteer();
		}
		else if (aiTickCount < 461) {
			upKey();
			rightKey();
		}
		else if (aiTickCount < 680) {
			upKey();
			noSteer();
		}
		else if (aiTickCount < 716) {
			upKey();
			rightKey();
		}
		else if (aiTickCount < 930) {
			upKey();
			noSteer();
		}
		else if (aiTickCount < 960) {
			upKey();
			rightKey();
		}
		else if (aiTickCount < 1020) {
			upKey();
			noSteer();
		}
		else if (aiTickCount < 1037) {
			upKey();
			rightKey();
		}
		else if (aiTickCount < 1140) {
			upKey();
			noSteer();
		}
		else if (aiTickCount < 1160){
			pos = glm::vec3(3.0, 0.0, 0.0);
			setHeading(0.0);
			noSteer();
			noPedal();
		}
		else {
			aiTickCount = -1;
		}
			
		aiTickCount++;
	}
	else {
		noPedal();
		noSteer();
	}
	
}
