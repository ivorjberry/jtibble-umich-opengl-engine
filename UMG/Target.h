#ifndef _TARGET_H_
#define _TARGET_H_

#include "Render.h"
#include "Physics.h"
#include "Move.h"
#include "Effect.h"

class Target: public Collidable, public Renderable, public Movable
{
	//Obstacles are assumed to be rectangular solids that are axis-aligned
	//Feel free to put member variables in here like position, length, width, height, color, etc
	//Also feel free to add public methods like setPosition/Width/Height/Length/Color
	
	float position[3]; //x,y,z
	float targetRot;
	bool physicsEnabled;
	float diameter;
	float targetRotAmount;
	bool shouldBeDeleted;
	EffectContainer* effectContainer;
	
public:
	Target ( float d, EffectContainer* eC )
	{
		position[0] = position[1] = position[2] = 0;
		targetRot = 0;
		targetRotAmount = 0.5;
		physicsEnabled = true;
		diameter = d;
		shouldBeDeleted = false;
		effectContainer = eC;
		
	}
 
	void render( void )
	{
		glPushMatrix();
		
			glTranslatef( position[0], position[1], position[2]);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(targetRot, 1.0f, 0.0f, 0.0f);
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			PrimitiveCylinder outer( 0.95f * diameter, 1.0f * diameter );
			outer.render();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			PrimitiveCylinder middle( 0.98f * diameter, 0.6f * diameter);
			middle.render();
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			PrimitiveCylinder inner( 1.0f * diameter, 0.2f * diameter );
			inner.render();

		glPopMatrix();
	}

	void move ( void )
	{
		targetRot += targetRotAmount;
	}
	
	int getNumElements(void)
	{
		return 1;
	}

	void getPoint(int n, float* point)
	{

	}

	//write the 8 verteces into the passed-in float array 
	void getBox(int n, float* box)
	{
		float length = 1.0f * diameter;
		float width = 1.0f * diameter;
		float height = 1.0f * diameter;
		float yaw = targetRot;
	
		float maxXArg1 = abs( length / 2.0f * (float)cos( yaw * M_PI / 180.0f ) );
		float maxXArg2 = abs( width / 2.0f * (float)sin( yaw * M_PI / 180.0f ) );

		float maxZArg1 = abs( width  / 2.0f * (float)cos( yaw * M_PI / 180.0f ) );
		float maxZArg2 = abs( length  / 2.0f * (float)sin( yaw * M_PI / 180.0f ) );


		float boundingMaxX = max( maxXArg1, maxXArg2 );
		float maxX = position[0] + boundingMaxX;
		float minX = position[0] - boundingMaxX;

		float boundingMaxZ = max( maxZArg1, maxZArg2 );
		float maxZ = position[2] + boundingMaxZ;
		float minZ = position[2] - boundingMaxZ;

		float bBoxBottom = position[1] - height/2.0f;
		float bBoxTop = position[1] + height/2.0f;

		//front is the face of the cube in the +Z axis
		box[0] = minX; //-X (bottom left)
		box[1] = bBoxBottom; 
		box[2] = maxZ; 
		box[3] = minX; //-X (top left)
		box[4] = bBoxTop;
		box[5] = maxZ;
		box[6] = maxX; //+X (top right)
		box[7] = bBoxTop;
		box[8] = maxZ;
		box[9] = maxX; //+X (bottom right)
		box[10] = bBoxBottom;
		box[11] = maxZ;
			
		//back (-Z direction)
		box[12] = minX;
		box[13] = bBoxBottom;
		box[14] = minZ;
		box[15] = minX;
		box[16] = bBoxTop;
		box[17] = minZ;
		box[18] = maxX;
		box[19] = bBoxTop;
		box[20] = minZ;
		box[21] = maxX;
		box[22] = bBoxBottom;
		box[23] = minZ;
	}

	//self-explanitory
	void getCenterOfCollidable(float* box)
	{
		box[0] = position[0];
		box[1] = position[1];
		box[2] = position[2];
	}
	
	void registerCollision(CollidableType cT, int collidingBoxNum)
	{
		currentCollisions.push_back(cT);
	}

	void processCollisions()
	{
		for(int x=0; x< (int)currentCollisions.size(); x++)
		{
			if(currentCollisions.at(x) == PROJECTILE_MINIGUN ||
				currentCollisions.at(x) == PROJECTILE_MINE ||
				currentCollisions.at(x) == PROJECTILE_MISSILE){
				shouldBeDeleted = true;

				effectContainer->add(EXPLOSION,position);
			}
		}
		
		clearCurrentCollisions();
	}


	//targets touching obstacles should NOT generate a physics event
	CollidableType getCollidableType(void)
	{
		return TARGET;
	}
	
	bool isPhysicsActive()
	{
		//return true if physics have been enabled
		return physicsEnabled;
	}

	void setPosition(float x, float y, float z)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	void setRotation(float angle)
	{
		targetRot = angle;
	}

	bool shouldBeRemoved(void)
	{
		return shouldBeDeleted;
	}
};


class TargetContainer{
	vector<Target*> list;
	Render* renderer;
	Physics* physics;
	Move* mover;
	EffectContainer* effectContainer;
public:
	TargetContainer(Render* r, Move* m, Physics* p, EffectContainer* eC){
		list = vector<Target*>();
		renderer = r;
		mover = m;
		physics = p;
		effectContainer = eC;
	}
	~TargetContainer()
	{
		while(list.size()>0)
		{
			Target* t = list.at(0);
			delete t;
			list.erase(list.begin());
		}
	}

	//Creates and allocates new obstacle
	void add( float diameter )
	{
		Target* t = new Target(diameter, effectContainer);
		list.push_back(t);
		renderer->add(t);
		mover->add(t);
		physics->add(t);
		t->setCollidableShape(COLLIDABLE_BOX);
	}

	void remove( int i )
	{
		Target* t = list.at(i);
		delete t;
		list.erase(list.begin()+i);
	}

	Target* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}

		//goes through all projectiles and removes them from all
	//managers if they need to be deleted
	void clean( void )
	{
		unsigned int n = list.size();

		for(unsigned int i = 0; i < n; i++)
		{

			if(list.at(i)->shouldBeRemoved())
			{
				renderer->remove(list.at(i));
				mover->remove(list.at(i));
				physics->removeAsMoving(list.at(i));

				
			}
			
		}
		
	}

	void cleanAll( void )
	{
		unsigned int n = list.size();

		for(unsigned int i = 0; i < n; i++)
		{

			renderer->remove(list.at(i));
			mover->remove(list.at(i));
			physics->removeAsMoving(list.at(i));
			//remove(i);
			//i--;

		}
		list.clear();
	}

};

#endif