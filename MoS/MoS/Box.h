#pragma once
#include "entity.h"
#include "Utilities.h"

class Box :
	public Entity
{
public:
	Box(glm::vec3 _pos, float _mass, glm::vec3 _dim);
	~Box(void);

	void createBox(float xSize, float ySize, float zSize);

	void render();
	glm::vec3 getDim(){return dim;}

private:
	GLuint vao;          // Vertex array object, the main handle for geometry
	int nverts; // Number of vertices in the vertex array
	int ntris;  // Number of triangles in the index array (may be zero)
	GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
	GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
	GLfloat *vertexarray; // Vertex array on interleaved format: x y z nx ny nz s t
	GLuint *indexarray;   // Element index array

	glm::vec3 dim;

/*	float verts[42] = {
		-0.5*w, 0.5*h, -0.5*d, 
		0.5*w, 0.5*h, -0.5*d, 
		0.5*w, 0.5*h, 0.5*d, 
		-0.5*w, 0.5*h, 0.5*d, 
		-0.5*w, -0.5*h, 0.5*d, 
		0.5*w, -0.5*h, 0.5*d, 
		-0.5*w, -0.5*h, -0.5*d,
		0.5*w, -0.5*h, -0.5*d,
		-0.5*w, -0.5*h, -0.5*d,
		0.5*w, -0.5*h, -0.5*d,
		-0.5*w, -0.5*h, -0.5*d,
		-0.5*w, -0.5*h, 0.5*d, 
		0.5*w, -0.5*h, -0.5*d, 
		0.5*w, -0.5*h, 0.5*d
	};
	*/
/*	int polys[] = {
		0, 3, 1, 1, 3, 2,    // TOP
		3, 4, 2, 2, 4, 5,    // FRONT
		4, 6, 5, 5, 6, 7,    // BOTTOM
		8, 0, 9, 9, 0, 1,    // BACK
		10, 11, 0, 0, 11, 3, // LEFT
		1, 2, 12, 12, 2, 13  // RIGHT
	};
	*/
	//render cube
	

	void display(ostream& os) const;
};

