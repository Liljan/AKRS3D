#pragma once
#include "Utilities.h"
#include "Entity.h"
#include "Sphere.h"
#include "Box.h"
class physicsHandler
{
public:
	physicsHandler();

	glm::vec3 calculatePosition(Sphere& theEntity, GLFWwindow *window);

	~physicsHandler();

	float getTime(){ return currTime; }

private:
	float currTime = 0.0;
};

