#include "physicsHandler.h"
#include "Sphere.h"
#include "Box.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "glm/gtx/rotate_vector.hpp"



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

void physicsHandler::integrate(Entity *E)
{
	glm::vec3 newVel = currVel + deltaTime*currAcc;
	glm::vec3 newPos = currPos + deltaTime*newVel;
	
	float newAngVel = currAngVel + deltaTime*currAngAcc;
	float newAngPos = currAngPos + deltaTime*newAngVel;

	E->setAcceleration(currAcc);
	E->setVelocity(newVel);
	E->setPosition(newPos);

	//E->setOrientation(glm::normalize(glm::rotate(E->getOrientation(), newAngPos - currAngPos, E->getRotAxis())));
	

	//cout << E->getOrientation().x << endl;
	E->setAngularAcceleration(currAngAcc);
	E->setAngularVelocity(newAngVel);
	E->setAngularPosition(newAngPos);
}

void physicsHandler::calculateMovement(vector<Entity*> *theEntityList, GLFWwindow *window)
{

	for (int i = 0; i < theEntityList->size(); i++)
	{
		if (theEntityList->at(i)->getOtype() != 'P')
		{
			currPos = theEntityList->at(i)->getPosition();
			currAcc = theEntityList->at(i)->getAcceleration();
			currVel = theEntityList->at(i)->getVelocity();

			currAcc = gravity;

			currAngAcc = theEntityList->at(i)->getAngularAcceleration();
			currAngVel = theEntityList->at(i)->getAngularVelocity();
			currAngPos = theEntityList->at(i)->getAngularPosition();

			handleKeyInput(window); 
			integrate(theEntityList->at(i));
		}
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
	if (glfwGetKey(window, GLFW_KEY_C)){
		currAcc = 10.0f*(glm::vec3(0.0f, 0.2f, 0.0f) - currPos);
	}
}


void physicsHandler::resolveCollision(vector<Entity*> * theEntityList)
{
	srand(time(NULL));

	glm::vec3 iVel;
	glm::vec3 iPos;
	glm::vec3 jPos;
	glm::vec3 jVel;

	glm::vec3 posVector;
	float vLength;
	float diffVel;
	float move;
	float j;
	float vRel;

	glm::vec3 normal;
	glm::vec3 p1Normal;
	glm::vec3 p2Normal;
	glm::mat4 rotCoSystem1;
	glm::mat4 posCoSystem1;
	glm::mat4 rotCoSystem2;
	glm::mat4 posCoSystem2;

	glm::vec4 vec_1;
	glm::vec4 nVec_1;

	glm::vec4 vec_2;
	glm::vec4 nVec_2;

	glm::vec3 vec3_1;
	glm::vec3 vec3_2;
	glm::vec3 vec3_3;
	glm::vec3 vec3_4;

	glm::vec4 nBasePos;

	glm::vec2 Pdim;

	float rad1 = 0.5f;
	float rad2 = 0.5f;
	float length = 0.f;
	float J = 0.0f;

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

			//SPHERE TO SPHERE
			if ( theEntityList->at(i)->getOtype() == 'S' && theEntityList->at(j)->getOtype() == 'S')
			{
				tempSphere1 = static_cast<Sphere*> (theEntityList->at(i));
				tempSphere2 = static_cast<Sphere*> (theEntityList->at(j));
				if (vLength < tempSphere1->getRadius() + tempSphere2->getRadius())
				{

					posVector = glm::normalize(posVector);

					m1 = theEntityList->at(i)->getMass();
					m2 = theEntityList->at(j)->getMass();

					move = (tempSphere1->getRadius() + tempSphere2->getRadius() - vLength);

					move /= 2.0f;

					normal = posVector;
					p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
					p2Normal = glm::cross(normal, p1Normal);
					rotCoSystem1 = glm::mat4(glm::vec4(glm::normalize(normal), 0.0f), glm::vec4(glm::normalize(p1Normal), 0.0f), glm::vec4(glm::normalize(p2Normal), 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				
					//vec_1 = glm::vec4(glm::reflect(iVel, normal), 1.0f);
					vec_1 = glm::vec4(iVel, 1.0f);
					nVec_1 = glm::transpose(rotCoSystem1)*vec_1;
				
					//vec_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
					vec_2 = glm::vec4(jVel, 1.0f);
					nVec_2 = glm::transpose(rotCoSystem1)*vec_2;
				
				//	diffVel = nVec_1.x - nVec_2.x;

					v3 = (m1*nVec_1.x + m2*nVec_2.x + m2*elasticity*(nVec_2.x - nVec_1.x)) / (m1 + m2);
					v4 = (m1*nVec_1.x + m2*nVec_2.x - m1*elasticity*(nVec_2.x - nVec_1.x)) / (m1 + m2);

					//nVec_1 = glm::vec4((nVec_1.x-diffVel), nVec_1.y, nVec_1.z, 1.0f);
					nVec_1 = glm::vec4(v3, nVec_1.y, nVec_1.z, 1.0f);
					vec_1 = rotCoSystem1*nVec_1;
					theEntityList->at(i)->setVelocity(glm::vec3(vec_1));
					
					//nVec_2 = glm::vec4((nVec_2.x+diffVel), nVec_2.y, nVec_2.z, 1.0f);
					nVec_2 = glm::vec4(v4, nVec_2.y, nVec_2.z, 1.0f);

					vec_2 = rotCoSystem1*nVec_2;
					theEntityList->at(j)->setVelocity(glm::vec3(vec_2));

					//FIRST BALL
					nBasePos = glm::transpose(rotCoSystem1)*glm::vec4(iPos, 1.0f);
					nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
					nBasePos = rotCoSystem1*nBasePos;
					theEntityList->at(i)->setPosition(glm::vec3(nBasePos));

					//SECOND BALL
					nBasePos = glm::transpose(rotCoSystem1)*glm::vec4(jPos, 1.0f);
					nBasePos = glm::vec4(nBasePos.x - move, nBasePos.y, nBasePos.z, 1.0f);
					nBasePos = rotCoSystem1*nBasePos;
					theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
					

					// ROTATION

					//vec3_1 = iVel - jVel;

					//j = -(1+elasticity) * vec3_1 *

					vec3_1 = tempSphere1->getAngularVelocity() * glm::normalize(glm::cross(posVector, tempSphere1->getRotAxis())) - glm::vec3(0.0f, nVec_2.y, nVec_2.z)  ;
					vec3_2 =  tempSphere2->getAngularVelocity() * glm::normalize(glm::cross(posVector, tempSphere2->getRotAxis())) + glm::vec3(0.0f, nVec_1.y, nVec_1.z)  ;

					//vec3_1 = iVel + tempSphere1->getAngularVelocity() * glm::normalize(glm::cross(posVector, tempSphere1->getRotAxis()));
					//vec3_2 = jVel + tempSphere2->getAngularVelocity() * glm::normalize(glm::cross(posVector, tempSphere2->getRotAxis()));

					//vRel = glm::dot(posVector, (vec3_1 - vec3_2);

					//j = -(1 + elasticity) * vRel / ( 1/m1 + 1/m2 + posVector )
					//vec3_3 = vec3_1;

					//vec3_1 = vec3_2;
					//vec3_2 = vec3_3;

					theEntityList->at(i)->setAngularVelocity(glm::length(vec3_1));
					theEntityList->at(j)->setAngularVelocity(glm::length(vec3_2));

					vec3_1 = glm::cross(vec3_1, posVector);
					vec3_2 = glm::cross(vec3_2, posVector);

					theEntityList->at(i)->setRotAxis(vec3_1);
					theEntityList->at(j)->setRotAxis(vec3_2);
										

				}
				//theEntityList->at(i)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
			}

			//BOX TO BOX
			if (theEntityList->at(i)->getOtype() == 'B' && theEntityList->at(j)->getOtype() == 'B')
			{
				m1 = theEntityList->at(i)->getMass();
				m2 = theEntityList->at(j)->getMass();

				tempBox1 = static_cast<Box*> (theEntityList->at(i));
				tempBox2 = static_cast<Box*> (theEntityList->at(j));
				vec3_1 = tempBox1->getDim();

				posVector = iPos - jPos;

				std::vector<glm::vec3> hitPoints;
				iPos = tempBox1->getPosition();
				jPos = tempBox2->getPosition();

				if (vec3_1.x*2.0f > glm::length(iPos - jPos))
				{

					

					//CHECK IF 2nd box is within 1st box
					///////////////////////////////////////////////////////////////////////////////////////////////////////
					normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis()) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
					p1Normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis()) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
					p2Normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis()) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

					rotCoSystem1 = glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis());
					posCoSystem1 = glm::translate(glm::mat4(1), jPos);

					std::vector<glm::vec4> vertexList;
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
					vertexList.push_back(rotCoSystem1 * posCoSystem1 * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));

					rotCoSystem1 = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
					posCoSystem1 = glm::translate(glm::mat4(1), -iPos);

					//rotCoSystem1 = glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis());
					
					vertexList[0] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[0];
					vertexList[1] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[1];
					vertexList[2] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[2];
					vertexList[3] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[3];
					vertexList[4] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[4];
					vertexList[5] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[5];
					vertexList[6] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[6];
					vertexList[7] = glm::transpose(rotCoSystem1) * posCoSystem1 * vertexList[7];
						
					/*
					vertexList[0] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[0];
					vertexList[1] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[1];
					vertexList[2] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[2];
					vertexList[3] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[3];
					vertexList[4] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[4];
					vertexList[5] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[5];
					vertexList[6] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[6];
					vertexList[7] = glm::inverse(rotCoSystem1) * glm::inverse(posCoSystem1) * vertexList[7];
					*/

					vec_1 = glm::transpose(rotCoSystem1) * posCoSystem1 * glm::vec4(jPos, 1.0f);

					nVec_1 = glm::transpose(rotCoSystem1) * glm::vec4(iVel, 1.0f);
					nVec_2 = glm::transpose(rotCoSystem1) * glm::vec4(jVel, 1.0f);

					for (int k = 0; k < 8; k++)
					{
						vertexList[k] = vertexList[k] - glm::vec4(iPos, 0);
						if ((vertexList[k].x <= vec3_1.x / 2.0f && vertexList[k].x >= -vec3_1.x / 2.0f) && (vertexList[k].y <= vec3_1.y / 2.0f && vertexList[k].y >= -vec3_1.y / 2.0f) && (vertexList[k].z <= vec3_1.z / 2.0f && vertexList[k].z >= -vec3_1.z / 2.0f))
						{
							cout << vertexList[k].x << " " << vertexList[k].y << " " << vertexList[k].z << endl;
							posCoSystem1 = glm::translate(glm::mat4(1), iPos);

							hitPoints.push_back(glm::vec3(posCoSystem1 * rotCoSystem1 * vertexList[k]));
							cout << hitPoints[0].x << " " << hitPoints[0].y << " " << hitPoints[0].z << endl;

						}
					}

					vertexList.clear();
					//CHECK IF 1st box is within 2nd box
					///////////////////////////////////////////////////////////////////////////////////////////////////////
					normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis()) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

					p1Normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis()) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
					p2Normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis()) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

					rotCoSystem2 = glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis());
					posCoSystem2 = glm::translate(glm::mat4(1), iPos);

					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
					vertexList.push_back(rotCoSystem2 * posCoSystem2 * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));

					rotCoSystem2 = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
					posCoSystem2 = glm::translate(glm::mat4(1), -jPos);

					//rotCoSystem2 = glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis());
					
					/*
					vertexList[0] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[0];
					vertexList[1] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[1];
					vertexList[2] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[2];
					vertexList[3] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[3];
					vertexList[4] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[4];
					vertexList[5] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[5];
					vertexList[6] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[6];
					vertexList[7] = glm::inverse(rotCoSystem2) * glm::inverse(posCoSystem2) * vertexList[7];
					*/
						
					vertexList[0] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[0];
					vertexList[1] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[1];
					vertexList[2] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[2];
					vertexList[3] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[3];
					vertexList[4] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[4];
					vertexList[5] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[5];
					vertexList[6] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[6];
					vertexList[7] = glm::transpose(rotCoSystem2) * posCoSystem2 * vertexList[7];
						

					vec_2 = glm::transpose(rotCoSystem2) * posCoSystem2 * glm::vec4(iPos, 1.0f);

					for (int k = 0; k < 8; k++)
					{
						vertexList[k] = vertexList[k] - glm::vec4(jPos, 1.0f);
						if ((vertexList[k].x <= vec3_1.x / 2.0f && vertexList[k].x >= -vec3_1.x / 2.0f) && (vertexList[k].y <= vec3_1.y / 2.0f && vertexList[k].y >= -vec3_1.y / 2.0f) && (vertexList[k].z <= vec3_1.z / 2.0f && vertexList[k].z >= -vec3_1.z / 2.0f))
						{
							posCoSystem2 = glm::translate(glm::mat4(1), jPos);

							hitPoints.push_back(glm::vec3(posCoSystem2 * rotCoSystem2 * vertexList[k]));

							cout << " hej";
						}
					}

					//Hitpoints found
					///////////////////////////////////////////////////////////////////////////////////////////////////////

					//posVector = iPos - jPos;
					posVector = glm::normalize(posVector);
					////////////////////////////////////////////////////
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 1.0f, .0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
					}
					////
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, -1.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(0.0f, -1.0f, .0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(-1.0f, 0.0f, 1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox1->getAngularPosition(), tempBox1->getRotAxis())*glm::vec4(-1.0f, 0.0f, 1.0f, 1.0f)));
					}
					///////////////////////////////////////////////////////////
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 1.0f, .0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
					}
					////
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, -1.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(0.0f, -1.0f, .0f, 1.0f)));
					}
					if (glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f))) > length)
					{
						length = glm::dot(posVector, glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(-1.0f, 0.0f, 1.0f, 1.0f)));
						normal = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), tempBox2->getAngularPosition(), tempBox2->getRotAxis())*glm::vec4(-1.0f, 0.0f, 1.0f, 1.0f)));
					}
					///////////////////////////////////////////////////////////

					if (hitPoints.size() != 0 && hitPoints.size() != 8)
					{


						vec3_1 = hitPoints[0] - iPos;
						vec3_2 = hitPoints[0] - jPos;

						vec3_3 = tempBox1->getVelocity() + tempBox1->getAngularVelocity() * glm::normalize(glm::cross(vec3_1, tempBox1->getRotAxis()));
						vec3_4 = tempBox2->getVelocity() + tempBox2->getAngularVelocity() * glm::normalize(glm::cross(vec3_2, tempBox2->getRotAxis()));

						J = (-(1 + elasticity) * glm::dot((glm::vec3(nVec_1) - glm::vec3(nVec_2)), normal)) /
							1 / m1 + 1 / m2 + glm::dot(normal, glm::cross(glm::inverse(tempBox1->getInertia())*glm::cross(vec3_1, normal), vec3_1)) + glm::dot(normal, glm::cross(glm::inverse(tempBox2->getInertia())*glm::cross(vec3_2, normal), vec3_2));

						cout << J << endl;
						vec3_4 = tempBox1->getVelocity() - J*normal / m1;
						vec3_3 = tempBox1->getAngularVelocity() * tempBox1->getRotAxis() - glm::inverse(tempBox1->getInertia())*glm::cross(vec3_1, J*normal);

						vec3_1 = tempBox2->getVelocity() + J*normal / m2;
						vec3_2 = tempBox2->getAngularVelocity() * tempBox2->getRotAxis() + glm::inverse(tempBox2->getInertia())*glm::cross(vec3_2, J*normal);

						theEntityList->at(i)->setPosition(iPos + 0.5f*normal);
						theEntityList->at(i)->setVelocity(vec3_4);
						theEntityList->at(i)->setRotAxis(glm::normalize(vec3_3));
						theEntityList->at(i)->setAngularVelocity(glm::length(vec3_3));

						theEntityList->at(i)->setPosition(jPos - 0.5f*normal);
						theEntityList->at(j)->setVelocity(vec3_1);
						theEntityList->at(j)->setRotAxis(glm::normalize(vec3_2));
						theEntityList->at(j)->setAngularVelocity(glm::length(vec3_2));
					}
				}
			}

			//SPHERE TO PLANE
			if (theEntityList->at(i)->getOtype() == 'P' && theEntityList->at(j)->getOtype() == 'S')
			{
				tempPlane = static_cast<Plane*> (theEntityList->at(i));
				tempSphere1 = static_cast<Sphere*> (theEntityList->at(j));
				
				normal = tempPlane->getNormal();
				p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
				p2Normal = glm::cross(normal, p1Normal);
				rotCoSystem1 = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				//nBasePos = glm::inverse(coSystem) * glm::translate(glm::mat4(1), glm::vec3(0.0f, -tempSphere->getPosition().y, 0.0f)) * glm::vec4(jPos, 1.0f);
				Pdim = tempPlane->getDim();
				posVector = tempPlane->getPosition();

				nBasePos = glm::transpose(rotCoSystem1) *
					glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							-posVector.x, -posVector.y, -posVector.z, 1.0f) *
							glm::vec4(jPos, 1.0f);

				if (nBasePos.x > -tempSphere1->getRadius() && nBasePos.x < tempSphere1->getRadius() && nBasePos.y < Pdim.y / 2.0f && nBasePos.y > -Pdim.y / 2.0f && nBasePos.z < Pdim.x / 2.0f && nBasePos.z > -Pdim.x / 2.0f)
				{

				//	glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
				//		0.0f, 1.0f, 0.0f, -8.0f,
				//		0.0f, 0.0f, 1.0f, 0.0f,
				//		0.0f, 0.0f, 0.0f, 1.0f);

					
					vec_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
					nVec_2 = glm::inverse(rotCoSystem1)*vec_2;
					nVec_2 = glm::vec4(nVec_2.x*0.5f, nVec_2.y, nVec_2.z, 1.0f);
					vec_2 = rotCoSystem1*nVec_2;
					theEntityList->at(j)->setVelocity(glm::vec3(vec_2));

					//nBasePos = glm::inverse(coSystem)*glm::vec4(jPos, 1.0f);

					move = (nBasePos.x / abs(nBasePos.x)) * (tempSphere1->getRadius() - abs(nBasePos.x));
					nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
					//nBasePos = glm::translate(glm::mat4(1), glm::vec3(0.0f, tempSphere->getPosition().y, 0.0f))*coSystem*nBasePos;
					nBasePos =
						glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						posVector.x, posVector.y, posVector.z, 1.0f) *
						(rotCoSystem1)*
						nBasePos;
					theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
				}
				//vec_1 = glm::vec4(glm::reflect(currVel, normal), 1.0f);
				//nVec_1 = glm::inverse(coSystem)*vec_1;
				//nVec_1 = glm::vec4(nVec_1.x*0.5f, nVec_1.y, nVec_1.z, 1.0f);

				//vec_1 = coSystem*nVec_1;


				//theEntityList->at(j)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
			}

			if (theEntityList->at(i)->getOtype() == 'P' && theEntityList->at(j)->getOtype() == 'B')
			{
				tempPlane = static_cast<Plane*> (theEntityList->at(i));
				tempBox1 = static_cast<Box*> (theEntityList->at(j));

				normal = tempPlane->getNormal();
				p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
				p2Normal = glm::cross(normal, p1Normal);
				rotCoSystem1 = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				//nBasePos = glm::inverse(coSystem) * glm::translate(glm::mat4(1), glm::vec3(0.0f, -tempSphere->getPosition().y, 0.0f)) * glm::vec4(jPos, 1.0f);
				Pdim = tempPlane->getDim();
				posVector = tempPlane->getPosition();

				nBasePos = glm::transpose(rotCoSystem1) *
					glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					-posVector.x, -posVector.y, -posVector.z, 1.0f) *
					glm::vec4(jPos, 1.0f);

				if (nBasePos.x > -tempBox1->getDim().x && nBasePos.x < tempBox1->getDim().x && nBasePos.y < Pdim.y / 2.0f && nBasePos.y > -Pdim.y / 2.0f && nBasePos.z < Pdim.x / 2.0f && nBasePos.z > -Pdim.x / 2.0f)
				{

					//	glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
					//		0.0f, 1.0f, 0.0f, -8.0f,
					//		0.0f, 0.0f, 1.0f, 0.0f,
					//		0.0f, 0.0f, 0.0f, 1.0f);


					vec_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
					nVec_2 = glm::inverse(rotCoSystem1)*vec_2;
					nVec_2 = glm::vec4(nVec_2.x*0.5f, nVec_2.y, nVec_2.z, 1.0f);
					vec_2 = rotCoSystem1*nVec_2;
					theEntityList->at(j)->setVelocity(glm::vec3(vec_2));

					//nBasePos = glm::inverse(coSystem)*glm::vec4(jPos, 1.0f);

					move = (nBasePos.x / abs(nBasePos.x)) * (tempBox1->getDim().x - abs(nBasePos.x));
					nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
					//nBasePos = glm::translate(glm::mat4(1), glm::vec3(0.0f, tempSphere->getPosition().y, 0.0f))*coSystem*nBasePos;
					nBasePos =
						glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						posVector.x, posVector.y, posVector.z, 1.0f) *
						(rotCoSystem1)*
						nBasePos;
					theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
				}
				//vec_1 = glm::vec4(glm::reflect(currVel, normal), 1.0f);
				//nVec_1 = glm::inverse(coSystem)*vec_1;
				//nVec_1 = glm::vec4(nVec_1.x*0.5f, nVec_1.y, nVec_1.z, 1.0f);

				//vec_1 = coSystem*nVec_1;


				//theEntityList->at(j)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
			}

		}
		/*
		glm::vec3 currPos = theEntityList->at(i)->getPosition();
		glm::vec3 currVel = theEntityList->at(i)->getVelocity();
		//COLLISION DETECT move to separate function later	
		if (currPos.y < 0.5)
		{
			normal = glm::vec3(0.1f, 1.0f, 0.0f);
			p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
			p2Normal = glm::cross(normal, p1Normal);
			coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			vec_1 = glm::vec4(glm::reflect(currVel, normal), 1.0f);
			nVec_1 = glm::inverse(coSystem)*vec_1;

			nVec_1 = glm::vec4(nVec_1.x*0.5f, nVec_1.y, nVec_1.z, 1.0f);

			vec_1 = coSystem*nVec_1;

			theEntityList->at(i)->setVelocity(glm::vec3(vec_1));
			theEntityList->at(i)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
		}
		*/
	}
}

