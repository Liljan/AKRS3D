#include <stdio.h>  // For file input in soupReadOBJ()
#include <stdlib.h> // For malloc() and free()
#include <math.h>   // For sin() and cos() in soupCreateSphere()
#include <string.h> // For strcmp()

#include "TriangleSoup.hpp"

/* Constructor: initialize a TriangleSoup object to all zeros */
TriangleSoup::TriangleSoup() {
	vao = 0;
	vertexbuffer = 0;
	indexbuffer = 0;
	vertexarray = NULL;
	indexarray = NULL;
	nverts = 0;
	ntris = 0;
}


/* Destructor: clean up allocated data in a TriangleSoup object */
TriangleSoup::~TriangleSoup() {
    clean();
};


void TriangleSoup::clean() {

	if(glIsVertexArray(vao)) {
		glDeleteVertexArrays(1, &vao);
	}
	vao = 0;

	if(glIsBuffer(vertexbuffer)) {
		glDeleteBuffers(1, &vertexbuffer);
	}
	vertexbuffer = 0;

	if(glIsBuffer(indexbuffer)) {
		glDeleteBuffers(1, &indexbuffer);
	}
	indexbuffer = 0;

	if(vertexarray) {
		delete[] vertexarray;
	}
	if(indexarray) 	{
		delete[] indexarray;
	}
	nverts = 0;
	ntris = 0;
}


/* Create a simple box geometry */
void TriangleSoup::createBox(float xsize, float ysize, float zsize) {
	float vertices[8][3] = { { -xsize / 2, -1.0, -zsize / 2 }, { xsize / 2, -ysize / 2, -zsize / 2 },
	{ xsize / 2, ysize / 2, -zsize / 2 }, { -xsize / 2, ysize / 2, -zsize / 2 }, { -xsize / 2, -ysize / 2, zsize / 2 },
	{ xsize / 2, -ysize / 2, zsize / 2 }, { xsize / 2, ysize / 2, zsize / 2 }, { -xsize / 2, ysize / 2, zsize / 2 } };

	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[1]);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[6]);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[3]);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[5]);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[7]);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[4]);
	glEnd();
	/*
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
	*/
};


/*
 * createSphere(float radius, int segments)
 *
 * Create a TriangleSoup objectwith vertex and index arrays
 * to draw a textured sphere with normals.
 * Increasing the parameter 'segments' yields more triangles.
 * The vertex array is on interleaved format. For each vertex, there
 * are 8 floats: three for the vertex coordinates (x, y, z), three
 * for the normal vector (n_x, n_y, n_z) and finally two for texture
 * coordinates (s, t). The arrays are allocated by malloc() inside the
 * function and should be disposed of using free() when they are no longer
 * needed, e.g with the function soupDelete().
 *
 * Author: Stefan Gustavson (stegu@itn.liu.se) 2014.
 * This code is in the public domain.
 */
void TriangleSoup::createSphere(float radius, int segments) {
    float M_PI = 3.14159265358979323846;
	int i, j, base, i0;
	float x, y, z, R;
	double theta, phi;
	int vsegs, hsegs;
	int stride = 8;

	// Delete any previous content in the TriangleSoup object
	clean();

	vsegs = segments;
	if (vsegs < 2) vsegs = 2;
	hsegs = vsegs * 2;
	nverts = 1 + (vsegs-1) * (hsegs+1) + 1; // top + middle + bottom
	ntris = hsegs + (vsegs-2) * hsegs * 2 + hsegs; // top + middle + bottom
	vertexarray = new float[nverts * 8];
	indexarray = new GLuint[ntris * 3];

	// The vertex array: 3D xyz, 3D normal, 2D st (8 floats per vertex)
	// First vertex: top pole (+z is "up" in object local coords)
	vertexarray[0] = 0.0f;
	vertexarray[1] = 0.0f;
	vertexarray[2] = radius;
	vertexarray[3] = 0.0f;
	vertexarray[4] = 0.0f;
	vertexarray[5] = 1.0f;
	vertexarray[6] = 0.5f;
	vertexarray[7] = 1.0f;
	// Last vertex: bottom pole
	base = (nverts-1)*stride;
	vertexarray[base] = 0.0f;
	vertexarray[base+1] = 0.0f;
	vertexarray[base+2] = -radius;
	vertexarray[base+3] = 0.0f;
	vertexarray[base+4] = 0.0f;
	vertexarray[base+5] = -1.0f;
	vertexarray[base+6] = 0.5f;
	vertexarray[base+7] = 0.0f;
	// All other vertices:
	// vsegs-1 latitude rings of hsegs+1 vertices each
	// (duplicates at texture seam s=0 / s=1)
	for(j=0; j<vsegs-1; j++) { // vsegs-1 latitude rings of vertices
		theta = (double)(j+1)/vsegs*M_PI;
		z = cos(theta);
		R = sin(theta);
		for (i=0; i<=hsegs; i++) { // hsegs+1 vertices in each ring (duplicate for texcoords)
        	phi = (double)i/hsegs*2.0*M_PI;
        	x = R*cos(phi);
        	y = R*sin(phi);
			base = (1+j*(hsegs+1)+i)*stride;
    		vertexarray[base] = radius*x;
    		vertexarray[base+1] = radius*y;
    		vertexarray[base+2] = radius*z;
    		vertexarray[base+3] = x;
    		vertexarray[base+4] = y;
    		vertexarray[base+5] = z;
    		vertexarray[base+6] = (float)i/hsegs;
    		vertexarray[base+7] = 1.0f-(float)(j+1)/vsegs;
		}
	}

	// The index array: triplets of integers, one for each triangle
	// Top cap
	for(i=0; i<hsegs; i++) {
    	indexarray[3*i]=0;
		indexarray[3*i+1]=1+i;
		indexarray[3*i+2]=2+i;
	}
	// Middle part (possibly empty if vsegs=2)
	for(j=0; j<vsegs-2; j++) {
		for(i=0; i<hsegs; i++) {
			base = 3*(hsegs + 2*(j*hsegs + i));
			i0 = 1 + j*(hsegs+1) + i;
			indexarray[base] = i0;
			indexarray[base+1] = i0+hsegs+1;
			indexarray[base+2] = i0+1;
			indexarray[base+3] = i0+1;
			indexarray[base+4] = i0+hsegs+1;
			indexarray[base+5] = i0+hsegs+2;
		}
	}
	// Bottom cap
	base = 3*(hsegs + 2*(vsegs-2)*hsegs);
	for(i=0; i<hsegs; i++) {
		indexarray[base+3*i] = nverts-1;
		indexarray[base+3*i+1] = nverts-2-i;
		indexarray[base+3*i+2] = nverts-3-i;
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
		8*nverts * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Texture coordinates
	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3, 6 (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8*sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat))); // texcoords

 	// Activate the index buffer
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
 	// Present our vertex indices to OpenGL
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	 	3*ntris*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the buffers while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

};

/* Print data from a TriangleSoup object, for debugging purposes */
void TriangleSoup::print() {
     int i;

     printf("TriangleSoup vertex data:\n\n");
     for(i=0; i<nverts; i++) {
         printf("%d: %8.2f %8.2f %8.2f\n", i,
         vertexarray[8*i], vertexarray[8*i+1], vertexarray[8*i+2]);
     }
     printf("\nTriangleSoup face index data:\n\n");
     for(i=0; i<ntris; i++) {
         printf("%d: %d %d %d\n", i,
         indexarray[3*i], indexarray[3*i+1], indexarray[3*i+2]);
     }
};

/* Print information about a TriangleSoup object (stats and extents) */
void TriangleSoup::printInfo() {
     int i;
     float x, y, z, xmin, xmax, ymin, ymax, zmin, zmax;

     printf("TriangleSoup information:\n");
     printf("vertices : %d\n", nverts);
     printf("triangles: %d\n", ntris);
     xmin = xmax = vertexarray[0];
     ymin = ymax = vertexarray[1];
     zmin = zmax = vertexarray[2];
     for(i=1; i<nverts; i++) {
         x = vertexarray[8*i];
         y = vertexarray[8*i+1];
         z = vertexarray[8*i+2];
//         printf("x y z : %8.2f %8.2f %8.2f\n", x, y, z);
         if(x<xmin) xmin = x;
         if(x>xmax) xmax = x;
         if(y<ymin) ymin = y;
         if(y>ymax) ymax = y;
         if(z<zmin) zmin = z;
         if(z>zmax) zmax = z;
     }
     printf("xmin: %8.2f\n", xmin);
     printf("xmax: %8.2f\n", xmax);
     printf("ymin: %8.2f\n", ymin);
     printf("ymax: %8.2f\n", ymax);
     printf("zmin: %8.2f\n", zmin);
     printf("zmax: %8.2f\n", zmax);
};

/* Render the geometry in a TriangleSoup object */
void TriangleSoup::render() {

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * ntris, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);

};

/*
 * private
 * printError() - Signal an error.
 * Simple printf() to console for portability.
 */
void TriangleSoup::printError(const char *errtype, const char *errmsg) {
  fprintf(stderr, "%s: %s\n", errtype, errmsg);
};
