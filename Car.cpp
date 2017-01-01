#include "Car.h"


Car::Car() : Actor("TestModels/car2.obj")
{
	heading = 0.0;
	forVec = glm::vec4(0, 0, 1, 1);
	steerAngle = 0;
	vel = 0;
	wheels[LEFT_WHEEL] = new Actor("TestModels/tyre.obj");
	wheels[RIGHT_WHEEL] = new Actor("TestModels/tyre.obj");

	wheels[RIGHT_WHEEL]->setXPos(RIGHT_WHEEL_X_INC);
	wheels[RIGHT_WHEEL]->setYPos(RIGHT_WHEEL_Y_INC);
	wheels[RIGHT_WHEEL]->setZPos(RIGHT_WHEEL_Z_INC);

	wheels[LEFT_WHEEL]->setXPos(LEFT_WHEEL_X_INC);
	wheels[LEFT_WHEEL]->setYPos(LEFT_WHEEL_Y_INC);
	wheels[LEFT_WHEEL]->setZPos(LEFT_WHEEL_Z_INC);
	crashed = false;
}

Car::~Car()
{
}

void Car::leftKey()
{
	if(steerAngle < 2)
		steerAngle += 0.25;
	else 
		steerAngle = 2;
}

void Car::upKey()
{
	if (vel < 0.25)
		vel += 0.025;

}

void Car::rightKey()
{
	if (steerAngle > -2)
		steerAngle -= 0.25;
	else 
		steerAngle = -2;
}

void Car::downKey()
{
	if (vel > -0.25)
		vel -= 0.025;
}

void Car::tick()
{
	wheels[RIGHT_WHEEL]->setXPos(RIGHT_WHEEL_X_INC);
	wheels[RIGHT_WHEEL]->setYPos(RIGHT_WHEEL_Y_INC);
	wheels[RIGHT_WHEEL]->setZPos(RIGHT_WHEEL_Z_INC);

	wheels[LEFT_WHEEL]->setXPos(LEFT_WHEEL_X_INC);
	wheels[LEFT_WHEEL]->setYPos(LEFT_WHEEL_Y_INC);
	wheels[LEFT_WHEEL]->setZPos(LEFT_WHEEL_Z_INC);

	wheels[LEFT_WHEEL]->setPos(glm::vec3(transformations*glm::vec4(wheels[LEFT_WHEEL]->getPos().x, wheels[LEFT_WHEEL]->getPos().y, wheels[LEFT_WHEEL]->getPos().z,1.0)));

	wheels[RIGHT_WHEEL]->setPos(glm::vec3(transformations*glm::vec4(wheels[RIGHT_WHEEL]->getPos().x, wheels[RIGHT_WHEEL]->getPos().y, wheels[RIGHT_WHEEL]->getPos().z, 1.0)));

	if (vel >= 0) {
		wheels[LEFT_WHEEL]->setHeading(heading + (steerAngle*22.5));
		wheels[RIGHT_WHEEL]->setHeading(heading + (steerAngle*22.5) + 180);
	}
	else {
		wheels[LEFT_WHEEL]->setHeading(heading + (steerAngle*-22.5));
		wheels[RIGHT_WHEEL]->setHeading(heading + (steerAngle*-22.5) + 180);
	}

	wheels[LEFT_WHEEL]->render();
	wheels[RIGHT_WHEEL]->render();

	steerAngle = steerAngle * std::abs(vel / 0.25);
	this->setHeading(heading + (steerAngle /** 0.02f*/));
	glm::mat4 rotMat = glm::rotate(glm::mat4(1), heading,glm::vec3(0,1,0));
	pos = pos + (glm::vec3((rotMat * forVec) * vel) /** 0.025f*/);


	
	for (int i = 0; i < collList.size(); i++) {
		if (this != collList[i] && this->collidesWith(collList[i]) ) {
			pos = pos - (glm::vec3((rotMat * forVec) * vel)  * 2.0f /** 0.025f*/);
			vel = -vel * 1.3;
			steerAngle = 0;
			this->crashed = false;
			break;
		}
	}

	wheels[LEFT_WHEEL]->render();
	wheels[RIGHT_WHEEL]->render();


}

void Car::noSteer()
{
	if (steerAngle < 0) {
		steerAngle += std::abs(steerAngle/6);
	}
	else if (steerAngle > 0) {
		steerAngle -= steerAngle/6;
	}
}

void Car::brake()
{
	if (vel > 0) {
		vel -= vel/6;
	}
	if (vel < 0) {
		vel += std::abs(vel / 6);
	}
	if (vel == 0) {
		vel = 0;
	}
}

void Car::noPedal()
{
	if (vel > 0) {
		vel -= vel/6;
	}
	if (vel < 0) {
		vel += std::abs(vel/6);
	}
	if (vel == 0) {
		vel = 0;
	}
}

