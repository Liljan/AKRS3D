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
		glm::vec3 currPos = theEntityList->at(i)->getPosition();
		glm::vec3 currAcc = theEntityList->at(i)->getAcceleration();
		glm::vec3 currVel = theEntityList->at(i)->getVelocity();
		glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f)*0.01f;

		//INPUT CHECK
		currAcc = gravity;

		//EULER CALC 
		glm::vec3 newVel = currVel + deltaTime*currAcc;
		glm::vec3 newPos = currPos + deltaTime*newVel;

		theEntityList->at(i)->setAcceleration(currAcc);
		theEntityList->at(i)->setVelocity(newVel);
		theEntityList->at(i)->setPosition(newPos);
	}

}

void physicsHandler::resolveCollision(vector<Entity*> * theEntityList)
{
	glm::vec3 iVel;
	glm::vec3 iPos;
	glm::vec3 jPos;
	glm::vec3 posVector;
	float vLength;

	glm::vec3 normal;
	glm::vec3 p1Normal;
	glm::vec3 p2Normal;
	glm::mat4 coSystem;

	glm::vec4 reflectedNewVel;
	glm::vec4 nreflectedNewVel;

	float rad1 = 0.5f;
	float rad2 = 0.5f;
	for (int i = 0; i < theEntityList->size(); i++)
	{
		iPos = theEntityList->at(i)->getPosition();
		iVel = theEntityList->at(i)->getVelocity();

		for (int j = 0; j < theEntityList->size(); j++)
		{
			 
			jPos = theEntityList->at(j)->getPosition();

			posVector = iPos - jPos;
			vLength = glm::length(posVector);

			if (vLength < rad1 + rad2 && i != j)
			{
				normal = posVector;
				p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
				p2Normal = glm::cross(normal, p1Normal);
				coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				reflectedNewVel = glm::vec4(glm::reflect(iVel, normal), 1.0f);
				nreflectedNewVel = glm::inverse(coSystem)*reflectedNewVel;

				nreflectedNewVel = glm::vec4(nreflectedNewVel.x*0.5f, nreflectedNewVel.y, nreflectedNewVel.z, 1.0f);

				reflectedNewVel = coSystem*nreflectedNewVel;

				theEntityList->at(i)->setVelocity(glm::vec3(reflectedNewVel));
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

			reflectedNewVel = glm::vec4(glm::reflect(currVel, normal), 1.0f);
			nreflectedNewVel = glm::inverse(coSystem)*reflectedNewVel;

			nreflectedNewVel = glm::vec4(nreflectedNewVel.x*0.5f, nreflectedNewVel.y, nreflectedNewVel.z, 1.0f);

			reflectedNewVel = coSystem*nreflectedNewVel;

			theEntityList->at(i)->setVelocity(glm::vec3(reflectedNewVel));
			theEntityList->at(i)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
		}
	}
}

