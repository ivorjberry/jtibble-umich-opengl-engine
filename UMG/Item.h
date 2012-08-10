#ifndef _ITEM_H_
#define _ITEM_H_

#include "Settings.h"
#include "SystemTimer.h"
#include "Render.h"
#include "Physics.h"

class Item: public Collidable, public Renderable
{
	float position[3]; //x,y,z
	float rotate;
	float color[4]; //rgb
	bool physicsEnabled;
	float itemDiameter;
	CollidableType itemType;
	SystemTimer timer;
	float waitTime;
	
public:
	Item ( CollidableType cT )
	{
		itemType = cT;
		//position[0] = position[1] = position[2] = 0;
		position[0] = 700.0f;
		position[1] = 0.5;
		position[2] = -200.0f;
		rotate = 0.0f;
		color[0] = color[1] = color[2] = 0.5f;
		physicsEnabled = true;
		itemDiameter = 1.0f;
		waitTime = 0.0f;
		//timer.startTimer();
	}
	//at the moment, we can just flat-shade them with a solid color
	void render( void )
	{
		if(timer.elapsedTime() >= waitTime)
		{
			physicsEnabled = true;
		}
		if(physicsEnabled)
		{
			glPushMatrix();
				glColor4f(color[0], color[1], color[2], 1.0f);
				glTranslatef( position[0], position[1], position[2]);
				glRotatef(rotate, 0.0f, 1.0f, 0.0f);
				PrimitiveCube newObst( itemDiameter, false );
				newObst.render();

				rotate += 3.0f;

				if (rotate >= 360)
				{
					rotate -= 360.0f;
				}
			glPopMatrix();
		}
	}

	int getNumElements(void)
	{
		return 1;
	}

	//write the 8 verteces into the passed-in float array (
	void getBox(int n, float* box)
	{
		float maxXArg = itemDiameter / 2.0f;
		float maxZArg = itemDiameter / 2.0f;
		float height = itemDiameter;

		float maxX = position[0] + maxXArg;
		float minX = position[0] - maxXArg;

		float maxZ = position[2] + maxZArg;
		float minZ = position[2] - maxZArg;

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

	void getPoint(int n, float* point)
	{

	}

	//self-explanitory
	void getCenterOfCollidable(float* box)
	{
		box[0] = position[0];
		box[1] = position[1];
		box[2] = position[2];
	}

	//probably won't react to collisions, so you don't have to do much with this
	void registerCollision(CollidableType cT, int collidingBoxNum)
	{
		physicsEnabled = false;
		float minRespawn = ITEM_MIN_RESPAWN;
		float maxRespawn = ITEM_MAX_RESPAWN;
		float range = maxRespawn - minRespawn;
		waitTime = (float)(rand()%100 / 100.0f) * range + minRespawn; //minRespawn up to maxRespawn, increments of 1/100 the difference
		timer.startTimer();
		int randItem = rand()%100;
		if (randItem < 5) 
			itemType = ITEM_HEALTH;
		else if (randItem >= 5 && randItem < 17)
			itemType = ITEM_BOMB;
		else if (randItem >= 17 && randItem < 29)
			itemType = ITEM_POWER_MISSILE;
		else if (randItem >= 29 && randItem < 44)
			itemType = ITEM_MINE;
		else if (randItem >= 44 && randItem < 60)
			itemType = ITEM_SPEED_MISSILE;
		else if (randItem >= 60 && randItem < 78)
			itemType = ITEM_MISSILE;
		else
			itemType = ITEM_MINIGUN;
	}

	//obstacles touching obstacles should NOT generate a physics event
	CollidableType getCollidableType(void)
	{
		return itemType;
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

	void getPosition(float* p)
	{
		p[0] = position[0];
		p[1] = position[1];
		p[2] = position[2];
	}

	void setColor(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	void processCollisions()
	{
			
		if(isColliding())
		{
			color[0] = 1.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
		}
		else
		{
			if( itemType == ITEM_MINIGUN )
			{
				color[0] = 0.5f;
				color[1] = 0.0f;//242.0f/255.0f;
				color[2] = 0.0f;
			}
			else if( itemType == ITEM_MINE )
			{
				color[0] = 0.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}
			else if( itemType == ITEM_MISSILE )
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
			}
			else if( itemType == ITEM_BOMB )
			{
				color[0] = 0.5f;
				color[1] = 0.0f;
				color[2] = 0.25f;
			}
			else if( itemType == ITEM_SPEED_MISSILE )
			{
				color[0] = SMISSILE_PROJECTILE_COLOR_R;
				color[1] = SMISSILE_PROJECTILE_COLOR_G;
				color[2] = SMISSILE_PROJECTILE_COLOR_B;
			}
			else if( itemType == ITEM_POWER_MISSILE )
			{
				color[0] = PMISSILE_PROJECTILE_COLOR_R;
				color[1] = PMISSILE_PROJECTILE_COLOR_G;
				color[2] = PMISSILE_PROJECTILE_COLOR_B;
			}
			else
			{
				color[0] = 0.0f;
				color[1] = 1.0f;
				color[2] = 1.0f;
			}
		
		}

		clearCurrentCollisions();
	}
};

//Also add an ObstacleContainer that allows us to add Obstacles into the Physics manager
// as STATIC collidable objects ---  physics->addAsStatic()
// Copy the Vehicle or Projectile Classes' Containers; they are the most thorough
class ItemContainer{
	vector<Item*> list;
	Render* renderer;
	Physics* physics;
public:
	ItemContainer(Render* r, Physics* p){
		list = vector<Item*>();
		renderer = r;
		physics = p;
	}
	~ItemContainer()
	{
		while(list.size()>0)
		{
			Item* o = list.at(0);
			delete o;
			list.erase(list.begin());
		}
	}

	//Creates and allocates new obstacle
	void add( CollidableType cT )
	{
		Item* o = new Item( cT );
		list.push_back(o);
		renderer->add(o);
		physics->add(o);
		o->setCollidableShape(COLLIDABLE_BOX);
	}

	void remove( int i )
	{
		Item* o = list.at(i);
		delete o;
		list.erase(list.begin()+i);
	}

	Item* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}

	Item* getLast( void )
	{
		return list.at(list.size()-1);
	}

	int getNum( void )
	{
		return (int)list.size();
	}

	void cleanAll( void )
	{
		for(unsigned int i = 0; i < (int) list.size(); i++)
		{
			Item* it = list.at(i);

			renderer->remove(it);
			physics->removeAsMoving(it);
			remove(i);
			i--; 
		}

	}

};

#endif