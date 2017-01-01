#include "Actor.h"


Actor::Actor() {
	heading = 0.0;
}


Actor::Actor(char* fileName) {
	this->init(fileName);
	heading = 0.0;
	pos = glm::vec3(0.0f,0.0f,0.0f);
}


void Actor::render() {
	//DRAW THE MODEL
	transformations = glm::translate(glm::mat4(1.0), pos);
	transformations = glm::rotate(transformations, heading, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = viewingMatrix * transformations;
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(shader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	model.drawElementsUsingVBO(shader);
}

glm::vec3 Actor::getPos() {
	return this->pos;
}

void Actor::setPos(glm::vec3 in)
{
	this->pos = in;
}

void Actor::setXPos(float x) {
	this->pos.x = x;
}

void Actor::setYPos(float y) {
	this->pos.y = y;
}

void Actor::setZPos(float z) {
	this->pos.z = z;
}

glm::vec3 Actor::getScale() {
	return this->scale;
}

void Actor::setXScale(float x) {
	this->pos.x = x;
}

void Actor::setYScale(float y) {
	this->pos.y = y;
}

void Actor::setZScale(float z) {
	this->pos.z = z;
}


//Update the heading and update the normals
void Actor::setHeading(float in)
{
	this->heading = in;
}

float Actor::getHeading()
{
	return this->heading;
}


void Actor::init(char* filename) {
	OBJLoader objLoader;
	cout << " loading model " << endl;
	if (objLoader.loadModel(filename, model))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;
		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();


		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.

											 //turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.initDrawElements();
		model.initVBO(shader);
		model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
}

void Actor::renderFrame()
{
	model.drawOctreeLeaves(shader);
}

void Actor::drawBoundBox()
{
	this->model.drawBoundingBox(shader);
}

bool Actor::collidesWith(Actor *target)
{
	Octree *aCurrent = this->model.getOctree();
	Octree *bCurrent = target->model.getOctree();

	//return octreeColl(aCurrent, bCurrent, target);

	if (octreeColl(aCurrent, bCurrent, target)) {
		for (int i = 0;i < 8; i++){
			if (!aCurrent->getChild(i))
				continue;
			for (int j = 0; j < 8; j++) {
				if (!bCurrent->getChild(j) || !octreeColl(aCurrent->getChild(i),bCurrent->getChild(j),target))
					continue;
				for (int k = 0;k < 8; k++) {
					if (!aCurrent->getChild(i)->getChild(k))
						continue;
					for (int l = 0; l < 8; l++) {
						if (!bCurrent->getChild(j)->getChild(l) || !octreeColl(aCurrent->getChild(i)->getChild(k), bCurrent->getChild(j)->getChild(l), target))
							continue;
						for (int m = 0;m < 8; m++) {
							if (!aCurrent->getChild(i)->getChild(k)->getChild(m))
								continue;
							for (int n = 0; n < 8; n++) {
									if (!bCurrent->getChild(j)->getChild(l)->getChild(n) || !octreeColl(aCurrent->getChild(i)->getChild(k)->getChild(m), bCurrent->getChild(j)->getChild(l)->getChild(n), target))
									continue;
									for (int o = 0; o < 8; o++) {
										if (!aCurrent->getChild(i)->getChild(k)->getChild(m)->getChild(o))
											continue;
										for (int p = 0; p < 8; p++) {
											if (!bCurrent->getChild(j)->getChild(l)->getChild(n)->getChild(p))
												continue;
											if (octreeColl(aCurrent->getChild(i)->getChild(k)->getChild(m)->getChild(o), bCurrent->getChild(j)->getChild(l)->getChild(n)->getChild(p), target)) {
												return true;
											}
										}
									}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool Actor::axisTest(glm::vec3 aVert[], glm::vec3 bVert[], glm::vec3 axis)
{
	//Handles zero case
	if (axis == glm::vec3(0.0f, 0.0f, 0.0f))
		return true;

	//We do this to ensure the first set tested are seeded
	float aMin =  std::numeric_limits<float>::max();
	float aMax = -std::numeric_limits<float>::max();
	float bMin =  std::numeric_limits<float>::max();
	float bMax = -std::numeric_limits<float>::max();

	//Find the min and Max values for each box
	for (int i = 0; i < 8; i++) {
		float aDist = glm::dot(aVert[i], axis);
		aMin = (aDist < aMin) ? aDist : aMin;
		aMax = (aDist > aMax) ? aDist : aMax;
		float bDist = glm::dot(bVert[i], axis);
		bMin = (bDist < bMin) ? bDist : bMin;
		bMax = (bDist > bMax) ? bDist : bMax;
	}

	//intersection test between a and b
	return !((aMax < bMin) || (bMax < aMin));
}

glm::vec3 Actor::getNormal(int i)
{
	return this->normals[i];
}

void Actor::setNormal(int i, glm::vec3 in)
{
	normals[i] = in;
}

glm::mat4 Actor::getTranssform()
{
	return this->transformations;
}

bool Actor::boxCollision(glm::vec3 aVert[8], glm::vec3 bVert[8], Actor *target)
{

	normals[FRONT_NORMAL] = aVert[1] - aVert[0];
	normals[LEFT_NORMAL] = aVert[4] - aVert[0];
	normals[TOP_NORMAL] = aVert[2] - aVert[0];

	target->setNormal(FRONT_NORMAL, bVert[1] - bVert[0]);
	target->setNormal(LEFT_NORMAL, bVert[4] - bVert[0]);
	target->setNormal(TOP_NORMAL, bVert[2] - bVert[0]);


	if (!axisTest(aVert, bVert,normals[FRONT_NORMAL]) ||
		!axisTest(aVert, bVert,normals[LEFT_NORMAL]) ||
		!axisTest(aVert, bVert,normals[TOP_NORMAL])  ||
		!axisTest(aVert, bVert,target->getNormal(FRONT_NORMAL)) ||
		!axisTest(aVert, bVert,target->getNormal(LEFT_NORMAL) )||
		!axisTest(aVert, bVert,target->getNormal(TOP_NORMAL)))
		return false;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (!axisTest(aVert, bVert, glm::cross(normals[i], target->getNormal(j)))) {
				return false;
			}
		}
	}

	return true;
}

bool Actor::octreeColl(Octree * aCurrent, Octree * bCurrent, Actor * target)
{

	if (!aCurrent || !bCurrent) {
		return false;
	}

	glm::vec3 aVert[8], bVert[8];

	aVert[0] = glm::vec3(aCurrent->getMaxX(), aCurrent->getMaxY(), aCurrent->getMaxZ());
	aVert[1] = glm::vec3(aCurrent->getMaxX(), aCurrent->getMaxY(), aCurrent->getMinZ());

	aVert[2] = glm::vec3(aCurrent->getMaxX(), aCurrent->getMinY(), aCurrent->getMaxZ());
	aVert[3] = glm::vec3(aCurrent->getMaxX(), aCurrent->getMinY(), aCurrent->getMinZ());

	aVert[4] = glm::vec3(aCurrent->getMinX(), aCurrent->getMaxY(), aCurrent->getMaxZ());
	aVert[5] = glm::vec3(aCurrent->getMinX(), aCurrent->getMaxY(), aCurrent->getMinZ());

	aVert[6] = glm::vec3(aCurrent->getMinX(), aCurrent->getMinY(), aCurrent->getMaxZ());
	aVert[7] = glm::vec3(aCurrent->getMinX(), aCurrent->getMinY(), aCurrent->getMinZ());

	bVert[0] = glm::vec3(bCurrent->getMaxX(), bCurrent->getMaxY(), bCurrent->getMaxZ());
	bVert[1] = glm::vec3(bCurrent->getMaxX(), bCurrent->getMaxY(), bCurrent->getMinZ());

	bVert[2] = glm::vec3(bCurrent->getMaxX(), bCurrent->getMinY(), bCurrent->getMaxZ());
	bVert[3] = glm::vec3(bCurrent->getMaxX(), bCurrent->getMinY(), bCurrent->getMinZ());

	bVert[4] = glm::vec3(bCurrent->getMinX(), bCurrent->getMaxY(), bCurrent->getMaxZ());
	bVert[5] = glm::vec3(bCurrent->getMinX(), bCurrent->getMaxY(), bCurrent->getMinZ());

	bVert[6] = glm::vec3(bCurrent->getMinX(), bCurrent->getMinY(), bCurrent->getMaxZ());
	bVert[7] = glm::vec3(bCurrent->getMinX(), bCurrent->getMinY(), bCurrent->getMinZ());

	for (int i = 0; i < 8; i++) {
		glm::vec4 current = glm::vec4(aVert[i].x, aVert[i].y, aVert[i].z, 1.0f);
		aVert[i] = glm::vec3(transformations * current);
		current = glm::vec4(bVert[i].x, bVert[i].y, bVert[i].z, 1.0f);
		bVert[i] = glm::vec3(target->getTranssform() * current);
	}

	return boxCollision(aVert, bVert, target);
}





