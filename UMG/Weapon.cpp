/*#include "Weapon.h"

Weapon::Weapon(vector<Projectile>* vP, WeaponInfo* wI, WeaponType wT)
{
	//myVehicle = v;			//store this so we can start projectiles
							//at the correct location with respect to the vehicle
	projectileList = vP;
	info = wI;
	type = wT;
	timer.startTime();
	
	
	if(wT == MINIGUN)
	{
		mountPosition[0] = 0.0f; //relative to center of vehicle (+0.6 X-axis)
		mountPosition[1] = 1.1f;
		mountPosition[2] = 0.0f;

		mountOrientation[0] = 0.0f; //roll - pitch - yaw
		mountOrientation[1] = 0.0f; //these are rotations around the mountPosition
		mountOrientation[2] = 90.0f; //degrees!

		length = 11.0f;	//to correctly position and orient rounds fired
					//(distance from mountPoint to exit aperture)
	
		//stats for the weapon
		roundsRemaining = 100;	//totally arbitrary
		projectileSpeed = 5.0f;
		projectileTopSpeed = 5.0f;
		damagePerShot = 1.0f;
		cooldownTime = 0.1f;
		reloadTime = 0.0f;
	}
	else if(wT == MINE)
	{
		mountPosition[0] = 0.0f;
		mountPosition[1] = 0.0f;
		mountPosition[2] = 0.0f;
				

		length = 0.0f;
		
		roundsRemaining = 10;	//totally arbitrary
		projectileSpeed = 0.0f;
		projectileTopSpeed = 0.0f;
		damagePerShot = 25.0f;
		cooldownTime = 1.0f;
		reloadTime = 0.0f;
		lifeTime = 10.0f;
	}
	else if(wT == MISSILE)
	{
		mountPosition[0] = 0.0f;
		mountPosition[1] = 1.0f;
		mountPosition[2] = 0.0f;
				

		length = 0.0f;
		
		roundsRemaining = 10;	//totally arbitrary
		projectileSpeed = 0.1f;
		projectileTopSpeed = 5.0f;
		damagePerShot = 25.0f;
		cooldownTime = 0.3f;
		reloadTime = 0.0f;
	}




}

Weapon::~Weapon()
{

}



void Weapon::render(void)
{
	glPushMatrix();
	//The weapon is rendered in the coordinate frame of the vehicle!
	if( type == MINIGUN )
	{
		glColor3f(0.3f, 0.3f, 0.3f);
		glTranslatef(mountPosition[0], mountPosition[1], mountPosition[2]);

		glRotatef(mountOrientation[0], 0.0f, 0.0f, 1.0f);	//roll
		glRotatef(mountOrientation[1], 1.0f, 0.0f, 0.0f);	//pitch
		glRotatef(mountOrientation[2], 0.0f, 1.0f, 0.0f);	//yaw

		glTranslatef(0.5, 0.0f, 0.0f);

		glScalef(1.0f, 0.1f, 0.1f);

		PrimitiveCube pc( 0.75 );
		pc.render();
	}
	else if( type == MINE )
	{

	}
	else if( type == MISSILE )
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(mountPosition[0], mountPosition[1], mountPosition[2]);
		
		glTranslatef(0.0f, 0.2f, 0.0f);

		PrimitiveCube pc( 0.2f );
		pc.render();
	}
	
	glPopMatrix();

	timer.incrFrame();
}



void Weapon::fire(float* groundPoint)
{
	//Don't fire the weapon if the weapon is still "cooling down"
	if(timer.getTime()<cooldownTime)
		return;
	//Start firing the weapon based on the vehicle's current position, orientation
	WeaponInfo pInfo = *info;


	//Because the weapon is created with a pointer to the Vehicle's 
	//WeaponInfo, we have to dereference it to get the current state 
	//of the vehicle. Then we can translate/rotate to the mountpoint,
	//and then translate/rotate to the muzzle/exit aperture of the 
	//weapon, where the projectile will be started from

	//if we got a groundPoint from the inputDevice, use it
	if( groundPoint != NULL)
	{
	pInfo.aimPoint[0] = groundPoint[0];
	pInfo.aimPoint[1] = groundPoint[1];
	pInfo.aimPoint[2] = groundPoint[2];
	}
	else
	{
		pInfo.aimPoint[0] = 0.0f;
		pInfo.aimPoint[1] = 0.0f;
		pInfo.aimPoint[2] = 0.0f;
	}

	pInfo.position[0] += mountPosition[0];
	pInfo.position[1] += mountPosition[1];
	pInfo.position[2] += mountPosition[2];
	

	//do trig to find end of barrel
	//translate from mountpoint to projectile exit-point
	pInfo.position[0] += length * (float)sin((mountOrientation[2]-pInfo.orientation[2])*M_PI/180.0);
	pInfo.position[2] += length * (float)cos((mountOrientation[2]-pInfo.orientation[2])*M_PI/180.0);
	
	
	pInfo.orientation[0] += mountOrientation[0];
	pInfo.orientation[1] += mountOrientation[1];
	pInfo.orientation[2] += mountOrientation[2];

	pInfo.velocity[0] += projectileSpeed * (float)cos(-1.0f*(mountOrientation[2]-pInfo.orientation[2])*M_PI/180.0);
	pInfo.velocity[1] = projectileSpeed * (float)sin(mountOrientation[1]*M_PI/180.0);
	pInfo.velocity[2] += projectileSpeed * (float)sin(-1.0f*(mountOrientation[2]-pInfo.orientation[2])*M_PI/180.0);

	

	if(type==MINIGUN)
	{
		pInfo.color[0] = 1.0;
		pInfo.color[1] = 242/255.0f;
		pInfo.color[2] = 0.0; //yellow
		pInfo.color[3] = 1.0f;
	}
	else if(type == MINE)
	{
		pInfo.color[0] = 0.0f;
		pInfo.color[1] = 1.0f;
		pInfo.color[2] = 0.0f;
		pInfo.color[3] = 1.0f;
	}
	else if(type == MISSILE)
	{
		pInfo.color[0] = 1.0f;
		pInfo.color[1] = 0.0f;
		pInfo.color[2] = 0.0f;
		pInfo.color[3] = 1.0f;
	}



//	Projectile p(pInfo, type);
	//projectileList->push_back(p);

	

	timer.startTime();
}


*/