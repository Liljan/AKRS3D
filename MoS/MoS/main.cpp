

#include <iostream>
#include "Utilities.h"
#include "Shader.h"
#include "MatrixStack.hpp"
#include "Box.h"
#include "Sphere.h"
#include "glm\glm.hpp"

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
	Shader phongShader;
	MatrixStack MVstack;
	MVstack.init();
	GLfloat I[16] = { 1.0f, 0.0f, 0.0f, 0.0f
					, 0.0f, 1.0f, 0.0f, 0.0f
					, 0.0f, 0.0f, 1.0f, 0.0f
					, 0.0f, 0.0f, 0.0f, 1.0f };

	GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f
					, 0.0f, 2.42f, 0.0f, 0.0f
					, 0.0f, 0.0f, -1.0f, -1.0f
					, 0.0f, 0.0f, -0.2f, 0.0f };

	GLfloat L[3] = { 10.0f, 1.0f, 1.0f };

	GLint locationMV;
	GLint locationP;
	GLint locationL;

	// start GLEW extension handler
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Do not try and bend the spoon. That's impossible. Instead... only try to realize the truth.", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/*
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	*/

	phongShader.createShader("vertexshader.glsl", "fragmentshader.glsl");



	Box theBox;
	theBox.createBox(1.5f, 0.5f, 0.5f);

	Sphere theSphere;
	theSphere.createSphere(1.0, 32);

	locationMV = glGetUniformLocation(phongShader.programID, "MV");
	locationP = glGetUniformLocation(phongShader.programID, "P");
	locationL = glGetUniformLocation(phongShader.programID, "lightPosition");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	float rot = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.1f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glUseProgram(phongShader.programID);
		//glBindVertexArray(vao);


		glUniformMatrix4fv(locationP, 1, GL_FALSE, P);
		glUniform3fv(locationL, 1, L);

		setupViewport(window, P);

		MVstack.push();
		MVstack.translate(0.0f, 0.0f, -4.5f);
			MVstack.push();
				MVstack.rotZ(rot);
				MVstack.rotY(0.1f*rot);
				MVstack.rotX(0.5f*rot);
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
				theSphere.render();
			MVstack.pop();
		MVstack.pop();

		rot += 0.0001f;

		glfwPollEvents();
		glfwSwapBuffers(window);

	}


	glfwTerminate();

	return 0;
}