#pragma once
#include <string>
#include "gl/glew.h"
#include "gl/wglew.h"
#include "console.h"
#include "shaders/Shader.h"   
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "Octree\Octree.h"

#define LEFT_NORMAL 0
#define TOP_NORMAL 1
#define FRONT_NORMAL 2

class Actor
{
public:
	Actor();
	Actor(char* fileName);
	void render();
	glm::vec3 getPos();
	void setPos(glm::vec3 in);
	void setXPos(float x);
	void setYPos(float y);
	void setZPos(float z);
	glm::vec3 getScale();
	void setXScale(float x);
	void setYScale(float y);
	void setZScale(float z);
	void setHeading(float in);
	float getHeading();
	void init(char* filename);
	void renderFrame();
	void drawBoundBox();
	bool collidesWith(Actor *target);
	bool axisTest(glm::vec3 aVert[],glm::vec3 bVert[],glm::vec3 axis);
	glm::vec3 getNormal(int i);
	void setNormal(int i, glm::vec3 in);
	glm::mat4 getTranssform();
	bool boxCollision(glm::vec3 aVert[8], glm::vec3 bVert[8],Actor *target);
	bool octreeColl(Octree *aCurrent, Octree *bCurrent,Actor *target);
protected:
	glm::vec3 pos;								//position in the world
	float heading;								//rotation of the object
	glm::vec3 scale;							//scaling of object
	ThreeDModel model;
	glm::mat4 transformations;
	glm::vec3 normals[3];
};

extern Shader* shader;
extern glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
extern glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
extern glm::mat4 viewingMatrix;
extern std::vector<Actor*> collList;

