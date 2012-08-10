#include "Camera.h"
#include "math.h"

Camera::Camera(VehicleContainer* vC, int vehicleNum)
{
	//Find theta
	
	vehicleContainer = vC;
	vehicleNumberToFollow = vehicleNum;
	shape = COLLIDABLE_POINT;

	//Writes vehicle position into camera orientation...?
	//v->getPosition(position);

	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

	lookAt[0] = 0.0f;
	lookAt[1] = 0.0f;
	lookAt[2] = 0.0f;

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;

	collidedInPastFrame = false;

	reset();
}


Camera::~Camera(void)
{
}

	void Camera::setPosition(float* p)
	{
		position[0] = p[0];
		position[1] = p[1];
		position[2] = p[2];
	}
	void Camera::getPosition(float* p)
	{
		p[0] = position[0];
		p[1] = position[1];
		p[2] = position[2];
	}

	void Camera::setLookAt(float* p)
	{
		lookAt[0] = p[0];
		lookAt[1] = p[1];
		lookAt[2] = p[2];
	}
	void Camera::getLookAt(float* p)
	{
		p[0] = lookAt[0];
		p[1] = lookAt[1];
		p[2] = lookAt[2];
	}
	void Camera::setUp(float* p)
	{
		up[0] = p[0];
		up[1] = p[1];
		up[2] = p[2];
	}
	void Camera::getUp(float* p)
	{
		p[0] = up[0];
		p[1] = up[1];
		p[2] = up[2];
	}
	float Camera::getFollowDist(void)
	{
		return followDist;
	}
	void Camera::setFollowDist(float d)
	{
		followDist = d;
	}
	float Camera::getAngle(void)
	{
		return angleBehind;
	}
	float Camera::getBaseAngle(void)
	{
		return baseAngle;
	}
	void Camera::setAngle(float a)
	{
		angleBehind = a;
	}
	void Camera::reset(void)
	{
		baseAngle = 0.0f;
		angleBehind = 0.0f;
		followDist = 20.0f;
		collideFollowDist = 20.0f;
		baseFollowDist = 0.0f;
		height = 5.0f;
	}
void Camera::render(void)
{
	glLoadIdentity();
	// Set background clearing color to dark cyan
	glClearColor(0.035f, 0.106f, 0.114f, 1.0f);
	
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Ensure collideFollowDist is within 0 to followDist
	
	//if (collideFollowDist != followDist)
	//collideFollowDist *= 2.0f;

	position[0] = lookAt[0] - (baseFollowDist + followDist) * (float)cos((angleBehind+baseAngle)*M_PI/180.0);
	position[1] = followDist/3.0f;
	position[2] = lookAt[2] - (baseFollowDist + followDist) * (float)sin((angleBehind+baseAngle)*M_PI/180.0);


	gluLookAt(	position[0], position[1], position[2],
				lookAt[0], lookAt[1], lookAt[2],
				up[0], up[1], up[2] );
	
}

void Camera::follow( int i )
{
	Vehicle* v = vehicleContainer->get(i);

	if(v == 0) return;

	//move the camera
	float p[3];
	v->getPosition(p);
	setLookAt(p);

	float o[3];
	v->getOrientation(o);
	baseAngle = o[2];

	float rateOfTurn = v->getRateOfTurn();
	baseAngle += rateOfTurn * 10.0f;

	/*
	up[0] = rateOfTurn * (float)cos(o[2] * M_PI / 180.0f);
	up[2] = rateOfTurn * (float)sin(o[2] * M_PI / 180.0f);
	up[1] = sqrt(up[0]*up[0] + up[2]*up[2]);
	*/
	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
				
	float speed = abs(v->getSpeed());
	float topSpeed = v->getTopSpeed();

	baseFollowDist = 30.0f * speed / topSpeed;

}

void Camera::followCurrent(void)
{

	follow(vehicleNumberToFollow);
}

void Camera::toggleFollowVehicle(void)
{
	vehicleNumberToFollow++;
	if((vehicleNumberToFollow) == vehicleContainer->numberOfVehicles())
		vehicleNumberToFollow = 0;	
}