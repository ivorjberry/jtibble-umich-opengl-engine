#include "Physics.h"	
#include "Graphics.h"

//This is the function that is responsible for iterating through all the physics 
//objects and checking if they do or do not collide with each other. 
void Physics::step(void)
{
	Collidable *A, *B;
	float boxA[24];
	float boxB[24];
	float pointA[3];
	float pointB[3];

	int numObjects = (int)list.size();

	//Iterate through all moving objects,
	for( int x=0; x<numObjects; x++)
	{
		A = list[x];

		//If physics is not active for that object, continue on
		//Some objects will disable physics after a single collision has been registered
		if( !(A->isPhysicsActive()) ) continue; 

		//Get the number of colliding elements in A
		int AnumElements = A->getNumElements();
		
		//Get A's CollidableType
		CollidableType cTA = A->getCollidableType();

		//All the shapes for a Collidable are the same
		CollidableShape Ashape = A->shape;


		//Iterate through all of object A's colliding shapes
		for( int AelementNum = 0; AelementNum < AnumElements; AelementNum++ )
		{

			//If the shape is a box, pull out a box; otherwise, get a point
			if(Ashape == COLLIDABLE_BOX)
			{
				A->getBox(AelementNum, boxA);
			}
			else if(Ashape == COLLIDABLE_POINT)
			{
				A->getPoint(AelementNum, pointA);
			}
	

			//Iterate through all other moving objects but itself
			for( int y=x+1; y<numObjects; y++)
			{	
				B = list[y];
				if( !(B->isPhysicsActive()) ) continue; //Don't check for collisions if not activated
				
				//Get B's CollidableType
				CollidableType cTB = B->getCollidableType();

				//Continue if the two types shouldn't be checked
				if(!collisionDetectionAllowed(cTA, cTB)) continue;

				//All the shapes for a Collidable are the same
				CollidableShape Bshape = B->shape;

				//Continue if trying to check point against point (worthless)
				if(Ashape == COLLIDABLE_POINT && Bshape == COLLIDABLE_POINT) continue;


				int BnumElements = B->getNumElements();

				//Iterate through all of B's elements
				for( int BelementNum = 0; BelementNum < BnumElements; BelementNum++ )
				{

					if(Bshape == COLLIDABLE_BOX)
					{
						B->getBox(BelementNum, boxB);
					}
					else
					{
						B->getPoint(BelementNum, pointB);
					}

					bool collisionDetected = false;

					if(Ashape == COLLIDABLE_BOX && Bshape == COLLIDABLE_BOX)
					{
						collisionDetected = collisionBoxBox(boxA, boxB);
					}
					else if(Ashape == COLLIDABLE_POINT && Bshape == COLLIDABLE_BOX)
					{
						collisionDetected = collisionPointBox(pointA, boxB);
					}
					else if(Ashape == COLLIDABLE_BOX && Bshape == COLLIDABLE_POINT)
					{
						collisionDetected = collisionPointBox(pointB, boxA);
					}

					if(collisionDetected)
					{
						A->registerCollision(cTB, AelementNum);
						B->registerCollision(cTA, BelementNum);
					}
					
				}
			}
		}

		A->processCollisions();
	}

}

	
// Returns true if any of the verteces in A are within B, or vice-versa
bool Physics::collisionBoxBox(float* A, float* B)
{
	float AmaxX, AminX, AmaxY, AminY, AmaxZ, AminZ;
	float BmaxX, BminX, BmaxY, BminY, BmaxZ, BminZ;

	//Set the max and min to limits of float type
	AmaxX = -1.0f*FLT_MAX;
	AmaxY = -1.0f*FLT_MAX;
	AmaxZ = -1.0f*FLT_MAX;
	AminX = FLT_MAX;
	AminY = FLT_MAX;
	AminZ = FLT_MAX;

	BmaxX = -1.0f*FLT_MAX;
	BmaxY = -1.0f*FLT_MAX;
	BmaxZ = -1.0f*FLT_MAX;
	BminX = FLT_MAX;
	BminY = FLT_MAX;
	BminZ = FLT_MAX;
	
	//find the maximum, minimum X, Y, and Z values for both the A and B verteces
	AmaxX = max(AmaxX, max(A[0], max(A[3], max(A[6], max(A[9], max(A[12], max(A[15], max(A[18], A[21]))))))));
	AmaxY = max(AmaxY, max(A[1], max(A[4], max(A[7], max(A[10], max(A[13], max(A[16], max(A[19], A[22]))))))));
	AmaxZ = max(AmaxZ, max(A[2], max(A[5], max(A[8], max(A[11], max(A[14], max(A[17], max(A[20], A[23]))))))));
	
	AminX = min(AminX, min(A[0], min(A[3], min(A[6], min(A[9], min(A[12], min(A[15], min(A[18], A[21]))))))));
	AminY = min(AminY, min(A[1], min(A[4], min(A[7], min(A[10], min(A[13], min(A[16], min(A[19], A[22]))))))));
	AminZ = min(AminZ, min(A[2], min(A[5], min(A[8], min(A[11], min(A[14], min(A[17], min(A[20], A[23]))))))));
	
	BmaxX = max(BmaxX, max(B[0], max(B[3], max(B[6], max(B[9], max(B[12], max(B[15], max(B[18], B[21]))))))));
	BmaxY = max(BmaxY, max(B[1], max(B[4], max(B[7], max(B[10], max(B[13], max(B[16], max(B[19], B[22]))))))));
	BmaxZ = max(BmaxZ, max(B[2], max(B[5], max(B[8], max(B[11], max(B[14], max(B[17], max(B[20], B[23]))))))));
	
	BminX = min(BminX, min(B[0], min(B[3], min(B[6], min(B[9], min(B[12], min(B[15], min(B[18], B[21]))))))));
	BminY = min(BminY, min(B[1], min(B[4], min(B[7], min(B[10], min(B[13], min(B[16], min(B[19], B[22]))))))));
	BminZ = min(BminZ, min(B[2], min(B[5], min(B[8], min(B[11], min(B[14], min(B[17], min(B[20], B[23]))))))));

	//Check to see if any of the verteces of A are inside B
	for(int x=0; x<8; x++)
	{
		float toCheckX, toCheckY, toCheckZ;
		toCheckX = A[3*x];
		toCheckY = A[3*x + 1];
		toCheckZ = A[3*x + 2];

		if( toCheckX >= BminX && toCheckX <= BmaxX &&
			toCheckY >= BminY && toCheckY <= BmaxY &&
			toCheckZ >= BminZ && toCheckZ <= BmaxZ )
			return true;

		toCheckX = B[3*x];
		toCheckY = B[3*x + 1];
		toCheckZ = B[3*x + 2];

		if( toCheckX >= AminX && toCheckX <= AmaxX &&
			toCheckY >= AminY && toCheckY <= AmaxY &&
			toCheckZ >= AminZ && toCheckZ <= AmaxZ )
			return true;

	}

	return false;


}

bool Physics::collisionPointBox(float* p, float* B)
{
	float AmaxX, AmaxY, AmaxZ;
	float BmaxX, BminX, BmaxY, BminY, BmaxZ, BminZ;

	//Set the max and min to limits of float type
	AmaxX = p[0];
	AmaxY = p[1];
	AmaxZ = p[2];

	BmaxX = -1.0f*FLT_MAX;
	BmaxY = -1.0f*FLT_MAX;
	BmaxZ = -1.0f*FLT_MAX;
	BminX = FLT_MAX;
	BminY = FLT_MAX;
	BminZ = FLT_MAX;
	
	BmaxX = max(BmaxX, max(B[0], max(B[3], max(B[6], max(B[9], max(B[12], max(B[15], max(B[18], B[21]))))))));
	BmaxY = max(BmaxY, max(B[1], max(B[4], max(B[7], max(B[10], max(B[13], max(B[16], max(B[19], B[22]))))))));
	BmaxZ = max(BmaxZ, max(B[2], max(B[5], max(B[8], max(B[11], max(B[14], max(B[17], max(B[20], B[23]))))))));
	
	BminX = min(BminX, min(B[0], min(B[3], min(B[6], min(B[9], min(B[12], min(B[15], min(B[18], B[21]))))))));
	BminY = min(BminY, min(B[1], min(B[4], min(B[7], min(B[10], min(B[13], min(B[16], min(B[19], B[22]))))))));
	BminZ = min(BminZ, min(B[2], min(B[5], min(B[8], min(B[11], min(B[14], min(B[17], min(B[20], B[23]))))))));

	//Check to see if any of the verteces of A are inside B
	float toCheckX, toCheckY, toCheckZ;
	toCheckX = AmaxX;
	toCheckY = AmaxY;
	toCheckZ = AmaxZ;

	if( toCheckX >= BminX && toCheckX <= BmaxX &&
		toCheckY >= BminY && toCheckY <= BmaxY &&
		toCheckZ >= BminZ && toCheckZ <= BmaxZ )
	{
		return true;
	}
	else 
	{
		return false;
	}

}



bool Physics::collisionDetectionAllowed(CollidableType A, CollidableType B)
{
	//defaults to true unless one of the following pairings happens
	if( collisionLegal[A][B])
		return true;
	else return false;
}


void Physics::fillCollisionLegalArray(vector<vector<bool> >& a)
{
	//Create vector of vectors of bool
	vector<bool> temp = vector<bool>();
	for(int x=0; x<END_TYPE_FOR_ARRAY_FILL; x++)
	{
		temp.push_back(false);
	}

	for(int x=0; x<	END_TYPE_FOR_ARRAY_FILL; x++)
	{
		a.push_back(temp);
	}

	int ySize = a.size();
	int xSize = a.at(0).size();

	//Go through all CollidableTypes and set their collide-ability

	//Projectile-Minigun
	a[PROJECTILE_MINIGUN][VEHICLE] = true;
	a[VEHICLE][PROJECTILE_MINIGUN] = true;

	a[PROJECTILE_MINIGUN][OBSTACLE] = true;
	a[OBSTACLE][PROJECTILE_MINIGUN] = true;

	a[PROJECTILE_MINIGUN][TARGET] = true;
	a[TARGET][PROJECTILE_MINIGUN] = true;

	//Projectile-Mine
	a[PROJECTILE_MINE][VEHICLE] = true;
	a[VEHICLE][PROJECTILE_MINE] = true;
	
	a[PROJECTILE_MINE][OBSTACLE] = true;
	a[OBSTACLE][PROJECTILE_MINE] = true;

	a[PROJECTILE_MINE][TARGET] = true;
	a[TARGET][PROJECTILE_MINE] = true;

	
	//Projectile-Missile
	a[PROJECTILE_MISSILE][VEHICLE] = true;
	a[VEHICLE][PROJECTILE_MISSILE] = true;

	a[PROJECTILE_MISSILE][OBSTACLE] = true;
	a[OBSTACLE][PROJECTILE_MISSILE] = true;

	a[PROJECTILE_MISSILE][TARGET] = true;
	a[TARGET][PROJECTILE_MISSILE] = true;


	//Projectile-Bomb
	a[PROJECTILE_BOMB][VEHICLE] = true;
	a[VEHICLE][PROJECTILE_BOMB] = true;

	a[PROJECTILE_BOMB][OBSTACLE] = true;
	a[OBSTACLE][PROJECTILE_BOMB] = true;

	//Projectile-Speed Missile
	a[PROJECTILE_SPEED_MISSILE][VEHICLE] = true;
	a[VEHICLE][PROJECTILE_SPEED_MISSILE] = true;

	a[PROJECTILE_SPEED_MISSILE][OBSTACLE] = true;
	a[OBSTACLE][PROJECTILE_SPEED_MISSILE] = true;

	a[PROJECTILE_SPEED_MISSILE][TARGET] = true;
	a[TARGET][PROJECTILE_SPEED_MISSILE] = true;

	//Projectile-Power Missile
	a[PROJECTILE_POWER_MISSILE][VEHICLE] = true;
	a[VEHICLE][PROJECTILE_POWER_MISSILE] = true;

	a[PROJECTILE_POWER_MISSILE][OBSTACLE] = true;
	a[OBSTACLE][PROJECTILE_POWER_MISSILE] = true;

	a[PROJECTILE_POWER_MISSILE][TARGET] = true;
	a[TARGET][PROJECTILE_POWER_MISSILE] = true;

	//Item-Minigun
	a[ITEM_MINIGUN][VEHICLE] = true;
	a[VEHICLE][ITEM_MINIGUN] = true;

	//Item-Mine
	a[ITEM_MINE][VEHICLE] = true;
	a[VEHICLE][ITEM_MINE] = true;
	
	//Item-Missile
	a[ITEM_MISSILE][VEHICLE] = true;
	a[VEHICLE][ITEM_MISSILE] = true;

	//Item-Speed Missile
	a[ITEM_SPEED_MISSILE][VEHICLE] = true;
	a[VEHICLE][ITEM_SPEED_MISSILE] = true;

	//Item-Power Missile
	a[ITEM_POWER_MISSILE][VEHICLE] = true;
	a[VEHICLE][ITEM_POWER_MISSILE] = true;

	//Item-Bomb
	a[ITEM_BOMB][VEHICLE] = true;
	a[VEHICLE][ITEM_BOMB] = true;

	//Item-Health
	a[ITEM_HEALTH][VEHICLE] = true;
	a[VEHICLE][ITEM_HEALTH] = true;

	//Vehicle
	a[VEHICLE][VEHICLE] = true;

	a[VEHICLE][OBSTACLE] = true;
	a[OBSTACLE][VEHICLE] = true;
	
	a[VEHICLE][TARGET] = true;
	a[TARGET][VEHICLE] = true;

	a[VEHICLE][AI_PROBE] = true;
	a[AI_PROBE][VEHICLE] = true;

	//Obstacle
	a[OBSTACLE][AI_PROBE] = true;
	a[AI_PROBE][OBSTACLE] = true;

	//Target
	a[TARGET][AI_PROBE] = true;
	a[AI_PROBE][TARGET] = true;

	//Camera
	a[CAMERA][OBSTACLE] = true;
	a[OBSTACLE][CAMERA] = true;
	
	
}



void Physics::flipBoundingBoxes( void )
{
	drawBoundingBoxes = !drawBoundingBoxes;
	//remove bounding box graphics from rendering
	for(int x=0; x<(int)boundingBoxes.size(); x++)
	{
		BoundingBox* bb = boundingBoxes.at(x);
		if(drawBoundingBoxes)
		{
			bb->enableRender();
		}
		else
		{
			bb->disableRender();			
		}
	}

}







void Physics::LOS(Collidable* a)
{
	int numChecks = a->getNumElements();

	for(int x=0; x<numChecks; x++)
	{
		float point[3];
		a->getPoint(x, point);
		unsigned int s = obstacleList.size();
		for(unsigned int y=0; y<s; y++)
		{
			float box[24];
			obstacleList[y]->getBox(0,box);
			bool collisionDetected = collisionPointBox(point,box);
			if(collisionDetected)
			{
				a->registerCollision(OBSTACLE,0);
				return;
			}

		}
	}

}



