#pragma once
#include "Utilities.h"
#include "Entity.h"
#include "Sphere.h"
#include "Box.h"
#include <vector>
class physicsHandler
{
public:
	physicsHandler();
	~physicsHandler();

	void calculatePosition(vector<Entity*> *theEntityList, GLFWwindow *window);

	void integrate();

	//void addObject(Entity& theEntity);

	void resolveCollision(vector<Entity*> *theEntityList);

	void calculateTime();
	
	float getTime(){ return currTime; }

private:
	float currTime = 0.0;
	float deltaTime = 0.0;
};

