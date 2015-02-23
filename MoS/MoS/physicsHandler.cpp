#include "physicsHandler.h"
#include "Sphere.h"
#include "Box.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>



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
	//annu mer
	if (glfwGetKey(window, GLFW_KEY_X)){
		currAcc = 0.0f*(glm::vec3(0.0f, 0.2f, 0.0f) - currPos);
		currVel = glm::vec3(0);
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

	glm::vec3 normal;
	glm::vec3 p1Normal;
	glm::vec3 p2Normal;
	glm::mat4 coSystem;

	glm::vec4 reflectedNewVel_1;
	glm::vec4 nreflectedNewVel_1;

	glm::vec4 reflectedNewVel_2;
	glm::vec4 nreflectedNewVel_2;

	glm::vec4 nBasePos;

	glm::mat3 rotM;

	glm::vec2 Pdim;

	float rad1 = 0.5f;
	float rad2 = 0.5f;
	glm::vec3 dim1, dim2, orientation1, orientation2;
	float angle1, angle2;

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
				tempSphere = static_cast<Sphere*> (theEntityList->at(i));
				rad1 = tempSphere->getRadius();
				tempSphere = static_cast<Sphere*> (theEntityList->at(j));
				rad2 = tempSphere->getRadius();
				if (vLength < rad1 + rad2)
				{



					m1 = theEntityList->at(i)->getMass();
					m2 = theEntityList->at(j)->getMass();

					tempSphere = static_cast<Sphere*> (theEntityList->at(i));

					move = (rad1 + rad2 - vLength);

					move /= 2.0f;

					normal = posVector;
					p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
					p2Normal = glm::cross(normal, p1Normal);
					coSystem = glm::mat4(glm::vec4(glm::normalize(normal), 0.0f), glm::vec4(glm::normalize(p1Normal), 0.0f), glm::vec4(glm::normalize(p2Normal), 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				
					//reflectedNewVel_1 = glm::vec4(glm::reflect(iVel, normal), 1.0f);
					reflectedNewVel_1 = glm::vec4(iVel, 1.0f);
					nreflectedNewVel_1 = glm::transpose(coSystem)*reflectedNewVel_1;
				
					//reflectedNewVel_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
					reflectedNewVel_2 = glm::vec4(jVel, 1.0f);
					nreflectedNewVel_2 = glm::transpose(coSystem)*reflectedNewVel_2;
				
				//	diffVel = nreflectedNewVel_1.x - nreflectedNewVel_2.x;

					v3 = (m1*nreflectedNewVel_1.x + m2*nreflectedNewVel_2.x + m2*elasticity*(nreflectedNewVel_2.x - nreflectedNewVel_1.x)) / (m1 + m2);
					v4 = (m1*nreflectedNewVel_1.x + m2*nreflectedNewVel_2.x - m1*elasticity*(nreflectedNewVel_2.x - nreflectedNewVel_1.x)) / (m1 + m2);

					//nreflectedNewVel_1 = glm::vec4((nreflectedNewVel_1.x-diffVel), nreflectedNewVel_1.y, nreflectedNewVel_1.z, 1.0f);
					nreflectedNewVel_1 = glm::vec4(v3, nreflectedNewVel_1.y, nreflectedNewVel_1.z, 1.0f);
					reflectedNewVel_1 = coSystem*nreflectedNewVel_1;
					theEntityList->at(i)->setVelocity(glm::vec3(reflectedNewVel_1));
					
					//nreflectedNewVel_2 = glm::vec4((nreflectedNewVel_2.x+diffVel), nreflectedNewVel_2.y, nreflectedNewVel_2.z, 1.0f);
					nreflectedNewVel_2 = glm::vec4(v4, nreflectedNewVel_2.y, nreflectedNewVel_2.z, 1.0f);
					reflectedNewVel_2 = coSystem*nreflectedNewVel_2;
					theEntityList->at(j)->setVelocity(glm::vec3(reflectedNewVel_2));

					//FIRST BALL
					nBasePos = glm::transpose(coSystem)*glm::vec4(iPos, 1.0f);
					nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
					nBasePos = coSystem*nBasePos;
					theEntityList->at(i)->setPosition(glm::vec3(nBasePos));

					//SECOND BALL
					nBasePos = glm::transpose(coSystem)*glm::vec4(jPos, 1.0f);
					nBasePos = glm::vec4(nBasePos.x - move, nBasePos.y, nBasePos.z, 1.0f);
					nBasePos = coSystem*nBasePos;
					theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
				}
				//theEntityList->at(i)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
			}

			//BOX TO BOX
			if (theEntityList->at(i)->getOtype() == 'B' && theEntityList->at(j)->getOtype() == 'B')
			{
				tempBox = static_cast<Box*> (theEntityList->at(i));
				dim1 = tempBox->getDim();
				angle1 = tempBox->getAngularPosition();
				orientation1 = tempBox->getOrientation();
				tempBox = static_cast<Box*> (theEntityList->at(j));
				dim2 = tempBox->getDim();
				angle2 = tempBox->getAngularPosition();
				orientation2 = tempBox->getOrientation();

				//Kolla om de är innanför kollisionssfär
				if (vLength <= glm::length(dim1) + glm::length(dim2))
				{
					rotM = glm::mat3(cos(angle1)+pow(orientation1.x,2)*(1-cos(angle1)), orientation1.x*orientation1.y*(1-cos(angle1))-orientation1.z*sin(angle1), orientation1.x*orientation1.z*(1-cos(angle1))+orientation1.y*sin(angle1),
									 orientation1.x*orientation1.y*(1-cos(angle1))+orientation1.z*sin(angle1), cos(angle1)+pow(orientation1.y,2)*(1-cos(angle1)), orientation1.y*orientation1.z*(1-cos(angle1))-orientation1.x*sin(angle1),
									 orientation1.x*orientation1.z*(1-cos(angle1))-orientation1.y*sin(angle1), orientation1.y*orientation1.z*(1-cos(angle1))+orientation1.x*sin(angle1), cos(angle1)+pow(orientation1.z,2)*(1-cos(angle1)));

					//ortogonalprojektioner. Projicera alla punkter på linjen mellan kuberna för att avgöra vilka punkter som kolliderar
					//de första 8 står för box1 och de 8 sista för box2
					glm::vec3 projections[16];
					projections[0] = ((rotM*glm::vec3(dim1.x, dim1.y, dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[1] = ((rotM*glm::vec3(dim1.x, -dim1.y, dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[2] = ((rotM*glm::vec3(-dim1.x, dim1.y, dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[3] = ((rotM*glm::vec3(-dim1.x, -dim1.y, dim1.z)*posVector) / glm::dot(posVector,posVector))*posVector;
					projections[4] = ((rotM*glm::vec3(dim1.x, dim1.y, -dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[5] = ((rotM*glm::vec3(dim1.x, -dim1.y, -dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[6] = ((rotM*glm::vec3(-dim1.x, dim1.y, -dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[7] = ((rotM*glm::vec3(-dim1.x, -dim1.y, -dim1.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[8] = ((rotM*glm::vec3(dim2.x, dim2.y, dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[9] = ((rotM*glm::vec3(dim2.x, -dim2.y, dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[10] = ((rotM*glm::vec3(-dim2.x, dim2.y, dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[11] = ((rotM*glm::vec3(-dim2.x, -dim2.y, dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[12] = ((rotM*glm::vec3(dim2.x, dim2.y, -dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[13] = ((rotM*glm::vec3(dim2.x, -dim2.y, -dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[14] = ((rotM*glm::vec3(-dim2.x, dim2.y, -dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;
					projections[15] = ((rotM*glm::vec3(-dim2.x, -dim2.y, -dim2.z)*posVector) / glm::dot(posVector, posVector))*posVector;

					normal = posVector;
					p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
					p2Normal = glm::cross(normal, p1Normal);
					glm::mat3 tempCoSystem = glm::mat3(glm::vec3(glm::normalize(normal)), glm::vec3(glm::normalize(p1Normal)), glm::vec3(glm::normalize(p2Normal)));

					for (int k = 0; k < 16; k++)
						projections[k] = tempCoSystem*projections[k];

					float maxValue = 0, minValue = 10000;
					int maxIndex, minIndex;
					for (int k = 0; k < 8; k++)
					{
						if (projections[k].z > maxValue)
						{
							maxValue = projections[k].z;
							maxIndex = k;
						}
					}
					for (int k = 8; k < 16; k++)
					{
						if (projections[k].z < minValue)
						{
							minValue = projections[k].z;
							minIndex = k;
						}
					}
					glm::vec3 newVel = projections[maxIndex] - (posVector - projections[minIndex]);
					theEntityList->at(i)->setVelocity(-newVel);
					theEntityList->at(j)->setVelocity(newVel);

					cout << projections[0].z << " " << projections[1].z << " " << projections[2].z << " " << maxIndex << endl;
				}
			}


			//SPHERE TO PLANE
			if (theEntityList->at(i)->getOtype() == 'P' && theEntityList->at(j)->getOtype() == 'S')
			{
				tempPlane = static_cast<Plane*> (theEntityList->at(i));
				tempSphere = static_cast<Sphere*> (theEntityList->at(j));
				
				normal = tempPlane->getNormal();
				p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
				p2Normal = glm::cross(normal, p1Normal);
				coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				//nBasePos = glm::inverse(coSystem) * glm::translate(glm::mat4(1), glm::vec3(0.0f, -tempSphere->getPosition().y, 0.0f)) * glm::vec4(jPos, 1.0f);
				Pdim = tempPlane->getDim();
				posVector = tempPlane->getPosition();

				nBasePos = glm::transpose(coSystem) * 
					glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							-posVector.x, -posVector.y, -posVector.z, 1.0f) *
							glm::vec4(jPos, 1.0f);

				if (nBasePos.x > -tempSphere->getRadius() && nBasePos.x < tempSphere->getRadius() && nBasePos.y < Pdim.y / 2.0f && nBasePos.y > -Pdim.y / 2.0f && nBasePos.z < Pdim.x / 2.0f && nBasePos.z > -Pdim.x / 2.0f)
				{

				//	glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
				//		0.0f, 1.0f, 0.0f, -8.0f,
				//		0.0f, 0.0f, 1.0f, 0.0f,
				//		0.0f, 0.0f, 0.0f, 1.0f);

					
					reflectedNewVel_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
					nreflectedNewVel_2 = glm::inverse(coSystem)*reflectedNewVel_2;
					nreflectedNewVel_2 = glm::vec4(nreflectedNewVel_2.x*0.5f, nreflectedNewVel_2.y, nreflectedNewVel_2.z, 1.0f);
					reflectedNewVel_2 = coSystem*nreflectedNewVel_2;
					theEntityList->at(j)->setVelocity(glm::vec3(reflectedNewVel_2));

					//nBasePos = glm::inverse(coSystem)*glm::vec4(jPos, 1.0f);

					move = (nBasePos.x / abs(nBasePos.x)) * (tempSphere->getRadius() - abs(nBasePos.x));
					nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
					//nBasePos = glm::translate(glm::mat4(1), glm::vec3(0.0f, tempSphere->getPosition().y, 0.0f))*coSystem*nBasePos;
					nBasePos =
						glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						posVector.x, posVector.y, posVector.z, 1.0f) *
						(coSystem) *
						nBasePos;
					theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
				}
				//reflectedNewVel_1 = glm::vec4(glm::reflect(currVel, normal), 1.0f);
				//nreflectedNewVel_1 = glm::inverse(coSystem)*reflectedNewVel_1;
				//nreflectedNewVel_1 = glm::vec4(nreflectedNewVel_1.x*0.5f, nreflectedNewVel_1.y, nreflectedNewVel_1.z, 1.0f);

				//reflectedNewVel_1 = coSystem*nreflectedNewVel_1;


				//theEntityList->at(j)->setPosition(glm::vec3(currPos.x, 0.50001f, currPos.z));
			}
			
			//BOX TO PLANE
			if (theEntityList->at(i)->getOtype() == 'P' && theEntityList->at(j)->getOtype() == 'B')
			{
				tempPlane = static_cast<Plane*> (theEntityList->at(i));
				tempBox = static_cast<Box*> (theEntityList->at(j));

				normal = tempPlane->getNormal();
				p1Normal = glm::cross(normal, glm::cross(normal, glm::vec3(normal.z, -normal.x, -normal.y)));
				p2Normal = glm::cross(normal, p1Normal);
				coSystem = glm::mat4(glm::vec4(normal, 0.0f), glm::vec4(p1Normal, 0.0f), glm::vec4(p2Normal, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				//nBasePos = glm::inverse(coSystem) * glm::translate(glm::mat4(1), glm::vec3(0.0f, -tempSphere->getPosition().y, 0.0f)) * glm::vec4(jPos, 1.0f);
				Pdim = tempPlane->getDim();
				posVector = tempPlane->getPosition();

				nBasePos = glm::transpose(coSystem) *
					glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					-posVector.x, -posVector.y, -posVector.z, 1.0f) *
					glm::vec4(jPos, 1.0f);

				float tempRadius = glm::length(tempBox->getDim());
				if (nBasePos.x > -tempRadius && nBasePos.x < tempRadius && nBasePos.y < Pdim.y / 2.0f && nBasePos.y > -Pdim.y / 2.0f && nBasePos.z < Pdim.x / 2.0f && nBasePos.z > -Pdim.x / 2.0f)
				{

					//	glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
					//		0.0f, 1.0f, 0.0f, -8.0f,
					//		0.0f, 0.0f, 1.0f, 0.0f,
					//		0.0f, 0.0f, 0.0f, 1.0f);


					reflectedNewVel_2 = glm::vec4(glm::reflect(jVel, normal), 1.0f);
					nreflectedNewVel_2 = glm::inverse(coSystem)*reflectedNewVel_2;
					nreflectedNewVel_2 = glm::vec4(nreflectedNewVel_2.x*0.5f, nreflectedNewVel_2.y, nreflectedNewVel_2.z, 1.0f);
					reflectedNewVel_2 = coSystem*nreflectedNewVel_2;
					theEntityList->at(j)->setVelocity(glm::vec3(reflectedNewVel_2));

					//nBasePos = glm::inverse(coSystem)*glm::vec4(jPos, 1.0f);

					move = (nBasePos.x / abs(nBasePos.x)) * (tempRadius - abs(nBasePos.x));
					nBasePos = glm::vec4(nBasePos.x + move, nBasePos.y, nBasePos.z, 1.0f);
					//nBasePos = glm::translate(glm::mat4(1), glm::vec3(0.0f, tempSphere->getPosition().y, 0.0f))*coSystem*nBasePos;
					nBasePos =
						glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						posVector.x, posVector.y, posVector.z, 1.0f) *
						(coSystem)*
						nBasePos;
					theEntityList->at(j)->setPosition(glm::vec3(nBasePos));
				}
			}
		}

	}
}

