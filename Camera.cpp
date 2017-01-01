#include "Camera.h"


Camera::Camera()
{
	pos = glm::vec3(0, 0, 0);
	
}


Camera::~Camera()
{
}

glm::vec3 Camera::getPos()
{
	return this->pos;
}

void Camera::setXPos(float x)
{
	this->pos.x = x;
}

void Camera::setYPos(float y)
{
	this->pos.y = y;
}

void Camera::setZPos(float z)
{
	this->pos.z = z;
}

void Camera::setPos(glm::vec3 newPos)
{
	this->pos = newPos;
}

void Camera::setFocusPoint(Actor* point)
{
	this->target = point;
}

void Camera::render()
{
	glUniform1f(glGetUniformLocation(shader->handle(), "displacement"), 0);
	GLuint matLocation = glGetUniformLocation(shader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);
}

void Camera::topCam()
{
	this->setPos(glm::vec3(target->getPos().x, target->getPos().y + 20, target->getPos().z - 1));
	viewingMatrix = glm::lookAt(pos, target->getPos(), glm::vec3(0, 1, 0));

	viewingMatrix = glm::translate(viewingMatrix, target->getPos());						//move axis to target
	viewingMatrix = glm::rotate(viewingMatrix, -target->getHeading(), glm::vec3(0, 1, 0));  //rotate around target
	viewingMatrix = glm::translate(viewingMatrix, -target->getPos());						//move axis back
}

void Camera::backCam()
{
	this->setPos(glm::vec3(target->getPos().x, target->getPos().y + 3, target->getPos().z - 5));

	glm::vec3 point = glm::vec3(target->getPos().x, target->getPos().y + 0.5, target->getPos().z);

	viewingMatrix = glm::lookAt(pos, point, glm::vec3(0, 1, 0));

	viewingMatrix = glm::translate(viewingMatrix, target->getPos());						//move axis to target
	viewingMatrix = glm::rotate(viewingMatrix, -target->getHeading(), glm::vec3(0, 1, 0));  //rotate around target
	viewingMatrix = glm::translate(viewingMatrix, -target->getPos());						//move axis back
}

void Camera::dashCam()
{
	this->setPos(glm::vec3(target->getPos().x + 0.35 /*10.35*/, target->getPos().y + 1.3, target->getPos().z - 0.5));
	
	glm::vec3 point = glm::vec3(target->getPos().x + 0.3, target->getPos().y + 1.3, target->getPos().z + 0.1);
	
	viewingMatrix = glm::lookAt(pos, point, glm::vec3(0, 1, 0));


	viewingMatrix = glm::translate(viewingMatrix, target->getPos());						//move axis to target
	viewingMatrix = glm::rotate(viewingMatrix, -target->getHeading(), glm::vec3(0, 1, 0));  //rotate around target
	viewingMatrix = glm::translate(viewingMatrix, -target->getPos());						//move axis back
}
