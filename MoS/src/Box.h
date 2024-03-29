#pragma once
#include "Entity.h"
#include "Utilities.h"

class Box :
	public Entity
{
public:
	Box()
	{
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		vertexarray = NULL;
		indexarray = NULL;
		nverts = 0;
		ntris = 0;
	};

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
	glm::vec3 vertices[8];
	glm::vec3 dim;

	void display(ostream& os) const;
};

