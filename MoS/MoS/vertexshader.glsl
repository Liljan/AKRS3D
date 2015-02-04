#version 400
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 MV;
uniform mat4 P;

uniform vec3 lightPosition;

out vec3 interpolatedColor;
out vec3 interpolatedNormal;

struct lightSource
{
	vec3 position;
	vec3 diffuse;
	vec3 specular;
};

lightSource light0 = lightSource(
vec3(lightPosition),
vec3(1.0, 1.0, 1.0),
vec3(1.0, 1.0, 1.0)
);

// temp, will be uniform vec3
vec3 objectColor(0.7, 0.7, 0.7);

void main () 
{
    // Ambient, diffuse and specular constants. nS is a notation on shininess (higher = more shiny)
	float kA = 0.3;
	float kS = 0.7;
	float kD = 0.8;
	float nS = 15;

	gl_Position = P*MV * vec4 (Position, 1.0);
	interpolatedNormal = mat3(MV) * Normal;

	vec3 reflection = reflect(normalize(-light0.position), normalize(interpolatedNormal));
	vec3 cameraPosition = vec3(0.0, 0.0, 1.0);
	
	float dLight = dot(normalize(interpolatedNormal), normalize(light0.position));
	float sLight = dot(reflection, cameraPosition);
	dLight = max(0, dLight);
	sLight = max(0, sLight);

    // totalLightinPixel = (Ambient + Diffuse + Specular light)*objectColor
	interpolatedColor = kA * objectColor + kS * pow(sLight, nS) * objectColor + dLight * kD * objectColor;

}