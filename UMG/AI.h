#ifndef _AI_H_
#define _AI_H_

#include <vector>
#include "Physics.h"
#include "Graphics.h"
#include "Render.h"
#include "Projectile.h"
#include "Vehicle.h"
#include "Obstacle.h"
#include "Item.h"
#include "InputDevice.h"


class LOS
	:public Collidable
{
private:
	float A[3];
	float B[3];
	int numElements;

public:
	bool LOS_CLEAR;

	LOS()
	{
		LOS_CLEAR = true;
		numElements = 0;
	}

	void setPoints(float* a, float* b)
	{

		A[0] = a[0];
		A[1] = a[1];
		A[2] = a[2];
		B[0] = b[0];
		B[1] = b[1];
		B[2] = b[2];
	}

	int getNumElements(void)
	{
		return numElements;
	}

	void setNumElements(int n)
	{
		numElements = n;
	}
	void getPoint(int n, float* p)
	{
		float percent = (float)(n) / (float)numElements; //so we don't put points on the vehicles
		float distance = percent * sqrt(pow(A[0]-B[0],2) + pow(A[1]-B[1],2) + pow(A[2]-B[2],2));

		float xDiff = B[0] - A[0];
		float yDiff = 0.5f;
		float zDiff = B[2] - A[2];

		xDiff = A[0] + xDiff * percent;
		zDiff = A[2] + zDiff * percent;

		p[0] = xDiff;
		p[1] = yDiff;
		p[2] = zDiff;

	}

	void getBox(int n, float* b)
	{
	}

	void render(void)
	{
		float p[3];

		for(int x = 0; x<numElements; x++)
		{
			getPoint(x,p);
			//p[1]+=0.5;
			float size = 0.3f;
			
			glBegin(GL_LINES);
				glVertex3f(p[0] - size, p[1]       , p[2]       );
				glVertex3f(p[0] + size, p[1]       , p[2]       );
				glVertex3f(p[0]       , p[1] - size, p[2]       );
				glVertex3f(p[0]       , p[1] + size, p[2]       );
				glVertex3f(p[0]       , p[1]       , p[2] - size);
				glVertex3f(p[0]       , p[1]       , p[2] + size);
			glEnd();
		}
	}
	CollidableType getCollidableType(void)
	{
		return AI_PROBE;
	}
	bool isPhysicsActive()
	{
		return LOS_CLEAR;
	}
	void processCollisions()
	{

	}
	
	void registerCollision(CollidableType, int)
	{
		LOS_CLEAR = false;
	}
	void getCenterOfCollidable(float* c)
	{

	}

};










class AI
	:public Collidable, public Renderable
{
	
	int myVehicleNumber;
	bool didPickItem;
	Item* driveToItem;
	VehicleContainer* vC;
	ProjectileContainer* pC;
	ObstacleContainer* oC;
	ItemContainer* iC;
	InputDevice* iD;
	Physics* physics;

	//LOS checker!
	LOS* los;

	//Where the AI wants to shoot
	float aimPoint[3];

	//Where the AI wants to drive
	float drivePoint[3];

	//The number of the last vehicle this AI targeted
	// (initialized to -1)
	int lastTargetedVehicle;

	//The amount of aggression the AI is feeling
	// (varies between 0.0 and 1.0), starts randomly in that range
	float aggression;

	//The amount of angular inertia that contributes to our change in yaw
	float angularInertia;

	//Array to store per-frame collisions
	bool probeCheckArray[4];

	bool showLOS;

	//tells whether it should worry about this ai
	bool valid;

public:

	CollidableType cT;
	bool physicsEnabled;
	bool collisionDetected;

	AI( int, 
		VehicleContainer*,
		ProjectileContainer*,
		ObstacleContainer*,
		ItemContainer*,
		InputDevice*,
		Physics*);
	~AI(void);

	// The important AI decision-making functions
	void think( void );
	bool findClosestItem( void );
	bool shouldShootThere( float* );
	void shootAtPlayer( void );
	float dist2(float*, float*);
	float dist3(float*, float*);

	//Functions needed to be a Collideable object:
	int getNumElements(void);
	void getBox(int, float*);
	void getPoint(int, float*);
	void getCenterOfCollidable(float*);
	CollidableType getCollidableType(void);
	bool isPhysicsActive( void );
	void registerCollision(CollidableType, int);
	void processCollisions( void );

	//LOS toggle
	void switchLOS( void );

	//Functions to be a Renderable object
	void render(void);


	bool isValid(void)
	{
		return valid;
	}

	void setValid(void)
	{
		valid = true;
	}

	void setInvalid(void)
	{
		valid = false;
	}


};

class AIContainer 
{
	vector<AI*> list;

	//Managers
	Physics* physics;
	Render* renderer;

	//Containers
	VehicleContainer* vC;
	ProjectileContainer* pC;
	ObstacleContainer* oC;
	ItemContainer* iC;
	InputDevice* iD;

	int totalAI;

public:
	AIContainer(Physics* p, 
				Render* r,
				VehicleContainer* v,
				ProjectileContainer* pr,
				ObstacleContainer* o,
				ItemContainer* i,
				InputDevice* input)
	{
		list = vector<AI*>();
		renderer = r;
		physics = p;
		vC = v;
		pC = pr;
		oC = o;
		iC = i;
		iD = input;
	}

	~AIContainer()
	{
		while(list.size()>0)
		{
			AI* a = list.at(0);
			delete a;
			list.erase(list.begin());
		}
	}

	void add(int vehicleNum)
	{
		AI* a = new AI(vehicleNum, vC, pC, oC, iC, iD, physics);
		list.push_back(a);
		physics->add(a);
		renderer->add(a);
		a->setValid();
		a->setCollidableShape(COLLIDABLE_POINT);

		totalAI++;

	}

	void remove( int i )
	{
		AI* a = list.at(i);
		delete a;
		list.erase(list.begin()+i);
	}

	AI* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}

	void thinkOnAll( void )
	{
		unsigned int s = list.size();
		for( unsigned int i = 0; i<s; i++ )
		{
			AI* ai = list.at(i);
			if(ai->isValid())
				ai->think();
		}
	}
	void toggleLOS( void )
	{
		unsigned int s = list.size();
		for( unsigned int i = 0; i<s; i++ )
		{
			list.at(i)->switchLOS();
		}
	}

	int numberofAI(void)
	{
		return totalAI;
	}

	void clean(int i)
	{
		AI* a = list.at(i);
		renderer->remove(a);
		physics->removeAsMoving(a);
		a->setInvalid();
		totalAI--;
	}

	void cleanAll( void )
	{
		unsigned int n = list.size();

		for(unsigned int i = 0; i < n; i++)
		{

			clean(i);

		}
		list.clear();
		totalAI = 0;
	}
};

#endif
