#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "SystemTimer.h"
#include "Physics.h"
#include "Render.h"
#include "Move.h"
#include "Effect.h"
#include "Model.h"
#include "Settings.h"
#include "Sound.h"

struct WeaponInfo{
	float position[3];
	float velocity[3];
	float acceleration[3];

	float orientation[3];	//Roll, Pitch, Yaw (degrees)

	float aimPoint[3];		// X-Y-Z

	float color[4];			//RGB, from 0 to 1

	float speed;

};


class Projectile:public Collidable, public Renderable, public Movable
{
	bool physicsEnabled;
	bool collisionDetected;
	bool shouldBeDeleted;
	WeaponInfo info;
	CollidableType type;
	float speed;
	float steerAmount;
	SystemTimer activationTimer;
	Model* myModel;
	
	EffectContainer* effectContainer;
	SoundContainer* soundContainer;
	
	Sound* s;
	Sound* sE;

public:

	
	
	Projectile(CollidableType cT, WeaponInfo wI, EffectContainer* eC,SoundContainer* sC, Model* m)
	{
		info = wI;
		type = cT;
		myModel = m;

		collisionDetected = false;
		shouldBeDeleted = false;
		physicsEnabled = true;

		activationTimer = SystemTimer();
		activationTimer.startTimer();
		
		soundContainer = sC;

		float soundZeros[3] = {0.0f, 0.0f, 0.0f};

		if( cT == PROJECTILE_MINIGUN )
		{
			info.color[0] = 1.0f;
			info.color[1] = 242.0f / 255.0f;
			info.color[2] = 0.0f;
			info.color[3] = 1.0f;
			setCollidableShape(COLLIDABLE_POINT);
			s = soundContainer->get(3);
			s->setPosition(wI.position);
			s->setVelocity(soundZeros);
			s->play();
		}
		else if( cT == PROJECTILE_MINE )
		{
			physicsEnabled = false;
			info.color[0] = 0.0f;
			info.color[1] = 0.5f;
			info.color[2] = 0.0f;
			info.color[3] = 1.0f;
			setCollidableShape(COLLIDABLE_BOX);
		}
		else if( cT == PROJECTILE_MISSILE )
		{
			physicsEnabled = false;
			info.color[0] = 0.0f;
			info.color[1] = 0.0f;
			info.color[2] = 1.0f;
			info.color[3] = 1.0f;
			setCollidableShape(COLLIDABLE_POINT);

			s = soundContainer->get(4);
			s->setPosition(wI.position);
			s->setVelocity(soundZeros);
			s->play();
		}
		else if( cT == PROJECTILE_BOMB )
		{
			physicsEnabled = false;
			info.color[0] = 0.5f;
			info.color[1] = 0.0f;
			info.color[2] = 0.25f;
			info.color[3] = 1.0f;
			setCollidableShape(COLLIDABLE_POINT);
		}
		else if( cT == PROJECTILE_SPEED_MISSILE )
		{
			info.color[0] = SMISSILE_PROJECTILE_COLOR_R;
			info.color[1] = SMISSILE_PROJECTILE_COLOR_G;
			info.color[2] = SMISSILE_PROJECTILE_COLOR_B;
			info.color[3] = SMISSILE_PROJECTILE_COLOR_A;
			setCollidableShape(COLLIDABLE_POINT);
			s = soundContainer->get(4);
			s->setPosition(wI.position);
			s->setVelocity(soundZeros);
			s->play();
		}
		else if( cT == PROJECTILE_POWER_MISSILE )
		{
			info.color[0] = PMISSILE_PROJECTILE_COLOR_R;
			info.color[1] = PMISSILE_PROJECTILE_COLOR_G;
			info.color[2] = PMISSILE_PROJECTILE_COLOR_B;
			info.color[3] = PMISSILE_PROJECTILE_COLOR_A;
			setCollidableShape(COLLIDABLE_POINT);
			s = soundContainer->get(4);
			s->setPosition(wI.position);
			s->setVelocity(soundZeros);
			s->play();
		}
		else
		{
			info.color[0] = 1.0f;
			info.color[1] = 1.0f;
			info.color[2] = 1.0f;
			info.color[3] = 1.0f;
			setCollidableShape(COLLIDABLE_POINT);
		}

		
		steerAmount = 0.0f;
		effectContainer = eC;
	}

	~Projectile(void){ }

	bool shouldBeRemoved( void )
	{
		return shouldBeDeleted;
	}

	//MOVE FUNCTION
	void move ( void )
	{
		
		//destroy the projetile if it's out-of-bounds
		if( info.position[0] < -100 ||
			info.position[0] > 1100 ||
			info.position[1] < 0 || 
			info.position[2] < -1100 ||
			info.position[2] > 100 )
		{
			shouldBeDeleted = true;
		}
	

		if( type == PROJECTILE_MINIGUN)
		{
			//Minigun physics is easy-- bullets travel in straight lines!
			info.position[0] += info.velocity[0];
			info.position[1] += info.velocity[1];
			info.position[2] += info.velocity[2];
		}
		else if (type == PROJECTILE_POWER_MISSILE)
		{
			//Minigun physics is easy-- bullets travel in straight lines!
			info.position[0] += info.velocity[0];
			info.position[1] += info.velocity[1];
			info.position[2] += info.velocity[2];
			effectContainer->add(SMOKE, info.position);
		}
		else if (type == PROJECTILE_SPEED_MISSILE) {
			info.position[0] += info.velocity[0];
			info.position[1] += info.velocity[1];
			info.position[2] += info.velocity[2];
			effectContainer->add(SMOKE_SMALL, info.position);
		}
		else if( type == PROJECTILE_BOMB )
		{
			info.position[0] += info.velocity[0];
			info.position[1] += info.velocity[1];
			info.position[2] += info.velocity[2];

			float time = BOMB_ACTIVATION;
			float endTime = BOMB_LIFETIME;
			if(activationTimer.elapsedTime() >= time)
			{
				physicsEnabled = true;
				info.color[0] = 0.5f;
				info.color[1] = 0.0f;
				info.color[2] = 0.25f;
				info.color[3] = 1.0f;
			}

			if(activationTimer.elapsedTime() > endTime)
			{
				shouldBeDeleted = true;
			}
		}
		else if( type == PROJECTILE_MINE)
		{
			float time = MINE_ACTIVATION;
			float endTime = MINE_LIFETIME;
			//Nothing. Mines don't move unless we want them to roll (FOR THE LOVE OF.... NOT NOW!)
			if(activationTimer.elapsedTime() >= time)
			{
				physicsEnabled = true;
				info.color[1] = 0.2f;
			}
			
			if(activationTimer.elapsedTime() > endTime)
			{
				shouldBeDeleted = true;
			}
		}
		else if( type == PROJECTILE_MISSILE)
		{
			float time = MISSILE_ACTIVATION;
			float endTime = MISSILE_LIFETIME;
			float lifePercent = activationTimer.elapsedTime()/endTime;

			effectContainer->add(SMOKE, info.position);
			//Increase speed up to top speed
			//Calculate difference between current heading (yaw) and where it needs to go
			float diffX = info.aimPoint[0] - info.position[0];
			float diffZ = info.aimPoint[2] - info.position[2];
			double tmpAngle = atan2( diffZ , diffX );
			float angleToAimPointFromMissile = (float)(tmpAngle * 180.0f / M_PI);
			
			if(angleToAimPointFromMissile < 0) 
				angleToAimPointFromMissile = 360 + angleToAimPointFromMissile;

			//angle increases clockwise from 0 (straight ahead) to 180 (behind) to 360(ahead)
			while(info.orientation[2] < 0.0f)
				info.orientation[2]+=360.0f;

			while(info.orientation[2] > 360.0f)
				info.orientation[2] -= 360.0f;



			float angleDifference = angleToAimPointFromMissile - info.orientation[2];

			while(angleDifference < 0.0f)
				angleDifference += 360.0f;

			while(angleDifference > 360.0f)
				angleDifference -= 360.0f;
			
			bool turnRight;
			if(angleDifference < 180.0f) turnRight = true;
			else turnRight = false;

			if(physicsEnabled)
			{
				//NEED TO MAKE Area of Effect damage!!
				if(abs(180.0f - angleDifference) < 30.0f)
				{
					setCollidableShape(COLLIDABLE_BOX);
					steerAmount = 0.0;
					/*shouldBeDeleted = true;

					effectContainer->add(CRACK, info.position);
					effectContainer->add(FIREBALL, info.position);*/
				}

			}
			else
			{
				steerAmount = 8.0f;
			}



			if( turnRight) 
			{
				info.orientation[2]+=steerAmount;
			}
			else
			{
				info.orientation[2]-=steerAmount;
			}
			
			float currentSpeed = info.speed/2.0f + info.speed/2.0f*lifePercent;
			
			info.velocity[0] = info.speed * (float)cos((info.orientation[2])*M_PI/180.0);
			info.velocity[1] = 0.0f;//projectileSpeed * (float)sin(mountOrientation[1]*M_PI/180.0);
			info.velocity[2] = info.speed * (float)sin((info.orientation[2])*M_PI/180.0);
		
			
			//Update position
			info.position[0] += info.velocity[0];
			info.position[1] += info.velocity[1];
			info.position[2] += info.velocity[2];

			float percent = activationTimer.elapsedTime()/endTime;
			if(activationTimer.elapsedTime() >= time)
			{
				physicsEnabled = true;
				info.color[2] = 0.5f;
			}

			if(percent > 0.5 && shape == COLLIDABLE_POINT)
					shape = COLLIDABLE_BOX;

			if(activationTimer.elapsedTime() > endTime)
			{
				shouldBeDeleted = true;
			}

			//s->setPosition(info.position);
		}
	}

	//RENDER FUNCTION
	void render( void )
	{
		
		glPushMatrix();
			if( type == PROJECTILE_MINIGUN )
			{
				glDisable(GL_LIGHTING);
				//Long streaks, like tracer rounds blurred by speed
				glColor4f(info.color[0], info.color[1], info.color[2], info.color[3]);
				
				glTranslatef(info.position[0],
							info.position[1],
							info.position[2] );

				glRotatef(-1.0f * info.orientation[2] + 90.0f, 0.0f, 1.0f, 0.0f);
				glScalef(2.0f, 2.0f, 2.0f);

				if(myModel != NULL)
					myModel->render();
				
				glEnable(GL_LIGHTING);
			}
			else if( type == PROJECTILE_MINE )
			{
				//Something on the ground. Maybe a shape protruding, flashing?
				glTranslatef(info.position[0],
							info.position[1],
							info.position[2] );

				glColor4f(info.color[0], info.color[1], info.color[2], info.color[3]);
				glScalef(0.4f, 0.2f, 0.4f);
				if(myModel != NULL)
					myModel->render();
			}
			else if( type == PROJECTILE_MISSILE )
			{
				glPushMatrix();
					glColor4f(0.3f, 0.3f, 1.0f, 1.0f);
					glTranslatef(info.aimPoint[0],
									info.aimPoint[1], 
									info.aimPoint[2] );
					glTranslatef(0.0f, 4.0f, 0.0f);
					glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
					PrimitiveCone aimPointMarker(3.0f, 0.5f);
					aimPointMarker.render();
				glPopMatrix();

				
				glTranslatef(info.position[0],
							info.position[1],
							info.position[2] );
				
				glColor4f(info.color[0], info.color[1], info.color[2], info.color[3]);
				
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					glRotatef(info.orientation[2] + 180.0f, 0.0f, 0.0f, 1.0f);

				if(myModel != NULL)
					myModel->render();
			}
			else if( type == PROJECTILE_BOMB )
			{
				glTranslatef(info.position[0],
							info.position[1],
							info.position[2] );
				
				glColor4f(info.color[0], info.color[1], info.color[2], info.color[3]);
				glRotatef(activationTimer.elapsedTime()*360.0f, 0.0f, 1.0f, 0.0f);
				
				glScalef(0.3f, 0.3f, 0.3f);
				if(myModel != NULL)
					myModel->render();
			}
			else if( type == PROJECTILE_SPEED_MISSILE )
			{
				glColor4f(info.color[0], info.color[1], info.color[2], info.color[3]);
				
				glTranslatef(info.position[0],
							info.position[1],
							info.position[2] );

				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef(info.orientation[2] + 90.0f, 0.0f, 0.0f, 1.0f);
				glScalef(0.5f, 0.5f, 0.5f);

				if(myModel != NULL)
					myModel->render();
			}
			else if( type == PROJECTILE_POWER_MISSILE )
			{
				glColor4f(info.color[0], info.color[1], info.color[2], info.color[3]);
				
				glTranslatef(info.position[0],
							info.position[1],
							info.position[2] );

				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef(info.orientation[2] + 90.0f, 0.0f, 0.0f, 1.0f);
				glScalef(1.1f, 1.1f, 1.1f);

				if(myModel != NULL)
					myModel->render();
			}

		glPopMatrix();
	}



	int getNumElements(void)
	{
		if(type == PROJECTILE_MINIGUN)
		{
			return 3; //Don't change this! The physics depends on it being 0<=x<=3
		}
		else if (type == PROJECTILE_SPEED_MISSILE || type == PROJECTILE_POWER_MISSILE)
		{
			return 2;
		}
		else if(type == PROJECTILE_BOMB)
		{
			return 4; //probes are a little ways in +/- X, +/- Z directions
		}
		else
		{
			return 1;
		}
	}

	void getBox(int n, float* box)
	{
		float width = 5.0f;
		float height = 5.0f;

		//depends on type of the weapon
		if(type == PROJECTILE_MINE)
		{
			width = 5.0f;
			height = 1.0f;
		}
		else if(type == PROJECTILE_MISSILE)
		{
			width = 20.0f;
			height = 10.0f;
		}
		
		width /= 2.0f;
		height /= 2.0f;

			//front
		box[0] = info.position[0] - width;
		box[1] = info.position[1] - height;
		box[2] = info.position[2] + width; //+z
		box[3] = info.position[0] - width;
		box[4] = info.position[1] + height;
		box[5] = info.position[2] + width;//+z
		box[6] = info.position[0] + width;
		box[7] = info.position[1] + height;
		box[8] = info.position[2] + width;//+z
		box[9] = info.position[0] + width;
		box[10] = info.position[1] - height;
		box[11] = info.position[2] + width;//+z
			
		//back
		box[12] = info.position[0] - width;
		box[13] = info.position[1] - height;
		box[14] = info.position[2] - width;
		box[15] = info.position[0] - width;
		box[16] = info.position[1] + height;
		box[17] = info.position[2] - width;
		box[18] = info.position[0] + width;
		box[19] = info.position[1] + height;
		box[20] = info.position[2] - width;
		box[21] = info.position[0] + width;
		box[22] = info.position[1] - height;
		box[23] = info.position[2] - width;
		
		
	}

	void getPoint(int n, float* point)
	{
		if(type == PROJECTILE_MISSILE)
		{
			point[0] = info.position[0];
			point[1] = info.position[1];
			point[2] = info.position[2];
		}
		else if(type == PROJECTILE_MINIGUN)
		{
			float dist;
			if(n==0) dist = 8.0f;
			else if(n==1) dist = 0.0f;
			else dist = 4.0f;

			point[0] = info.position[0] + dist*(float)cos((info.orientation[2]-90)*M_PI/180.0f);
			point[1] = info.position[1];
			point[2] = info.position[2] + dist*(float)sin((info.orientation[2]-90)*M_PI/180.0f);
		}
		else if (type == PROJECTILE_SPEED_MISSILE || type == PROJECTILE_POWER_MISSILE) {
			float dist;
			if(n==0) dist = 4.0f;
			else if(n==1) dist = 0.0f;

			point[0] = info.position[0] + dist*(float)cos((info.orientation[2]-90)*M_PI/180.0f);
			point[1] = info.position[1];
			point[2] = info.position[2] + dist*(float)sin((info.orientation[2]-90)*M_PI/180.0f);
		}
		else if(type == PROJECTILE_BOMB)
		{
			float dist = 2.0f;
			float xOffset = 0.0f;
			float zOffset = 0.0f;

			if(n==0)
			{
				xOffset = dist;
			}
			else if(n==1)
			{
				zOffset = dist;
			}
			else if(n==2)
			{
				xOffset = -1.0f * dist;
			}
			else if(n==3)
			{
				zOffset = -1.0f * dist;
			}

			point[0] = info.position[0] + xOffset;
			point[1] = info.position[1];
			point[2] = info.position[2] + zOffset;
		}
	}

	void getCenterOfCollidable(float* box)
	{
		box[0] = info.position[0];
		box[1] = info.position[1];
		box[2] = info.position[2];
	}


	
	CollidableType getCollidableType( void ) 
	{
		return type;
	}

	bool isPhysicsActive( void )
	{
		//return true if physics have been enabled
		return physicsEnabled;
	}

	void registerCollision(CollidableType cT, int collidingBoxNum)
	{
		if(type == PROJECTILE_BOMB)
		{
			if(cT == OBSTACLE)
			{
				if(collidingBoxNum == 0 ||
					collidingBoxNum == 2) //reverse the X
				{
					info.velocity[0] *= -1.0f;
				}
				else if(collidingBoxNum == 1 ||
						collidingBoxNum == 3)
				{
					info.velocity[2] *= -1.0f;
				}
			}
			else
			{
				currentCollisions.push_back(cT);
			}
		}
		else
		{
			currentCollisions.push_back(cT);
		}

	}

	void processCollisions( void )
	{
		if(isColliding())
		{
			speed = 0.0f;

			shouldBeDeleted = true;
			if(type == PROJECTILE_MINE) {
				sE = soundContainer->get(1);
				sE->setPosition(info.position);
				sE->setVelocity(info.velocity);
				sE->play();
				effectContainer->add(FIREBALL, info.position);
				effectContainer->add(CRACK, info.position);
			} else if(type == PROJECTILE_BOMB) {
				sE = soundContainer->get(1);
				sE->setPosition(info.position);
				sE->setVelocity(info.velocity);
				sE->play();
				effectContainer->add(FIREBALL_SMALL, info.position);
				effectContainer->add(CRACK, info.position);
			} else if (type == PROJECTILE_MINIGUN ) {
				sE = soundContainer->get(2);
				sE->setPosition(info.position);
				sE->setVelocity(info.velocity);
				sE->play();
				effectContainer->add(SPARK, info.position);
			}
			else if (type == PROJECTILE_MISSILE ) {
				s->stop();
				sE = soundContainer->get(1);
				sE->setPosition(info.position);
				sE->setVelocity(info.velocity);
				sE->play();
				effectContainer->add(FIREBALL_SMALL, info.position);
			}
			else if (type == PROJECTILE_SPEED_MISSILE ) {
				s->stop();
				sE = soundContainer->get(1);
				sE->setPosition(info.position);
				sE->setVelocity(info.velocity);
				sE->play();
				effectContainer->add(FIREBALL_SMALL, info.position);
			}
			else if (type == PROJECTILE_POWER_MISSILE ) {
				s->stop();
				sE = soundContainer->get(1);
				sE->setPosition(info.position);
				sE->setVelocity(info.velocity);
				sE->play();
				effectContainer->add(FIREBALL_SMALL, info.position);
			}
			else
			{
				effectContainer->add(CRACK, info.position);
			}
		}
		else
		{
			//physicsEnabled = true;
		}
		
		clearCurrentCollisions();
	}

};

class ProjectileContainer{
	vector<Projectile*> list;
	Render* renderer;
	Move* mover;
	Physics* physics;
	EffectContainer* eC;
	ModelContainer* mC;

	SoundContainer* sC;
public:
	ProjectileContainer(Render* r,
						Move* m,
						Physics* p,
						EffectContainer* effectC,
						SoundContainer* soundC,
						ModelContainer* modelC){

		list = vector<Projectile*>();
		renderer = r;
		mover = m;
		physics = p;
		eC = effectC;
		mC = modelC;
		sC = soundC;
	}
	~ProjectileContainer()
	{
		while(list.size()>0)
		{
			Projectile* v = list.at(0);
			delete v;
			list.erase(list.begin());
		}
	}

	//Creates and allocates new Projectile
	void add( CollidableType cT, WeaponInfo wI )
	{
		Model* m = NULL;
		if(cT == PROJECTILE_MINE)
			m = mC->add("Resources/mine.obj");
		else if(cT == PROJECTILE_MINIGUN)
			m = mC->add("Resources/minigun.obj");
		else if(cT == PROJECTILE_MISSILE || cT == PROJECTILE_SPEED_MISSILE || cT == PROJECTILE_POWER_MISSILE)
			m = mC->add("Resources/missile.obj");
		else if(cT == PROJECTILE_BOMB)
			m = mC->add("Resources/bomb.obj");

		Projectile* v = new Projectile(cT, wI, eC, sC, m);
		list.push_back(v);
		renderer->add(v);
		mover->add(v);
		physics->add(v);
	}

	void remove( int i )
	{
		Projectile* v = list.at(i);
		delete v;
		list.erase(list.begin()+i);
		v = NULL;
	}

	Projectile* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}

	//goes through all projectiles and removes them from all
	//managers if they need to be deleted
	void clean( void )
	{
		//unsigned int n = list.size();

		for(unsigned int i = 0; i < (int) list.size(); i++)
		{
			Projectile* p = list.at(i);

			if(p->shouldBeRemoved())
			{
				renderer->remove(p);
				mover->remove(p);
				physics->removeAsMoving(p);
				remove(i);
				i--; 
			}
		}
	}

	void cleanAll( void )
	{
		//unsigned int n = list.size();

		for(unsigned int i = 0; i < (int) list.size(); i++)
		{
			Projectile* p = list.at(i);

			renderer->remove(p);
			mover->remove(p);
			physics->removeAsMoving(p);
			remove(i);
			i--; 
		}
	}

};

#endif