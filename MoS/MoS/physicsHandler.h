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

	void calculateMovement(vector<Entity*> *theEntityList, GLFWwindow *window);

	void integrate(Entity *E);
	
	void handleKeyInput(GLFWwindow *window);


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
	glm::vec3 currRotVel;

	glm::vec3 currOrient;
	float currAngVel;
	float currAngAcc;
	float currAngPos;

	float elasticity = 1.0f;
	float m1, m2;
	float v3, v4;

	Plane *tempPlane;
	Box *tempBox1;
	Box *tempBox2;
	Sphere *tempSphere1;
	Sphere *tempSphere2;

};

