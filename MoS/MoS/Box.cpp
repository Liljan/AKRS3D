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

void Box::render()
{

}

void Box::display(ostream& os) const{
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