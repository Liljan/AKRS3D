#include <iostream>
#include "Utilities.h"
#include "Shader.h"
#include "MatrixStack.hpp"
#include "physicsHandler.h"
#include "Box.h"
#include "Sphere.h"
#include "Plane.h"

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

	GLfloat I[16] = { 1.0f, 0.0f, 0.0f, 0.0f
					, 0.0f, 1.0f, 0.0f, 0.0f
					, 0.0f, 0.0f, 1.0f, 0.0f
					, 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f
					, 0.0f, 2.42f, 0.0f, 0.0f
					, 0.0f, 0.0f, -1.0f, -1.0f
					, 0.0f, 0.0f, -0.2f, 0.0f };
	GLfloat L[3] = { 0.0f, 2.0f, 12.0f };

	GLint locationMV;
	GLint locationP;
	GLint locationL;

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
	Shader phongShader;
	phongShader.createShader("vertexshader.glsl", "fragmentshader.glsl");
	MatrixStack MVstack;
	MVstack.init();
	physicsHandler theHandler;

	Box theBox;
	theBox.createBox(1.5f, 0.5f, 0.5f);

	Sphere theSphere(glm::vec3(0.0f,6.0f,0.0f), 5.0f, 1.0f);
	theSphere.createSphere(0.5, 32);

	Sphere the2ndSphere(glm::vec3(2.0f, 6.0f, 0.0f), 5.0f, 1.0f);
	the2ndSphere.createSphere(0.5, 32);

	Plane thePlane;
	thePlane.createPlane(5.0f, 5.0f);

	//link variables to shader
	locationMV = glGetUniformLocation(phongShader.programID, "MV");
	locationP = glGetUniformLocation(phongShader.programID, "P");
	locationL = glGetUniformLocation(phongShader.programID, "lightPosition");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 pos = glm::vec3(0.0f);
	while (!glfwWindowShouldClose(window)) {
		//GL calls
		glClearColor(0.0f, 0.1f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glUseProgram(phongShader.programID);

		//Send static variables to vertexshader
		glUniformMatrix4fv(locationP, 1, GL_FALSE, P);
		glUniform3fv(locationL, 1, L);

		setupViewport(window, P);

		//Transform calculations and rendering
		MVstack.push();
		MVstack.translate(0.0f, -2.0f, -10.5f);
			MVstack.push();
			MVstack.rotZ(-0.1);
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				thePlane.render();
			MVstack.pop();
			MVstack.push();
				glfwPollEvents();
				pos = theHandler.calculatePosition(theSphere, window);
				MVstack.translate(pos.x, pos.y, pos.z);
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				theSphere.render();
			MVstack.pop();
		MVstack.pop();


		
		glfwSwapBuffers(window);

	}

	glfwTerminate();

	return 0;
}