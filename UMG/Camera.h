#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vehicle.h"
#include "Render.h"
#include "Physics.h"

class Camera : public Renderable, public Collidable
{
	VehicleContainer* vehicleContainer;
	float position[3];
	float lookAt[3];
	float up[3];

	float followDist;
	float collideFollowDist; // this will be between 0 and followDist
	float baseFollowDist;
	float angleBehind;
	float baseAngle;
	float height;

	int vehicleNumberToFollow;

	bool collidedInPastFrame;

public:

    Camera(VehicleContainer* vC, int vehicleNum);
	~Camera(void);

	void setPosition(float*);
	void getPosition(float*);
	void setLookAt(float*);
	void getLookAt(float*);
	void getUp(float*);
	void setUp(float*);
	void render(void);
	void follow(int);
	void followCurrent(void);
	void setFollowDist(float);
	float getFollowDist(void);
	void setAngle(float);
	float getAngle(void);
	float getBaseAngle(void);
	void reset(void);
	void toggleFollowVehicle(void);

	//For Collidable
	int getNumElements(void)
	{
		return 1;
	}
	void getBox(int n, float* p) { }
	void getPoint(int n, float* p)
	{
		p[0] = position[0];
		p[1] = position[1];
		p[2] = position[2];
	}
	void getCenterOfCollidable(float* p) {}
	CollidableType getCollidableType(void)
	{
		return CAMERA;
	}
	bool isPhysicsActive(void)
	{
		return true;
	}
	void processCollisions(void)
	{/*
		if(isColliding())
		{
			collidedInPastFrame = true;
			collideFollowDist /= 2.0f;
			clearCurrentCollisions();
		}
		else
		{
			if(!collidedInPastFrame == true) 
			{ 
				collideFollowDist *= 2.0f;
				collidedInPastFrame = false;
			}
		}
		if(collideFollowDist > followDist) collideFollowDist = followDist;
		if(collideFollowDist < 0.0f) collideFollowDist = 0.0f;	*/
	}
	void registerCollision(CollidableType c, int n)
	{
		//currentCollisions.push_back(c);
	}

};

#endif