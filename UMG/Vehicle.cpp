#include "Vehicle.h"


Vehicle::Vehicle(int num, Model* m, GLuint t)
{
	myVehicleNum = num;
	myModel = m;
	textureNumber = t;

	position[0] = 500.0f;
	position[1] = 0.0f;
	position[2] = 50.0f;
	
	velocity[0] = 0.0f;
	velocity[1] = 0.0f;
	velocity[2] = 0.0f;

	orientation[0] = 0.0f;
	orientation[1] = 0.0f;
	orientation[2] = 0.0f;
	
	color[0] = 0.0f;
	color[1] = 0.4f;
	color[2] = 0.4f;
	color[3] = 1.0f;

	speed = 0.0f;
	topSpeed = 10.0f;
	rateOfTurn = 0.0f;
	topRateOfTurn = 2.0f;
	speedDecayFactor = 0.99f;
	rateOfTurnDecayFactor = 0.9f;

	hitPoints = 100.0f;
	ammoRemaining = 0;
	totalAmmo = 100;

	physicsEnabled = true;
	collisionDetected = false;

	timer = SystemTimer();
	timer.startTimer();

	cT = NONE;

	shouldBeDeleted = false;
}


Vehicle::~Vehicle(void)
{
}

void Vehicle::render(void)
{
	if(hitPoints <= 0.0f)
		return;

	//Render the vehicle model
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);

		glRotatef(orientation[0], 0.0f, 0.0f, 1.0f); //roll (degrees)
		glRotatef(orientation[1], 1.0f, 0.0f, 0.0f); //pitch
		glRotatef(-1*orientation[2], 0.0f, 1.0f, 0.0f); //yaw
	
		glColor4f(1.0f, 1.0f, 1.0f, color[3]);
		glPushMatrix();
			glScalef(0.5f, 0.5f, 0.5f);
		
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureNumber);

			myModel->render();
		glPopMatrix();

		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		//Render the health bar
			if(myVehicleNum == 0) //player vehicle
			{
				//health bar
				glPushMatrix();
					glTranslatef(-2.5f, 0.9f, -0.5f + 0.5f*hitPoints/100.0f);
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
					PrimitiveCube healthRemainingCube(1.0f, false);
					glScalef(0.1f, 0.1f, 1.02f*hitPoints/100.0f);
					healthRemainingCube.render();
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-2.5f, 0.9f, 0.5f - 0.5f*(1 - hitPoints/100.0f));
					glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
					PrimitiveCube healthDifferenceCube(1.0f, false);
					glScalef(0.1f, 0.1f, (1.0f - hitPoints/100.0f));
					healthDifferenceCube.render();
				glPopMatrix();

				//ammo bar
				float ammoPercent = (float)ammoRemaining / (float)totalAmmo;
				glPushMatrix();
					glTranslatef(-2.5f, 1.0f, -0.5f + 0.5f*ammoPercent);
					glColor4f(0.0f, 0.0f, 0.8f, 1.0f);
					PrimitiveCube ammoRemainingCube(1.0f, false);
					glScalef(0.1f, 0.1f, ammoPercent);
					ammoRemainingCube.render();
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-2.5f, 1.0f, 0.5f - 0.5f*(1 - ammoPercent));
					glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
					PrimitiveCube ammoDifferenceCube(1.0f, false);
					glScalef(0.1f, 0.1f, 1.02f*(1.0f - ammoPercent));
					ammoDifferenceCube.render();
				glPopMatrix();

			}
			else
			{	
				//health bar
				glPushMatrix();
					glTranslatef(0.0f, 2.0f, -0.5f + 0.5f*hitPoints/100.0f);
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
					PrimitiveCube healthRemainingCube(1.0f, false);
					glScalef(0.1f, 0.1f, 1.02f*hitPoints/100.0f);
					healthRemainingCube.render();
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0.0f, 2.0f, 0.5f - 0.5f*(1 - hitPoints/100.0f));
					glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
					PrimitiveCube healthDifferenceCube(1.0f, false);
					glScalef(0.1f, 0.1f, (1.0f - hitPoints/100.0f));
					healthDifferenceCube.render();
				glPopMatrix();
			}



	glPopMatrix();
}


void Vehicle::move(void)
{
	if(hitPoints <=0.0f)
		return;

	if(speed > topSpeed) speed=topSpeed;
	else if(speed < -1.0f*topSpeed) speed=-topSpeed;

	if(rateOfTurn > topRateOfTurn) rateOfTurn=topRateOfTurn;
	else if(rateOfTurn < -1.0f*topRateOfTurn) rateOfTurn = -topRateOfTurn;

	orientation[2] += rateOfTurn;

	velocity[0] = speed*(float)cos(orientation[2]*M_PI/180.0);
	velocity[1] = 0.0f;
	velocity[2] = speed*(float)sin(orientation[2]*M_PI/180.0);

	position[0] = position[0] + velocity[0];
	position[2] = position[2] + velocity[2];

	speed = speed * speedDecayFactor;
	rateOfTurn = rateOfTurn * rateOfTurnDecayFactor;
	

}

bool Vehicle::canShoot(void)
{
	if( cT == NONE || ammoRemaining == 0 || hitPoints <= 0.0f) return false;
	else return true;
	
}

void Vehicle::setAmmo(int n)
{
	ammoRemaining = n;
}

void Vehicle::decrementAmmo(void)
{
	ammoRemaining -= 1;
}
	

void Vehicle::setPosition(float* p)
{
		position[0] = p[0];
		position[1] = p[1];
		position[2] = p[2];
}
void Vehicle::getPosition(float* p)
{
		p[0] = position[0];
		p[1] = position[1];
		p[2] = position[2];
}

void Vehicle::setVelocity(float* p)
{
	if(hitPoints<=0.0f) return;

		velocity[0] = p[0];
		velocity[1] = p[1];
		velocity[2] = p[2];
}
void Vehicle::getVelocity(float* p)
{
		p[0] = velocity[0];
		p[1] = velocity[1];
		p[2] = velocity[2];
}

void Vehicle::setOrientation(float* p)
{
		orientation[0] = p[0];
		orientation[1] = p[1];
		orientation[2] = p[2];
}
void Vehicle::getOrientation(float* p)
{
		p[0] = orientation[0];
		p[1] = orientation[1];
		p[2] = orientation[2];
}

void Vehicle::setColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Vehicle::setSpeed(float s)
{
		speed = s;
}

float Vehicle::getSpeed(void)
{
		return speed;
}

void Vehicle::setRateOfTurn(float r)
{
	if(hitPoints>0.0f)
		rateOfTurn = r;
}

float Vehicle::getRateOfTurn()
{
	return rateOfTurn;
}

float Vehicle::getTopSpeed(void)
{
	return topSpeed;
}

void Vehicle::setDecayFactor(float s)
{
	speedDecayFactor = s;
}

float Vehicle::getDecayFactor(void)
{
	return speedDecayFactor;
}

CollidableType Vehicle::getWeaponType(void)
{
	return cT;
}

int Vehicle::getAmmoRemaining(void)
{
	return ammoRemaining;
}

void Vehicle::setCollidableType(CollidableType type)
{
	cT = type;
}

int Vehicle::getNumElements(void)
{
	return 1;
}

void Vehicle::getPoint(int n, float* box)
{
}

void Vehicle::getBox(int n, float* box)
{
	float length = 5.0f;
	float width = 3.0f;
	float height = 1.5f;
	float yaw = orientation[2];
	
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

	//front is the face of the cube in the +Z axis
	box[0] = minX; //-X (bottom left)
	box[1] = 0.0f;  
	box[2] = maxZ; 
	box[3] = minX; //-X (top left)
	box[4] = height;  
	box[5] = maxZ;
	box[6] = maxX; //+X (top right)
	box[7] = height;
	box[8] = maxZ;
	box[9] = maxX; //+X (bottom right)
	box[10] = 0.0f;
	box[11] = maxZ;
			
	//back (-Z direction)
	box[12] = minX;
	box[13] = 0.0f;
	box[14] = minZ;
	box[15] = minX;
	box[16] = height;
	box[17] = minZ;
	box[18] = maxX;
	box[19] = height;
	box[20] = minZ;
	box[21] = maxX;
	box[22] = 0.0f;
	box[23] = minZ;
	
}
void Vehicle::getCenterOfCollidable(float* f)
{
	f[0] = position[0];
	f[1] = position[1];
	f[2] = position[2];
}
CollidableType Vehicle::getCollidableType(void)
{ 
	return VEHICLE;
}	

void Vehicle::registerCollision(CollidableType cT,  int collidingBoxNum)
{
	currentCollisions.push_back(cT);
}

void Vehicle::processCollisions(void)
{
	if(isColliding())
	{

		for(int x = 0; x < (int)currentCollisions.size(); x++)
		{
			CollidableType c = currentCollisions[x];
			if(c == ITEM_MINIGUN ||
				c == ITEM_MINE ||
				c == ITEM_MISSILE ||
				c == ITEM_BOMB ||
				c == ITEM_HEALTH ||
				c == ITEM_SPEED_MISSILE ||
				c == ITEM_POWER_MISSILE)
			{
				cT = c;					//Copy the item type to vehicle
				timer.startTimer();		//Start timer (for weapon deployment like mines)

				if(c == ITEM_MINIGUN)
				{
					ammoRemaining = MINIGUN_MAX_ROUNDS;
					totalAmmo = MINIGUN_MAX_ROUNDS;
				}
				else if(c == ITEM_MINE)
				{
					ammoRemaining = MINE_MAX_ROUNDS;
					totalAmmo = MINE_MAX_ROUNDS;
				}
				else if(c == ITEM_MISSILE)
				{
					ammoRemaining = MISSILE_MAX_ROUNDS;
					totalAmmo = MISSILE_MAX_ROUNDS;
				}
				else if(c == ITEM_BOMB)
				{
					ammoRemaining = BOMB_MAX_ROUNDS;
					totalAmmo = BOMB_MAX_ROUNDS;
				}
				else if(c == ITEM_SPEED_MISSILE)
				{
					ammoRemaining = SMISSILE_MAX_ROUNDS;
					totalAmmo = SMISSILE_MAX_ROUNDS;
				}
				else if(c == ITEM_POWER_MISSILE)
				{
					ammoRemaining = PMISSILE_MAX_ROUNDS;
					totalAmmo = PMISSILE_MAX_ROUNDS;
				}
				else if(c == ITEM_HEALTH)
				{
					float health = getHP();
					health = health + ((100.0f - health)*0.5f);
					setHP(health);
					ammoRemaining = 0.0f;
					cT = NONE;
				}
			}

			if(currentCollisions.at(x) == OBSTACLE)
			{
				if(myVehicleNum == 0)
				{
					position[0] = position[0] - velocity[0];
					position[2] = position[2] - velocity[2];
					speed = 0.0f;
				}
			}

			if(currentCollisions[x] == VEHICLE)
			{
				color[3] = 0.1f;
			}

#ifndef GOD_MODE
			if(currentCollisions[x] == PROJECTILE_MINIGUN)
			{
				hitPoints -= MINIGUN_DAMAGE;
			}
			else if(currentCollisions[x] == PROJECTILE_MINE)
			{
				hitPoints -= MINE_DAMAGE;
			}
			else if(currentCollisions[x] == PROJECTILE_MISSILE)
			{
				hitPoints -= MISSILE_DAMAGE;
			}
			else if(currentCollisions[x] == PROJECTILE_BOMB)
			{
				hitPoints -= BOMB_DAMAGE;
			}
			else if(currentCollisions[x] == PROJECTILE_SPEED_MISSILE)
			{
				hitPoints -= SMISSILE_DAMAGE;
			}
			else if(currentCollisions[x] == PROJECTILE_POWER_MISSILE)
			{
				hitPoints -= PMISSILE_DAMAGE;
			}
#endif

			if(hitPoints<=0.0f) //We're dead.
			{
				clearCurrentCollisions();


				//to allow vehicles to respawn comment shouldBeDeleted
				//and uncomment the reset player call
				physicsEnabled = false;
				shouldBeDeleted = true;
				hitPoints = 0.0f; //so we can't go below zero health!
			}

		}

	}
	else
	{
		color[3] = 1.0f;
	}

	clearCurrentCollisions();
}

void Vehicle::resetPlayer(void)
{
	hitPoints = 100.0f;
	position[0] = 50.0f;
	position[1] = 0.0f;
	position[2] = -50.0f;
	orientation[2] = 0.0f;
	speed = 0.0f;
	rateOfTurn = 0.0f;
	cT = NONE;
	ammoRemaining = 0;
	physicsEnabled = true;
}

bool Vehicle::isPhysicsActive(void)
{
	//return true if physics have been enabled
	return physicsEnabled;
}

void Vehicle::setPhysicsActive(bool in)
{
	physicsEnabled = in;
}