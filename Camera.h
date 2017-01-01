#pragma once
#include "Actor.h"

class Camera
{
public:
	Camera();
	~Camera();
	glm::vec3 getPos();
	void setXPos(float x);
	void setYPos(float y);
	void setZPos(float z);
	void setPos(glm::vec3 newPos);
	void setFocusPoint(Actor* point);
	void render();
	void topCam();
	void backCam();
	void dashCam();
private:
	glm::vec3 pos;
	Actor* target;
};

