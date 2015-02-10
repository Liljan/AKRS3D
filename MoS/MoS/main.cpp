#include <iostream>
#include "Utilities.h"
#include "Shader.h"
#include "MatrixStack.hpp"
#include "physicsHandler.h"
#include "Box.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"

#include <time.h>

#include <SDKDDKVer.h>

using namespace std;

void setupViewport(GLFWwindow *window, GLfloat *P)
{
	int width, height;

	glfwGetWindowSize(window, &width, &height);

	P[0] = P[5] * height / width;

	glViewport(0, 0, width, height);
}

int main()
{
	srand((unsigned)time(NULL));


	GLfloat I[16] = { 1.0f, 0.0f, 0.0f, 0.0f
					, 0.0f, 1.0f, 0.0f, 0.0f
					, 0.0f, 0.0f, 1.0f, 0.0f
					, 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f
					, 0.0f, 2.42f, 0.0f, 0.0f
					, 0.0f, 0.0f, -1.0f, -1.0f
					, 0.0f, 0.0f, -0.2f, 0.0f };
	GLfloat L[3] = { 0.0f, 10.0f, -3.0f };
	GLfloat C[3];

	GLint locationMV;
	GLint locationP;
	GLint locationL;
	GLint locationColor;

	// start GLEW extension handler
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	//create GLFW window and select context
	GLFWwindow* window = glfwCreateWindow(640, 480, "Do not try and bend the spoon. That's impossible. Instead... only try to realize the truth.", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	//start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//Create objects
	std::vector<Entity*> objectList;

	Shader phongShader;
	phongShader.createShader("vertexshader.glsl", "fragmentshader.glsl");
	MatrixStack MVstack;
	MVstack.init();

	Camera theCamera;

	physicsHandler theHandler;

	Box theBox;
	theBox.createBox(1.5f, 0.5f, 0.5f);

	Sphere theSphere(glm::vec3(0.0f,5.0f,0.0f), 5.0f, 1.0f);
	theSphere.createSphere(0.5, 32);

	Sphere the2ndSphere(glm::vec3(0.0f, 8.0f, 0.0f), 5.0f, 1.0f);
	the2ndSphere.createSphere(0.5, 32);
	
	Plane thePlane;
	thePlane.createPlane(15.0f, 15.0f);

	objectList.push_back(new Sphere(glm::vec3(0.0f, 5.0f, 0.0f), 5.0f, 0.5f));
	objectList.push_back(new Sphere(glm::vec3(0.0f, 8.0f, 0.0f), 5.0f, 0.5f));

	//objectList.push_back(the2ndSphere);
	//objectList.push_back(thePlane);
	//objectList.push_back(theBox);

	//link variables to shader
	locationMV = glGetUniformLocation(phongShader.programID, "MV");
	locationP = glGetUniformLocation(phongShader.programID, "P");
	locationL = glGetUniformLocation(phongShader.programID, "lightPosition");
	locationColor = glGetUniformLocation(phongShader.programID, "objectColor");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Entity *oPointer;
	vector<Entity*> *vPointer;
	vPointer = &objectList;


	oPointer = &theSphere;
	glm::vec3 pos = glm::vec3(0.0f);
	while (!glfwWindowShouldClose(window)) {

		if (glfwGetKey(window, GLFW_KEY_O)) {
			objectList.push_back(new Sphere(glm::vec3(0.0f, 8.0f, 0.0f), 5.0f, 0.5f));
		}
		
		//GL calls
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glUseProgram(phongShader.programID);

		//Send static variables to vertexshader
		glUniformMatrix4fv(locationP, 1, GL_FALSE, P);
		

		setupViewport(window, P);

		//TIME
		theHandler.calculateTime();
		theCamera.poll(window);

		//Transform calculations and rendering
		MVstack.push();
		MVstack.translate(glm::vec3(0.0f, 0.0f, -theCamera.getRad() ));
		MVstack.rotX(theCamera.getTheta());
		MVstack.rotY(theCamera.getPhi());
			MVstack.push();
			//MVstack.rotZ(-0.1);
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());

				C[0] = thePlane.getColorR();
				C[1] = thePlane.getColorG();
				C[2] = thePlane.getColorB();

				glUniform3fv(locationL, 1, L);
				glUniform3fv(locationColor, 1, C);

				thePlane.render();
			MVstack.pop();

		//	oPointer = objectList[i];

			theHandler.calculatePosition(vPointer, window);
			theHandler.resolveCollision(vPointer);

			for (int i = 0; i < vPointer->size(); i++)
			{
				MVstack.push();
					glfwPollEvents();	
					MVstack.translate(oPointer->getPosition());
					glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
					oPointer = objectList[i];

					C[0] = oPointer->getColorR();
					C[1] = oPointer->getColorG();
					C[2] = oPointer->getColorB();

					glUniform3fv(locationColor, 1, C);
					oPointer->render();
				MVstack.pop();
			}

		MVstack.pop();

		glfwSwapBuffers(window);

	}

	glfwTerminate();

	return 0;
}