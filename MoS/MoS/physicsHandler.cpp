#include "physicsHandler.h"
#include "Sphere.h"
#include "Box.h"
#include <iostream>



physicsHandler::physicsHandler()
{
	currTime = 0.0f;
	deltaTime = 0.0f;

	gravity = glm::vec3(0.0f, -9.81f, 0.0f)*1.0f;
}


physicsHandler::~physicsHandler()
{
}

void physicsHandler::calculateTime()
{
	float newTime = glfwGetTime();
	deltaTime = newTime - currTime;
	currTime = newTime;
}

void physicsHandler::integrate()
{
	//glm::vec3 newVel = currVel + deltaTime*currAcc;
	//glm::vec3 newPos = currPos + deltaTime*newVel;
}

void physicsHandler::calculatePosition(vector<Entity*> *theEntityList, GLFWwindow *window)
{

	for (int i = 0; i < theEntityList->size(); i++)
	{
		currPos = theEntityList->at(i)->getPosition();
		currAcc = theEntityList->at(i)->getAcceleration();
		currVel = theEntityList->at(i)->getVelocity();
		

		currAcc = gravity;

		handleKeyInput(window); 
		eulerCalc(theEntityList->at(i));
	}
}

void physicsHandler::handleKeyInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_KP_4)) {
		currAcc += glm::vec3(-40.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_KP_9)) {
		currAcc += glm::vec3(0.0f, 40.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_KP_6)) {
		currAcc += glm::vec3(40.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_KP_7)) {
		currAcc += glm::vec3(0.0f, -40.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_8)) {
		currAcc += glm::vec3(0.0f, 0.0f, -40.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_2)) {
		currAcc += glm::vec3(0.0f, 0.0f, 40.0f);
	}

	// Det ballar ur
	if (glfwGetKey(window, GLFW_KEY_C)) {
		currAcc = 10.0f*(glm::vec3(0.0f, 0.2f, 0.0f) - currPos);
	}
    // Freeze
    if (glfwGetKey(window, GLFW_KEY_C)) {
        currAcc = glm::vec3(0.0f, 0.0f, 0.0f);
        currVel = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void physicsHandler::eulerCalc(Entity* E)
{
	glm::vec3 newVel = currVel + deltaTime*currAcc;
	glm::vec3 newPos = currPos + deltaTime*newVel;

	E->setAcceleration(currAcc);
	E->setVelocity(newVel);
	E->setPosition(newPos);
}
void physicsHandler::resolveCollision(vector<Entity*> * theEntityList)
{
	glm::vec3 iVel;
	glm::vec3 iPos;
	glm::vec3 jPos;
	glm::vec3 jVel;

	glm::vec3 posVector;
	float vLength;
	float diffVel;

	glm::vec3 normal;
	glm::vec3 p1Normal;
	glm::vec3 p2Normal;
	glm::mat4 coSystem;

	glm::vec4 reflectedNewVel_1;
	glm::vec4 nreflectedNewVel_1;

	glm::vec4 reflectedNewVel_2;
	glm::vec4 nreflectedNewVel_2;

	glm::vec4 nBasePos;

	float rad1 = 0.5f;
	float rad2 = 0.5f;
	for (int i = 0; i < theEntityList->size() - 1; i++)
	{
		iPos = theEntityList->at(i)->getPosition();
		iVel = theEntityList->at(i)->getVelocity();

		for (int j = i + 1; j < theEntityList->size(); j++)
		{
			 
			jPos = theEntityList->at(j)->getPosition();
			jVel = theEntityList->at(j)->getVelocity();

			posVector = iPos - jPos;
			vLength = glm::length(posVector);

			if (vLength < rad1 + rad2 && i != j)
			{
				float move = (rad1 + rad2 - vLength);

				move /= 2.0f;

				normal = posVector;
				p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
				p2Normal = glm::cross(normal, p1Normal);
				coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				
				//reflectedNewVel_1 = glm::vec4(glm::reflect(iVel, normal), 1.0f);
				reflectedNewVel_1 = glm::vec4(iVel, 1.0f);
				nreflectedNewVel_1 = glm::inverse(coSystem)*reflectedNewVel_1;
				
				//reflectedNewVel_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
				reflectedNewVel_2 = glm::vec4(jVel, 1.0f);
				nreflectedNewVel_2 = glm::inverse(coSystem)*reflectedNewVel_2;
				
				diffVel = nreflectedNewVel_1.x - nreflectedNewVel_2.x;

				nreflectedNewVel_1 = glm::vec4((nreflectedNewVel_1.x-diffVel), nreflectedNewVel_1.y, nreflectedNewVel_1.z, 1.0f);
				reflectedNewVel_1 = coSystem*nreflectedNewVel_1;
				theEntityList->at(i)->setVelocity(glm::vec3(reflectedNewVel_1));

				nreflectedNewVel_2 = glm::vec4((nreflectedNewVel_2.x+diffVel), nreflectedNewVel_2.y, nreflectedNewVel_2.z, 1.0f);
				reflectedNewVel_2 = coSystem*nreflectedNewVel_2;
				theEntityList->at(j)->setVelocity(glm::vec3(reflectedNewVel_2));

				//FIRST BALL
				nBasePos = glm::inverse(coSystem)*glm::vec4(iPos, 1.0f);
				nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
				nBasePos = coSystem*nBasePos;
				theEntityList->at(i)->setPosition(glm::vec3(nBasePos));

				//SECOND BALL
				nBasePos = glm::inverse(coSystem)*glm::vec4(jPos, 1.0f);
				nBasePos = glm::vec4(nBasePos.x - move, nBasePos.y, nBasePos.z, 1.0f);
				nBasePos = coSystem*nBasePos;
				theEntityList->at(j)->setPosition(glm::vec3(nBasePos));

				//theEntityList->at(i)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
			}

		}

		glm::vec3 currPos = theEntityList->at(i)->getPosition();
		glm::vec3 currVel = theEntityList->at(i)->getVelocity();
		//COLLISION DETECT move to separate function later	
		if (currPos.y < 0.5)
		{
			normal = glm::vec3(0.1f, 1.0f, 0.0f);
			p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
			p2Normal = glm::cross(normal, p1Normal);
			coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			reflectedNewVel_1 = glm::vec4(glm::reflect(currVel, normal), 1.0f);
			nreflectedNewVel_1 = glm::inverse(coSystem)*reflectedNewVel_1;

			nreflectedNewVel_1 = glm::vec4(nreflectedNewVel_1.x*0.5f, nreflectedNewVel_1.y, nreflectedNewVel_1.z, 1.0f);

			reflectedNewVel_1 = coSystem*nreflectedNewVel_1;

			theEntityList->at(i)->setVelocity(glm::vec3(reflectedNewVel_1));
			theEntityList->at(i)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
		}
	}
}

