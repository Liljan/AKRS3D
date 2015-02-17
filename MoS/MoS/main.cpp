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
	// time related variables
	srand((unsigned)time(NULL));
	double timeSinceAction = glfwGetTime();
	double deltaTime;

	float rand1, rand2;

	// GL-related variables

	GLfloat I[16] = { 1.0f, 0.0f, 0.0f, 0.0f
					, 0.0f, 1.0f, 0.0f, 0.0f
					, 0.0f, 0.0f, 1.0f, 0.0f
					, 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f
					, 0.0f, 2.42f, 0.0f, 0.0f
					, 0.0f, 0.0f, -1.0f, -1.0f
					, 0.0f, 0.0f, -0.2f, 0.0f };
	GLfloat L[3] = { 0.0f, 2.0f, 4.0f };
	GLfloat Ca[3] = { 0.0f, 0.0f, 0.0f };
	glm::vec4 li(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 cam(0.0f, 0.0f, 0.0f, 1.0f);
	GLfloat C[3];

	GLint locationMV;
	GLint locationP;
	GLint locationL;
	GLint locationCa;
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

	Camera theCamera(15.0f);

	physicsHandler theHandler;
	
	objectList.push_back(new Plane(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, glm::vec2(5.0f, 5.0f)));
	objectList.push_back(new Plane(glm::vec3(4.0f, 3.0f, 0.0f), 5.0f, glm::vec2(5.0f, 5.0f)));
	objectList.push_back(new Plane(glm::vec3(0.0f, 8.0f, 0.0f), 5.0f, glm::vec2(5.0f, 5.0f)));
	//objectList.push_back(new Box(glm::vec3(0.0f, 3.0f,0.0f),2.0f , glm::vec3(1.0f, 1.0f, 1.0f) ));
	//objectList.push_back(new Sphere(glm::vec3(0.0f, 5.0f, 0.0f), 5.0f, 0.5f));
	//objectList.push_back(new Sphere(glm::vec3(0.0f, 8.0f, 0.0f), 5.0f, 0.5f));

	//link variables to shader
	locationMV = glGetUniformLocation(phongShader.programID, "MV");
	locationP = glGetUniformLocation(phongShader.programID, "P");
	locationL = glGetUniformLocation(phongShader.programID, "lightPosition");
	locationCa = glGetUniformLocation(phongShader.programID, "cameraPosition");
	locationColor = glGetUniformLocation(phongShader.programID, "objectColor");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Entity *oPointer;
	vector<Entity*> *vPointer;
	vPointer = &objectList;

	glm::mat4 transform;

	//oPointer = &theSphere;
	glm::vec3 pos = glm::vec3(0.0f);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// generate random coordinates
		rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		li = glm::vec4(0.0, 5.0, 0.0, 1.0);
		cam = glm::vec4(0.0, 0.0, 0.0, 1.0);


		deltaTime = glfwGetTime() - timeSinceAction;

		// Add balls to scene
		if (glfwGetKey(window, GLFW_KEY_O)) {
			objectList.push_back(new Sphere(glm::vec3(0.0f*rand1, 8.0f, 0.0f*rand2), 5.0f, 0.5f));
			std::cout << "Number of objects: " << objectList.size() << std::endl;
		}
		// Remove one ball
		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) && objectList.size() > 1  && deltaTime > 0.1) {
			objectList.erase(objectList.end() - 1);
			std::cout << "Number of objects: " << objectList.size() << std::endl;
			
			timeSinceAction = glfwGetTime();
		}
		// Flush your balls
		if (glfwGetKey(window, GLFW_KEY_DELETE) )
		{
			objectList.clear();
			objectList.push_back(new Plane(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, glm::vec2(5.0f, 5.0f)));
			system("cls");
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
		MVstack.translate(glm::vec3(0.0f, -3.0f, -10.0f));
			MVstack.translate(glm::vec3(0.0f, 0.0f, -theCamera.getRad() ));
			MVstack.rotX(theCamera.getTheta());
			MVstack.rotY(theCamera.getPhi());

			transform = glm::make_mat4(MVstack.getCurrentMatrix());

			//glm::inverse(glm::mat4(MVstack.getCurrentMatrix()));
			li = glm::inverse(transform)*li;
			cam = glm::inverse(transform)*cam;

			Ca[0] = cam.x;
			Ca[1] = cam.y;
			Ca[2] = cam.z;
			L[0] = li.x;
			L[1] = li.y;
			L[2] = li.z;
			glUniform3fv(locationL, 1, L);
			glUniform3fv(locationCa, 1, Ca);

			if (!glfwGetKey(window, GLFW_KEY_X))
			{
				theHandler.calculateMovement(vPointer, window);
				theHandler.resolveCollision(vPointer);
			}
			for (int i = 0; i < vPointer->size(); i++)
			{
				oPointer = objectList[i];

				MVstack.push();

					MVstack.translate(oPointer->getPosition());
					MVstack.rotAxis(oPointer->getOrientation(), oPointer->getAngularPosition());

					glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());

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