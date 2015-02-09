#include "physicsHandler.h"
#include "Sphere.h"
#include "Box.h"
#include <iostream>



physicsHandler::physicsHandler()
{
}


physicsHandler::~physicsHandler()
{
}

glm::vec3 physicsHandler::calculatePosition(Sphere& theEntity, GLFWwindow *window)
{
	glm::vec3 currPos = theEntity.getPosition();
	glm::vec3 currAcc = theEntity.getAcceleration();
	glm::vec3 currVel = theEntity.getVelocity();

	glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f)*12.0f;
	//TIME CALC move to separate function later
	float newTime = glfwGetTime();
	float deltaTime = newTime - currTime;
	currTime = newTime;

	//INPUT CHECK

	if (glfwGetKey(window, GLFW_KEY_LEFT)) {

		currAcc = (gravity + glm::vec3(-40.0f, 0.0f, 0.0f))/5.0f;
		cout << "hej";

	}
	else
	{
		currAcc = gravity / 5.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_UP)) {

		currAcc = (gravity + glm::vec3(0.0f, 200.0f, 0.0f)) / 5.0f;
		cout << "hej";

	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {

		currAcc = (gravity + glm::vec3(40.0f, 0.0f, 0.0f)) / 5.0f;
		cout << "hej";

	}

	if (glfwGetKey(window, GLFW_KEY_DOWN)) {

		currAcc = (gravity + glm::vec3(0.0f, -100.0f, 0.0f)) / 5.0f;
		cout << "hej";

	}
	if (glfwGetKey(window, GLFW_KEY_W)) {

		currAcc = (gravity + glm::vec3(0.0f, 0.0f, -40.0f)) / 5.0f;
		cout << "hej";

	}
	if (glfwGetKey(window, GLFW_KEY_S)) {

		currAcc = (gravity + glm::vec3(0.0f, 0.0f, 40.0f)) / 5.0f;
		cout << "hej";

	}


	//EULER CALC 
	glm::vec3 newVel = currVel + deltaTime*currAcc;
	glm::vec3 newPos = currPos + deltaTime*newVel;

	//COLLISION DETECT move to separate function later	
	if (newPos.y < 0.5)
	{
		glm::vec3 normal = glm::vec3(0.1f, 1.0f, 0.0f);
		glm::vec3 p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
		glm::vec3 p2Normal = glm::cross(normal, p1Normal);
		glm::mat4 coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		glm::vec4 reflectedNewVel = glm::vec4(glm::reflect(newVel, normal),1.0f);
		glm::vec4 nreflectedNewVel = glm::inverse(coSystem)*reflectedNewVel;

		nreflectedNewVel = glm::vec4(nreflectedNewVel.x*0.4f, nreflectedNewVel.y, nreflectedNewVel.z, 1.0f);

		reflectedNewVel = coSystem*nreflectedNewVel;

		theEntity.setVelocity(glm::vec3(reflectedNewVel));

		newPos = glm::vec3(newPos.x, 0.5001f, newPos.z);
		theEntity.setPosition(newPos);
		theEntity.setAcceleration(currAcc);
	}
	else
	{
		theEntity.setVelocity(newVel);
		theEntity.setPosition(newPos);
		theEntity.setAcceleration(currAcc);
	}
	

	//glm::mat4 trans;
	//trans = glm::translate(trans, currPos);
	return newPos;


}