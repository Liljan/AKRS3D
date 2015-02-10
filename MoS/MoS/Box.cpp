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
	/*
	const GLfloat vertex_array_data[] = {
		//Front (pos z)
		xSize / 2, ySize / 2.0f, zSize / 2.0f , 0.0f, 0.0f, 1.0f,
		-xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f, 0.0f, 0.0f, 1.0f,
		xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f, 0.0f, 0.0f, 1.0f,

		xSize / 2.0f, ySize / 2.0f, zSize / 2.0f, 0.0f, 0.0f, 1.0f,
		-xSize / 2.0f, ySize / 2.0f, zSize / 2.0f, 0.0f, 0.0f, 1.0f,
		-xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f, 0.0f, 0.0f, 1.0f,
		
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
		-xSize / 2, ySize / 2, -zSize / 2, 0.0f, 1.0f, 0.0f,
		-xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,

		-xSize / 2, ySize / 2, -zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, zSize / 2, 0.0f, 1.0f, 0.0f,
		xSize / 2, ySize / 2, -zSize / 2, 0.0f, 1.0f, 0.0f,

		//Bottom (neg y)
		xSize / 2, -ySize / 2, zSize / 2, 0.0f, -1.0f, 0.0f,
		-xSize / 2, -ySize / 2, -zSize / 2, 0.0f, -1.0f, 0.0f,
		-xSize / 2, -ySize / 2, zSize / 2, 0.0f, -1.0f, 0.0f,

		xSize / 2, -ySize / 2, -zSize / 2, 0.0f, -1.0f, 0.0f,
		-xSize / 2, -ySize / 2, -zSize / 2, 0.0f, -1.0f, 0.0f,
		xSize / 2, -ySize / 2, zSize / 2, 0.0f, -1.0f, 0.0f,
		
	};

	const GLuint index_array_data[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
	};
	
	*/

	GLfloat vertex_array_data[] = {
		-xSize / 2.0f, -xSize / 2.0f, xSize / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  //1 - 0
		xSize / 2.0f, -xSize / 2.0f, xSize / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //2 - 1
		xSize / 2.0f, xSize / 2.0f, xSize / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   //3 - 2
		-xSize / 2.0f, xSize / 2.0f, xSize / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  //4 - 3 
		-xSize / 2.0f, -xSize / 2.0f, -xSize / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,//5 - 4 
		xSize / 2.0f, -xSize / 2.0f, -xSize / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //6 - 5
		xSize / 2.0f, xSize / 2.0f, -xSize / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  //7 - 6 
		-xSize / 2.0f, xSize / 2.0f, -xSize / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //8 - 7

		-xSize / 2.0f, -xSize / 2.0f, xSize / 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //1 - 8
		xSize / 2.0f, -xSize / 2.0f, xSize / 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //2 - 9
		xSize / 2.0f, xSize / 2.0f, xSize / 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   //3 - 10
		-xSize / 2.0f, xSize / 2.0f, xSize / 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //4 - 11
		-xSize / 2.0f, -xSize / 2.0f, -xSize / 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //5 - 12
		xSize / 2.0f, -xSize / 2.0f, -xSize / 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //6 - 13
		xSize / 2.0f, xSize / 2.0f, -xSize / 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //7 - 14
		-xSize / 2.0f, xSize / 2.0f, -xSize / 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //8 - 15

		-xSize / 2.0f, -xSize / 2.0f, xSize / 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //1 - 16
		xSize / 2.0f, -xSize / 2.0f, xSize / 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //2 - 17
		xSize / 2.0f, xSize / 2.0f, xSize / 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   //3 - 18
		-xSize / 2.0f, xSize / 2.0f, xSize / 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //4 - 19
		-xSize / 2.0f, -xSize / 2.0f, -xSize / 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,//5 - 20
		xSize / 2.0f, -xSize / 2.0f, -xSize / 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //6 - 21
		xSize / 2.0f, xSize / 2.0f, -xSize / 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //7 - 22
		-xSize / 2.0f, xSize / 2.0f, -xSize / 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //8 - 23
	};

	static const GLuint index_array_data[] = {
		0, 1, 2, //Z-positiv/nära
		0, 2, 3, //
		7, 5, 4, //Z-negativ/borta
		7, 6, 5, //
		8, 12, 9, //X-negativ/vänster
		13, 9, 12, //
		10, 14, 11, //X-positiv/höger
		11, 14, 15, //
		17, 21, 18, //Y-positiv/ovan
		18, 21, 22, //
		16, 19, 23, //Y-negativ/under
		20, 16, 23, //  
	};
	nverts = 24;
	ntris = 12;

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
	//glEnableVertexAttribArray(2); // Texture coordinates
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
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
	//	8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

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
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * ntris, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);
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