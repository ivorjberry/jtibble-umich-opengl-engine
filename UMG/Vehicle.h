#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <vector>
#include "SystemTimer.h"
#include "Render.h"
#include "Model.h"
#include "Move.h"
#include "Physics.h"

//The only reason this is included is to get
//access to the WeaponType struct
//#include "Projectile.h" 


using namespace std;

class Vehicle
	:public Renderable, public Collidable, public Movable
{
	
	int myVehicleNum;
	float position[3];
	float velocity[3];
	float orientation[3]; //roll, pitch, yaw
	float color[4]; // red, blue, green, alpha (0.0 -> 1.0)

	Model* myModel;
	GLuint textureNumber;

	//stats for driving
	float speed;
	float topSpeed;
	float rateOfTurn;
	float topRateOfTurn;
	float speedDecayFactor;
	float rateOfTurnDecayFactor;
	
	//vector<Projectile>* pL;
	//Weapon* weapon;
//	WeaponInfo wI;
	CollidableType cT;
	SystemTimer timer;

	float hitPoints;
	int ammoRemaining;
	int totalAmmo;
	bool physicsEnabled;
	bool collisionDetected;

	bool shouldBeDeleted;


public:
	Vehicle(int num, Model* myModel, GLuint t);
	~Vehicle(void);
	
	//Motion
	void setPosition(float*);
	void getPosition(float*);
	void setVelocity(float*);
	void getVelocity(float*);
	void setOrientation(float*);
	void getOrientation(float*);
	void setSpeed(float);
	float getSpeed(void);
	void setRateOfTurn(float);
	float getRateOfTurn();
	float getTopSpeed(void);
	void setDecayFactor(float);
	float getDecayFactor(void);
	void setColor(float, float, float);

	//resetting of vehicles
	void resetPlayer(void);

	//Shooting
	bool canShoot(void);
	int getAmmoRemaining(void);
	void setAmmo(int);
	void decrementAmmo(void);
	CollidableType getWeaponType(void);
	void setCollidableType(CollidableType);
	float getTime()
	{
		return timer.elapsedTime();
	}

	void resetTime()
	{
		timer.startTimer();
	}
	

	//Functions needed to be a Renderable object:
	void render(void);

	//Functions needed to be a Collideable object:
	int getNumElements(void);
	void getBox(int, float*);
	void getPoint(int, float*);
	void getCenterOfCollidable(float*);
	CollidableType getCollidableType(void);
	bool isPhysicsActive( void );
	void setPhysicsActive(bool);
	void registerCollision(CollidableType, int);
	void processCollisions( void );


	
	bool shouldBeRemoved(void)
	{
		return shouldBeDeleted;
	}
	void inPlay( void )
	{
		shouldBeDeleted = false;
	}
	void resetHP( void )
	{
		hitPoints = 100.0f;
	}
	void setHP( float hp)
	{
		hitPoints = hp;
	}
	float getHP( void )
	{
		return hitPoints;
	}

	//Functions needed to be a Moveable object:
	void move(void);

};

class VehicleContainer{
	vector<Vehicle*> list;
	Render* renderer;
	Move* mover;
	Physics* physics;
	Model* vehicleModel;
	GLuint textureNum;
public:
	VehicleContainer(Render* r, Move* m, Physics* p, Model* model, GLuint t){
		list = vector<Vehicle*>();
		renderer = r;
		mover = m;
		physics = p;
		vehicleModel = model;
		textureNum = t;
	}
	~VehicleContainer()
	{
		while(list.size()>0)
		{
			Vehicle* v = list.at(0);
			delete v;
			list.erase(list.begin());
		}
	}

	//Creates and allocates new vehicle
	void add( void )
	{
		Vehicle* v = new Vehicle(list.size(), vehicleModel, textureNum);
		list.push_back(v);
		renderer->add(v);
		mover->add(v);
		physics->add(v);
		v->setCollidableShape(COLLIDABLE_BOX);
	}

	void remove( int i )
	{
		Vehicle* v = list.at(i);
		delete v;
		list.erase(list.begin()+i);
	}

	Vehicle* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}
	int numberOfVehicles( void )
	{
		return list.size();
	}
	bool clean( int i )
	{
		Vehicle* v = list.at(i);
		if(v->shouldBeRemoved())
		{
			return true;
		}
		return false;
	}
};

#endif