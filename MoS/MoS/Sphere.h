#pragma once
#include "entity.h"
class Sphere :
	public Entity
{
public:
	
	// Creates a sphere  
	Sphere(glm::vec3 _pos, float _mass, float _rad);
	~Sphere(void);

	void render();

	float getRadius(){return radius;}

private:
	float radius;
	void display(ostream& os) const;
};