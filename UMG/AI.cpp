#ifndef _AI_CPP_
#define _AI_CPP_

#include "AI.h"
//#include "Weapon.h"
#include "Settings.h"


AI::AI( int vehicleNum,
		VehicleContainer* v,
		ProjectileContainer* p,
		ObstacleContainer* o,
		ItemContainer* i,
		InputDevice* input,
		Physics* ph)
{
	myVehicleNumber = vehicleNum;
	vC = v; //Copy Container pointers
	pC = p;
	oC = o;
	iC = i;
	iD = input;
	physics = ph;

	los = new LOS();

	aimPoint[0] = 0.0f;
	aimPoint[1] = 0.0f;
	aimPoint[2] = 0.0f;

	drivePoint[0] = 0.0f;
	drivePoint[1] = 0.0f;
	drivePoint[2] = 0.0f;

	//set the LOS so we don't get errors when rendering for the first time
	float pTemp[3];
	v->get(myVehicleNumber)->getPosition(pTemp);
	los->setPoints(aimPoint, pTemp);

	lastTargetedVehicle = -1;

	aggression = rand()%11 / 100.0f; // range 0.0 - 1.0

	physicsEnabled = true;

	angularInertia = 0.0f;
	
	probeCheckArray[0] = false;
	probeCheckArray[1] = false;
	probeCheckArray[2] = false;
	probeCheckArray[3] = false;

	showLOS = false;

	didPickItem = false;
	driveToItem = NULL;
}


AI::~AI(void)
{
}

void AI::render(void)
{

	if(los->LOS_CLEAR)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	Vehicle* v = vC->get(myVehicleNumber);
	float myPos[3];
	v->getPosition(myPos);
	

	Vehicle* playerV = vC->get(0);

	if(showLOS){
		float playerPos[3];
		playerV->getPosition(playerPos);
		los->setPoints(myPos,playerPos);
		los->render();
	}

	//make small line pointing towards the point they are travelling towards
	//float towardsDP[3];
	//towardsDP[0] = myPos[0] + (drivePoint[0] - myPos[0]) / 1000.0f;
	//towardsDP[0] = (drivePoint[0] - myPos[0]) / 100.0f;
	//towardsDP[2] = myPos[2] + (drivePoint[2] - myPos[2]) / 1000.0f;


	/*if(v->getAmmoRemaining() == 0)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
			glVertex3f(myPos[0], 1.8f, myPos[2]);
			glVertex3f(towardsDP[0], 1.8f, towardsDP[2]);
		glEnd();
	}*/

	//Draw health bar behind player vehicle
	float orientation[3];
	float position[3];
	playerV->getOrientation(orientation);
	playerV->getPosition(position);

	glPushMatrix();
		glLoadIdentity();	
		float hitPoints = v->getHP();
		//health bar
		glTranslatef(-6.0f, 3.0f + myVehicleNumber * 0.25f, -8.0f);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.5f + 0.5f*hitPoints/100.0f);
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			PrimitiveCube healthRemainingCube(1.0f, false);
			glScalef(0.1f, 0.1f, 1.02f*hitPoints/100.0f);
			healthRemainingCube.render();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.5f - 0.5f*(1 - hitPoints/100.0f));
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			PrimitiveCube healthDifferenceCube(1.0f, false);
			glScalef(0.1f, 0.1f, (1.0f - hitPoints/100.0f));
			healthDifferenceCube.render();
		glPopMatrix();

	glPopMatrix();


	
}

//The all-important function of AI

void AI::think( void )
{
	Vehicle* v = vC->get(myVehicleNumber);
	float myPos[3];
	v->getPosition(myPos);


	float orientation[3];
	v->getOrientation(orientation);

	//turn towards our driving point
	//Calculate difference between current heading (yaw) and where it needs to go
	float diffX = drivePoint[0] - myPos[0];
	float diffZ = drivePoint[2] - myPos[2];
	double tmpAngle = atan2( diffZ , diffX );
	float angleToAimPoint = (float)(tmpAngle * 180.0f / M_PI);
			
	if(angleToAimPoint < 0) 
		angleToAimPoint = 360 + angleToAimPoint;

	//angle increases clockwise from 0 (straight ahead) to 180 (behind) to 360(ahead)
	while(orientation[2] < 0.0f)
		orientation[2]+=360.0f;

	while(orientation[2] > 360.0f)
		orientation[2] -= 360.0f;



	float angleDifference = angleToAimPoint - orientation[2];

	while(angleDifference < 0.0f)
		angleDifference += 360.0f;

	while(angleDifference > 360.0f)
		angleDifference -= 360.0f;
			
	bool turnRight;
	if(angleDifference < 180.0f) turnRight = true;
	else turnRight = false;
		
	
	float amount;
	if(dist3(myPos, drivePoint) > 20.0f)
		amount = 2.0f;
	else
		amount = 3.0f;

	if(turnRight)
		orientation[2]+=amount;
	else
		orientation[2]-=amount;

	if(angleDifference > 5.0f && angleDifference < 355.0f)
	{
		v->setOrientation(orientation);
	}


	if(v->getAmmoRemaining() != 0)
		didPickItem = false;
	
	CollidableType t = v->getWeaponType();
	if( t == NONE ||
		v->getAmmoRemaining() == 0)
	{
		//driveToItem = NULL;
		//didPickItem = false;
		findClosestItem();
	}
	else
	{
		Vehicle* playerV = vC->get(0);
		if(playerV->getHP() <= 0.0f) 
			return;

		playerV->getPosition(drivePoint);
#ifdef AI_AGGRESSION_ENABLED
		shootAtPlayer();
#endif
	}


	//los->LOS_CLEAR = true;



	
	/////////////////////////
	//PLAN OF ACTION:
	//
	// 
	// 1. AI Input Device:
	//		- Needs to let AI control Vehicle
	//		- Needs to let AI fire weapon and specify aimPoint
	//		
	// 2. AI Pathfinding
	//		- Hook the AI into the physics system and specify multiple (2) AI_PROBEs that will check for collisions with obstacles
	//		- If a collision is detected, turn the vehicle (this will allow for rudimentary collision avoidance
	//		- If a drive-to point is specified, turn the vehicle towards it (?)
	// 
	// 3. AI Item Pick-up
	//		- If an item is nearby, turn the vehicle towards it (if we need it, use some sort of judgment)
	//		- Investigate possibility of picking an item up based on type and range to target
	//
	// 4. Shooting
	//		- If the AI sees an opponent, have it set the aimpoint appropriately and SHOOT
	//		- Write line-of-sight checking code that hooks into physics system to see if given world location is visible
	//		- Handle placing mines somehow. This may be tricky.
	//
	// 5. Pursuit / Evasion
	//		- If pursuing, and we see a target/enemy, predict their future (occluded) location by knowing their position and velocity
	//		- If evading, try to figure out a go-to point that will get us behind an obstacle (this may be hard to implement)


}

bool AI::findClosestItem( void )
{
	if (didPickItem && driveToItem != NULL) {
		if(driveToItem->isPhysicsActive()) {
			return true;
		}
	}

	int totalItems = iC->getNum();
	
	if(totalItems == 0)
	{
		return false;
	}
	
	Vehicle* v = vC->get(myVehicleNumber);
	float myPos[3];
	v->getPosition(myPos);


	float p[3];
	int x = rand()%totalItems;
	driveToItem = iC->get(x);
	if(driveToItem->isPhysicsActive()) {
		driveToItem->getPosition(p);
		drivePoint[0] = p[0];
		drivePoint[1] = p[1];
		drivePoint[2] = p[2];
		didPickItem = true;
	} else {
		didPickItem = false;
	}
	return true;

}

float AI::dist2(float* a, float*b)
{
	return sqrt(pow(a[0]-b[0],2)+pow(a[1]-b[1],2));
}
float AI::dist3(float* a, float*b)
{
	return sqrt(pow(a[0]-b[0],2)+pow(a[1]-b[1],2)+pow(a[2]-b[2],2));
}
	

bool AI::shouldShootThere( float* p)
{
	return true;
}

void AI::shootAtPlayer( void )
{
	//Start by getting the position of the AI's vehicle
	Vehicle* v = vC->get(myVehicleNumber);
	float p[3]; 
	v->getPosition(p);

	//Get the player vehicle's location and store it into the aimPoint
	Vehicle* pV = vC->get(0);
	pV->getPosition(aimPoint);
		

	//How far is it?
	float dist = sqrt(pow(aimPoint[0]-p[0],2) + pow(aimPoint[2]-p[2],2));
	
	//How long will it take the current weapon to get there from here?
	CollidableType weaponType = v->getWeaponType();

	float projectileSpeed = 0.0f;

	if(weaponType == ITEM_MINIGUN)
	{
		projectileSpeed = MINIGUN_SPEED;
	}
	else if( weaponType == ITEM_BOMB)
	{
		projectileSpeed = BOMB_SPEED;
	}
	else if(weaponType == ITEM_MISSILE)
	{
		projectileSpeed = MISSILE_SPEED;
	}
	else if(weaponType == ITEM_SPEED_MISSILE)
	{
		projectileSpeed = SMISSILE_SPEED;
	}
	else if(weaponType == ITEM_POWER_MISSILE)
	{
		projectileSpeed = PMISSILE_SPEED;
	}


	
	float travelTime = dist / (projectileSpeed * 30.0f);

	//Get the player vehicle's velocity
	float pVel[3];
	pV->getVelocity(pVel);
	


	//Find if we can see the player's vehicle (aimPoint)

	los->setNumElements((int)dist / 20);
	los->setPoints(p,aimPoint);
	los->LOS_CLEAR = true;
	physics->LOS(los);

	bool playerVisible = los->LOS_CLEAR;

	CollidableType weapon = v->getWeaponType();

	if(playerVisible)
	{
		if( weapon == ITEM_MISSILE ||
			weapon == ITEM_BOMB ||
			weapon == ITEM_MINIGUN ||
			weapon == ITEM_SPEED_MISSILE ||
			weapon == ITEM_POWER_MISSILE)
		{
			aimPoint[0] += travelTime*30.0f*pVel[0];
			aimPoint[2] += travelTime*30.0f*pVel[2];
			iD->FireWeapon(myVehicleNumber, aimPoint);
		}
		else if(weapon == ITEM_MINE)
		{
			if(dist <= 100.0f || v->getTime() >= 20.0f) //wait 8 seconds,
			{
				if(rand()%30 == 0) //then, once per frame, have a 1:90 chance of dropping a mine
				{
					iD->FireWeapon(myVehicleNumber, aimPoint);
					v->resetTime();
				}
			}
		}
	}
}

int AI::getNumElements(void)
{
	return 4;
}

void AI::getPoint(int n, float* center)
{	
	float distanceOutFront = 20.0;

	//get vehicle info
	Vehicle* v = vC->get(myVehicleNumber);
	float p[3];
	v->getPosition(p);
	float o[3];
	v->getOrientation(o);

	//float centerX, centerY, centerZ;

	if(n == 0)
	{
		center[0] = p[0] + distanceOutFront * (float)cos( (o[2]-10) * M_PI / 180.0 );
		center[1] = p[1] + 0.5f;
		center[2] = p[2] + distanceOutFront * (float)sin( (o[2]-10) * M_PI / 180.0 );
	}
	else if(n==1)
	{	
		center[0] = p[0] + distanceOutFront * (float)cos( (o[2]+10) * M_PI / 180.0 );
		center[1] = p[1] + 0.5f;
		center[2] = p[2] + distanceOutFront * (float)sin( (o[2]+10) * M_PI / 180.0 );
	}
	else if(n==2)
	{
		center[0] = p[0] + 3.0f * (float)cos( (o[2]-90) * M_PI / 180.0 );
		center[1] = p[1] + 0.5f;
		center[2] = p[2] + 3.0f * (float)sin( (o[2]-90) * M_PI / 180.0 );
	}
	else if(n==3)
	{
		center[0] = p[0] + 3.0f * (float)cos( (o[2]+90) * M_PI / 180.0 );
		center[1] = p[1] + 0.5f;
		center[2] = p[2] + 3.0f * (float)sin( (o[2]+90) * M_PI / 180.0 );
	}
}

void AI::getBox(int n, float* box)
{	
}

//Not implemented any more... is it necessary?
void AI::getCenterOfCollidable(float* f)
{
	Vehicle* v = vC->get(myVehicleNumber);
	v->getPosition(f);

}

CollidableType AI::getCollidableType(void)
{ 
	return AI_PROBE;
}	

void AI::registerCollision(CollidableType cT, int collidingBoxNum)
{

	probeCheckArray[collidingBoxNum] = true;

}

void AI::processCollisions(void)
{
	
	Vehicle* v = vC->get(myVehicleNumber);
	
	/*if(probeCheckArray[0]) v->setColor(1.0f, 0.0f, 0.0f);
	if(probeCheckArray[1]) v->setColor(0.0f, 1.0f, 1.0f);
	if(probeCheckArray[2]) v->setColor(0.0f, 0.0f, 1.0f);
	if(probeCheckArray[3]) v->setColor(0.0f, 1.0f, 0.0f);
	*/

/*	if(probeCheckArray[2] || probeCheckArray[3])
	{
		probeCheckArray[0] = false;
		probeCheckArray[1] = false;
		probeCheckArray[2] = false;
		probeCheckArray[3] = false;


		return;
	}
	*/

	float o[3];
	v->getOrientation(o);

	float maxAngle = abs(v->getSpeed()) * angularInertia;

	if(maxAngle>3.0f) 
		maxAngle = 3.0f;

	if( !probeCheckArray[0] && !probeCheckArray[1] )
	{
		o[2] += maxAngle;
		angularInertia *= .80f;
	}
	else if( probeCheckArray[0] && probeCheckArray[1] ) //let Sr. Issaic Newton drive
	{
		o[2] += angularInertia;
		if(angularInertia>0) angularInertia += 0.5f;
		else angularInertia-=0.5f;
	}
	else if( probeCheckArray[0] ) //turn to the right
	{
		o[2] += maxAngle;
		angularInertia += 0.05f;
	}
	else if( probeCheckArray[1] )
	{
		o[2] += maxAngle;
		angularInertia -= 0.05f;
	}
	else if( probeCheckArray[2] )
	{
		o[2] += 1.0f;
	}
	else if( probeCheckArray[3] )
	{
		o[2] -= 1.0f;
	}

		
	
	v->setOrientation(o);

	probeCheckArray[0] = false;
	probeCheckArray[1] = false;
	probeCheckArray[2] = false;
	probeCheckArray[3] = false;

}

bool AI::isPhysicsActive(void)
{
	return physicsEnabled;
}

void AI::switchLOS( void )
{
	showLOS = !showLOS;
}

#endif