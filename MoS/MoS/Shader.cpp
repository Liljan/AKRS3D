#include "Shader.h"


Shader::Shader()
{
	this->programID = 0;

}


Shader::~Shader()
{
	if (programID != 0)
	glDeleteProgram(programID);
}


void Shader::createShader()
{
	const char* vertex_shader =
		"#version 440\n"
		"layout(location = 0) in vec3 Position;"
		"layout(location = 1) in vec3 Normal;"

		"uniform mat4 MV;"
		"uniform mat4 P;"
		//"in vec3 vp;"

		"out vec3 interpolatedNormal;"

		"void main () {"
		"  gl_Position = MV * vec4 (vp, 1.0);"
		"interpolatedNormal = mat3(MV) * Normal;"

		"}";

	const char* fragment_shader =
		"#version 440\n"

		"in vec3 interpolatedNormal;"

		"out vec4 frag_colour;"
		"void main () {"
		"vec3 nNormal = normalize(interpolatedNormal);"
		"float diffuse = max(0.0, nNormal.z);"
		"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);


	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glDeleteShader(vs);   // After successful linking,
	glDeleteShader(fs); // these are no longer needed

	programID = shader_programme;

}