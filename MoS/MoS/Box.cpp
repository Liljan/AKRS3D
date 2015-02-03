#include "Box.h"

Box::Box(glm::vec3 _pos, float _mass, glm::vec3 _dim)
{
	position = _pos;
	mass = _mass;
	dim = _dim;
	centerOfMass = position; // The center of mass is in the objects origin as default
	inertia = 1; // temporary

	velocity = { 0, 0, 0 };
	acceleration = { 0, 0, 0 };
	orientation = { 0, 0, 0 };
	angularVelocity = { 0, 0, 0 };
	angularAcceleration = { 0, 0, 0 };
}

Box::~Box(void)
{
	cout << "A box has died." << endl;
}

void Box::createBox(float xSize, float ySize, float zSize)
{
	// Constant data arrays for this simple test.
	// Note, however, that they need to be copied to dynamic arrays
	// in the class. These local variables are not persistent.
	//
	// The data array contains 8 floats per vertex:
	// coordinate xyz, normal xyz, texcoords st
	const GLfloat vertex_array_data[] = {
		-xSize / 2, -ySize / 2, -zSize / 2, // Vertex 0
		xSize / 2, -ySize / 2, -zSize / 2, // Vertex 1
		xSize / 2, ySize / 2, -zSize / 2,  // Vertex 2
		-xSize / 2, ySize / 2, -zSize / 2,  // Vertex 2
		-xSize / 2, -ySize / 2, zSize / 2,  // Vertex 2
		xSize / 2, -ySize / 2, zSize / 2,  // Vertex 2
		xSize / 2, ySize / 2, zSize / 2,  // Vertex 2
		-xSize / 2, ySize / 2, zSize / 2  // Vertex 2
	};

	const GLfloat vertex_array_data[] = {
		//Front (pos z)
		xSize / 2, ySize / 2, zSize / 2 , 0.0f, 0.0f, 1.0f,
		-xSize / 2, -ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		xSize / 2, -ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,

		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		-xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		-xSize / 2, -ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,

		//Back (neg z)
		xSize / 2, ySize / 2, -zSize / 2, 0.0f, 0.0f, -1.0f,
		-xSize / 2, -ySize / 2, -zSize / 2, 0.0f, 0.0f, -1.0f,
		-xSize / 2, ySize / 2, -zSize / 2, 0.0f, 0.0f, -1.0f,

		xSize / 2, ySize / 2, -zSize / 2, 0.0f, 0.0f, -1.0f,
		xSize / 2, -ySize / 2, -zSize / 2, 0.0f, 0.0f, -1.0f,
		-xSize / 2, -ySize / 2, -zSize / 2, 0.0f, 0.0f, -1.0f,

		//Side 1 (pos x)
		xSize / 2, ySize / 2, -zSize / 2, 1.0f, 0.0f, 0.0f,
		xSize / 2, -ySize / 2, zSize / 2, 1.0f, 0.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 1.0f, 0.0f, 0.0f,

		xSize / 2, ySize / 2, -zSize / 2, 1.0f, 0.0f, 0.0f,
		xSize / 2, -ySize / 2, -zSize / 2, 1.0f, 0.0f, 0.0f,
		xSize / 2, -ySize / 2, zSize / 2, 1.0f, 0.0f, 0.0f,

		//Side 2 (neg x)
		-xSize / 2, ySize / 2, zSize / 2, -1.0f, 0.0f, 0.0f,
		-xSize / 2, ySize / 2, -zSize / 2, -1.0f, 0.0f, 0.0f,
		-xSize / 2, -ySize / 2, zSize / 2, -1.0f, 0.0f, 0.0f,

		-xSize / 2, ySize / 2, -zSize / 2, -1.0f, 0.0f, 0.0f,
		-xSize / 2, -ySize / 2, -zSize / 2, -1.0f, 0.0f, 0.0f,
		-xSize / 2, -ySize / 2, zSize / 2, -1.0f, 0.0f, 0.0f,

		//Top (pos y)
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,

		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,

		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,

		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 0.0f, 1.0f,
	};

	//const GLfloat vertices[8][3] = { { -xSize / 2, -1.0, -zSize / 2 }, { xSize / 2, -ySize / 2, -zSize / 2 },
	//{ xSize / 2, ySize / 2, -zSize / 2 }, { -xSize / 2, ySize / 2, -zSize / 2 }, { -xSize / 2, -ySize / 2, zSize / 2 },
//	{ xSize / 2, -ySize / 2, zSize / 2 }, { xSize / 2, ySize / 2, zSize / 2 }, { -xSize / 2, ySize / 2, zSize / 2 } };

	const GLuint index_array_data[] = {
		0, 1, 2
	};

	nverts = 3;
	ntris = 1;

	vertexarray = new GLfloat[8 * nverts];
	indexarray = new GLuint[3 * ntris];
	for (int i = 0; i<8 * nverts; i++) {
		vertexarray[i] = vertex_array_data[i];
	}
	for (int i = 0; i<3 * ntris; i++) {
		indexarray[i] = index_array_data[i];
	}

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	// Generate two buffer IDs
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER,
		8 * nverts * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Texture coordinates
	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 floats (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3 or 6 floats (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

	// Activate the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	// Present our vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		3 * ntris*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the index buffer while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Box::render()
{

}

void Box::display(ostream& os) const
{
	os << "Shape: Box" << endl;
	os << "Dimensions: " << dim.x << ", " << dim.y << " ," << dim.z << endl; 
	os << endl;

	os << "Mass: " << mass << endl;
	os << "Center of mass: " << centerOfMass.x << ", " << centerOfMass.y << ", "<< centerOfMass.z << endl;
	os << "Inertia: " << inertia << endl;
	os << endl;

	os << "Position: " << position.x << ", " << position.y << ", "<< position.z << endl;
	os << "Velocity: " << velocity.x << ", " << velocity.y << ", "<< velocity.z << endl;
	os << "Acceleration: " << acceleration.x << ", " << acceleration.y << ", "<< acceleration.z << endl;
	os << endl;
	
	os << "Orientation: " << orientation.x << ", " << orientation.y << ", "<< orientation.z << endl;
	os << "Angular velocity: " << angularVelocity.x << ", " << angularVelocity.y << ", "<< angularVelocity.z << endl;
	os << "Angular acceleration: " << angularAcceleration.x << ", " << angularAcceleration.y << ", "<< angularAcceleration.z << endl;
	os << endl;

	os << "" << endl;
}