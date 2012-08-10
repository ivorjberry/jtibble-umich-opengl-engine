#ifndef COLLIDABLE_H_
#define COLLIDABLE_H_
#include <vector>
using namespace std;

enum CollidableType{
	NONE,
	PROJECTILE_MINIGUN,
	PROJECTILE_MINE,
	PROJECTILE_MISSILE,
	PROJECTILE_SPEED_MISSILE,
	PROJECTILE_POWER_MISSILE,
	PROJECTILE_BOMB,
	ITEM_MINIGUN,
	ITEM_MINE,
	ITEM_MISSILE,
	ITEM_BOMB,
	ITEM_SPEED_MISSILE,
	ITEM_POWER_MISSILE,
	ITEM_HEALTH,
	VEHICLE,
	OBSTACLE,
	TARGET,
	AI_PROBE,
	CAMERA,
	END_TYPE_FOR_ARRAY_FILL
};

enum CollidableShape{
	COLLIDABLE_BOX,
	COLLIDABLE_POINT
};

class Collidable
{
protected:
	vector<CollidableType> currentCollisions;

public:

	CollidableShape shape;

	virtual void setCollidableShape(CollidableShape s)
	{
		shape = s;
	}

	//Get the number of colliding elements
	virtual int getNumElements(void) = 0;

	//If it's a Colliding Box, get the 24 verteces
	virtual void getBox(int, float*) = 0;

	//If it's a Colliding Point, get the x, y, and z
	virtual void getPoint(int, float*) = 0;

	//Get the center of the object
	virtual void getCenterOfCollidable(float*) = 0;

	//Return the CollidableType associated with this object
	virtual CollidableType getCollidableType(void) = 0;

	//Return if the object is currently part of the physics simulation
	virtual bool isPhysicsActive() = 0;

	//Gets called by physics so that the collidable can process all collisions
	virtual void processCollisions() = 0;
	
	//Called by physics so that it can register a collision & store it until processing
	virtual void registerCollision(CollidableType, int) = 0;

	//Empties the physics detections
	void clearCurrentCollisions(void)
	{
		currentCollisions.clear();
	}

	//Simple
	bool isColliding(void)
	{
		return currentCollisions.size() != 0;
	}

};

#endif