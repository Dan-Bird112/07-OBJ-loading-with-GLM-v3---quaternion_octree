#pragma once
#include "Actor.h"
#define LEFT_WHEEL 0
#define RIGHT_WHEEL 1
#define LEFT_WHEEL_X_INC 0.9
#define LEFT_WHEEL_Y_INC 0
#define LEFT_WHEEL_Z_INC 1.6
#define RIGHT_WHEEL_X_INC -0.9
#define RIGHT_WHEEL_Y_INC 0
#define RIGHT_WHEEL_Z_INC 1.6

class Car : public Actor
{
public:
	Car();
	~Car();
	void leftKey();
	void upKey();
	void rightKey();
	void downKey();
	void tick();
	bool worldCollide();
	void noSteer();
	void brake();
	void noPedal();
protected:
	glm::vec4 forVec;
	float steerAngle;
	float vel;
	Actor* wheels[2];
	bool crashed;
};

