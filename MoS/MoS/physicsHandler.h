#pragma once
#include "Utilities.h"
#include "Entity.h"
#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include <vector>
class physicsHandler
{
public:
	physicsHandler();
	~physicsHandler();

	void calculatePosition(vector<Entity*> *theEntityList, GLFWwindow *window);

	void integrate();

	void handleKeyInput(GLFWwindow *window);

	void eulerCalc(Entity *E);

	//void addObject(Entity& theEntity);

	void resolveCollision(vector<Entity*> *theEntityList);

	void calculateTime();
	
	float getTime(){ return currTime; }

private:
	float currTime;
	float deltaTime;
	glm::vec3 currPos;
	glm::vec3 currAcc;
	glm::vec3 currVel;
	glm::vec3 gravity;

	Plane *tempPlane;
	Box *tempBox;
	Sphere *tempSphere;

};

