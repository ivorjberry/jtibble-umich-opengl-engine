#include "InputDevice.h"

#define VK_a 65
#define VK_b 66
#define VK_d 68
#define VK_p 80
#define VK_s 83
#define VK_t 84
#define VK_w 87
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32




InputDevice::InputDevice(	MenuContainer* mC,
							VehicleContainer* vC, 
							ProjectileContainer* pC,
							SoundContainer* sC,
							Camera* c,
							Physics* p,
							int windowX, int windowY )
{ 
	menuContainer = mC;
	vehicleContainer = vC;
	projectileContainer = pC;
	soundContainer = sC;
	camera = c;
	physics = p;
	mouseX = 0;
	mouseY = 0;
	windowXSize = windowX;
	windowYSize = windowY;
	lastValidAimPoint[0] = 0.0f;
	lastValidAimPoint[1] = 0.0f;
	lastValidAimPoint[2] = 0.0f;
}

bool InputDevice::getAimPoint(float* aimPoint)
{
	Vehicle* playerV = vehicleContainer->get(0);

	if(playerV == NULL) return false;

	if( ! playerV->canShoot() ) return false;
	
	//Get the x-y-z location of the cursor, projected into 3D space	

	//to find the x,y, and z location, we need the 
	//modelview and projection matricies, and the viewport
	double modelViewTemp[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewTemp);
	double projectionViewTemp[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projectionViewTemp);
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
				
	double obj1X, obj1Y, obj1Z, obj2X, obj2Y, obj2Z;
	double mX = (double) mouseX;
	double mY = windowYSize - (double)mouseY;//(double) mouseY;
		
	gluUnProject(mX,mY,0.0,modelViewTemp,projectionViewTemp,viewport,&obj1X,&obj1Y,&obj1Z);
	gluUnProject(mX,mY,1.0,modelViewTemp,projectionViewTemp,viewport,&obj2X,&obj2Y,&obj2Z);	
		
	float vX=(float)(obj2X-obj1X);
	float vY=(float)(obj2Y-obj1Y);
	float vZ=(float)(obj2Z-obj1Z);

	float cameraPos[3];
	camera->getPosition(cameraPos);

	float distanceToGround = cameraPos[1] / vY;

	//float aimPoint[3];
	if(distanceToGround > 0) return false;


	aimPoint[0] = cameraPos[0] - (distanceToGround * vX);
	aimPoint[1] = 0.0f;
	aimPoint[2] = cameraPos[2] - (distanceToGround * vZ);

	lastValidAimPoint[0] = aimPoint[0];
	lastValidAimPoint[1] = aimPoint[1];
	lastValidAimPoint[2] = aimPoint[2];

	return true;
}


void InputDevice::getProjectilePoint( CollidableType cT, WeaponInfo & wI)//, float* mountOrientation )
{
	//We can translate/rotate to the mountpoint,
	//and then translate/rotate to the muzzle/exit aperture of the 
	//weapon, where the projectile will be started from

	
	float mountPosition[3];
	float mountOrientation[3];
	float length;
	float projectileSpeed;

	switch (cT) {

	case ITEM_MINIGUN :
		mountPosition[0] = MINIGUN_MOUNT_POSITION_X;
		mountPosition[1] = MINIGUN_MOUNT_POSITION_Y;
		mountPosition[2] = MINIGUN_MOUNT_POSITION_Z;

		mountOrientation[0] = MINIGUN_MOUNT_ORIENTATION_ROLL;
		mountOrientation[1] = MINIGUN_MOUNT_ORIENTATION_PITCH;
		mountOrientation[2] = MINIGUN_MOUNT_ORIENTATION_YAW;

		length = MINIGUN_MOUNT_LENGTH;

		projectileSpeed = MINIGUN_SPEED;
		break;

	case ITEM_MINE :
		mountPosition[0] = MINE_MOUNT_POSITION_X;
		mountPosition[1] = MINE_MOUNT_POSITION_Y;
		mountPosition[2] = MINE_MOUNT_POSITION_Z;

		mountOrientation[0] = MINE_MOUNT_ORIENTATION_ROLL;
		mountOrientation[1] = MINE_MOUNT_ORIENTATION_PITCH;
		mountOrientation[2] = MINE_MOUNT_ORIENTATION_YAW;

		length = MINE_MOUNT_LENGTH;

		projectileSpeed = MINE_SPEED;
		break;

	case ITEM_BOMB :
		mountPosition[0] = BOMB_MOUNT_POSITION_X;
		mountPosition[1] = BOMB_MOUNT_POSITION_Y;
		mountPosition[2] = BOMB_MOUNT_POSITION_Z;

		mountOrientation[0] = BOMB_MOUNT_ORIENTATION_ROLL;
		mountOrientation[1] = BOMB_MOUNT_ORIENTATION_PITCH;
		mountOrientation[2] = BOMB_MOUNT_ORIENTATION_YAW;

		length = BOMB_MOUNT_LENGTH;

		projectileSpeed = BOMB_SPEED;
		break;

	case ITEM_MISSILE :
		mountPosition[0] = MISSILE_MOUNT_POSITION_X;
		mountPosition[1] = MISSILE_MOUNT_POSITION_Y;
		mountPosition[2] = MISSILE_MOUNT_POSITION_Z;

		mountOrientation[0] = MISSILE_MOUNT_ORIENTATION_ROLL;
		mountOrientation[1] = MISSILE_MOUNT_ORIENTATION_PITCH;
		mountOrientation[2] = MISSILE_MOUNT_ORIENTATION_YAW;

		length = MISSILE_MOUNT_LENGTH;

		projectileSpeed = MISSILE_SPEED;
		break;

	case ITEM_SPEED_MISSILE :
		mountPosition[0] = SMISSILE_MOUNT_POSITION_X;
		mountPosition[1] = SMISSILE_MOUNT_POSITION_Y;
		mountPosition[2] = SMISSILE_MOUNT_POSITION_Z;

		mountOrientation[0] = SMISSILE_MOUNT_ORIENTATION_ROLL;
		mountOrientation[1] = SMISSILE_MOUNT_ORIENTATION_PITCH;
		mountOrientation[2] = SMISSILE_MOUNT_ORIENTATION_YAW;

		length = SMISSILE_MOUNT_LENGTH;

		projectileSpeed = SMISSILE_SPEED;
		break;

	case ITEM_POWER_MISSILE :
		mountPosition[0] = PMISSILE_MOUNT_POSITION_X;
		mountPosition[1] = PMISSILE_MOUNT_POSITION_Y;
		mountPosition[2] = PMISSILE_MOUNT_POSITION_Z;

		mountOrientation[0] = PMISSILE_MOUNT_ORIENTATION_ROLL;
		mountOrientation[1] = PMISSILE_MOUNT_ORIENTATION_PITCH;
		mountOrientation[2] = PMISSILE_MOUNT_ORIENTATION_YAW;

		length = PMISSILE_MOUNT_LENGTH;

		projectileSpeed = PMISSILE_SPEED;
		break;

	default : 
		mountPosition[0] = 0.0f;
		mountPosition[1] = 0.0f;
		mountPosition[2] = 0.0f;

		mountOrientation[0] = 0.0f;
		mountOrientation[1] = 0.0f;
		mountOrientation[2] = 0.0f;

		length = 0.0f;

		projectileSpeed = 0.0f;
	}
	wI.position[0] += mountPosition[0];
	wI.position[1] += mountPosition[1];
	wI.position[2] += mountPosition[2];
	
	//do trig to find end of barrel
	//translate from mountpoint to projectile exit-point
	
	wI.position[0] += length * (float)sin((mountOrientation[2]-wI.orientation[2])*M_PI/180.0);
	wI.position[2] += length * (float)cos((mountOrientation[2]-wI.orientation[2])*M_PI/180.0);
	
	
	wI.orientation[0] += mountOrientation[0];
	wI.orientation[1] += mountOrientation[1];
	wI.orientation[2] += mountOrientation[2];

	
	wI.speed = projectileSpeed;
	wI.velocity[0] = projectileSpeed * (float)cos(-1.0f*(mountOrientation[2]-wI.orientation[2])*M_PI/180.0);
	wI.velocity[1] = projectileSpeed * (float)sin(mountOrientation[1]*M_PI/180.0);
	wI.velocity[2] = projectileSpeed * (float)sin(-1.0f*(mountOrientation[2]-wI.orientation[2])*M_PI/180.0);

}


//If the AI is shooting, aimPoint will not be NULL; else, it will be
void InputDevice::FireWeapon(int vehicleNumber, float* aimPoint)
{
	
	//get info about vehicle that is firing its weapon
	Vehicle* v = vehicleContainer->get(vehicleNumber);

	if( v == NULL ) return;  //Does the vehicle exist?

	if( ! v->canShoot() ) return; //Is it able to fire its weapon?

	CollidableType cT = v->getWeaponType();
	float vehicleTime = v->getTime();
	float coolDownTime;
	
	switch (cT) {

	case ITEM_MINIGUN :
		coolDownTime = MINIGUN_COOLDOWN;
		break;

	case ITEM_MINE :
		coolDownTime = MINE_COOLDOWN;
		break;
		
	case ITEM_MISSILE :
		coolDownTime = MISSILE_COOLDOWN;
		break;

	case ITEM_BOMB :
		coolDownTime = BOMB_COOLDOWN;
		break;

	case ITEM_SPEED_MISSILE :
		coolDownTime = SMISSILE_COOLDOWN;
		break;

	case ITEM_POWER_MISSILE :
		coolDownTime = PMISSILE_COOLDOWN;
		break;

	default :
		coolDownTime = 0.0f;
	}

	if( vehicleTime < coolDownTime )
	{
		//v->resetTime();
		return;
	}

	WeaponInfo wI;

	//intersect the vector <vX,vY,vZ> with the ground plane
	if(aimPoint == NULL)
	{
		float newAimPoint[] = {0.0f, 0.0f, 0.0f};
		if( !getAimPoint(newAimPoint))
		{
			return;
		}
		else
		{
			wI.aimPoint[0] = newAimPoint[0];
			wI.aimPoint[1] = newAimPoint[1];
			wI.aimPoint[2] = newAimPoint[2];
		}
			//store the result into a WeaponInfo object
		
	}
	else
	{
		wI.aimPoint[0] = aimPoint[0];
		wI.aimPoint[1] = aimPoint[1];
		wI.aimPoint[2] = aimPoint[2];
	}



	v->getPosition(wI.position);
	v->getOrientation(wI.orientation);


	if(cT == ITEM_MINIGUN || cT == ITEM_BOMB || cT == ITEM_SPEED_MISSILE || cT == ITEM_POWER_MISSILE)
	{
		float diffX = wI.aimPoint[0] - wI.position[0];
		float diffZ = wI.aimPoint[2] - wI.position[2];
		double tmpAngle = atan2( diffZ , diffX );
		float angleToAimPoint = (float)(tmpAngle * 180.0f / M_PI) - wI.orientation[2];

		while(angleToAimPoint < 0.0f) angleToAimPoint += 360.0f;
		while(angleToAimPoint > 360.0f) angleToAimPoint -= 360.0f;

		while(wI.orientation[2] < 0.0f)
			wI.orientation[2]+=360.0f;

		while(wI.orientation[2] > 360.0f)
			wI.orientation[2] -= 360.0f;

		float angleRelativeToVehicle = angleToAimPoint;

		while(angleRelativeToVehicle < 0.0f) angleRelativeToVehicle += 360.0f;
		while(angleRelativeToVehicle > 360.0f) angleRelativeToVehicle -= 360.0f;

		if(!(angleRelativeToVehicle < 45 || angleRelativeToVehicle > 315))
			return;

		wI.orientation[2] += angleToAimPoint;
	}
	
	if(cT == ITEM_MISSILE)
		wI.orientation[2] += (float)(rand()%90) - 45.0f;
	else if(cT == ITEM_MINIGUN)
		wI.orientation[2] += (float)(rand()%100)/100.0f - 0.5f;
	
	getProjectilePoint(cT, wI);

	
	CollidableType wT;
	if(cT == ITEM_MINIGUN) {
		wT = PROJECTILE_MINIGUN;
	} else if(cT == ITEM_MINE) {
		wT = PROJECTILE_MINE;
	} else if(cT == ITEM_MISSILE) {
		wT = PROJECTILE_MISSILE;
	} else if(cT == ITEM_BOMB) {
		wT = PROJECTILE_BOMB;
	} else if(cT == ITEM_SPEED_MISSILE) {
		wT = PROJECTILE_SPEED_MISSILE;
	} else if(cT == ITEM_POWER_MISSILE) {
		wT = PROJECTILE_POWER_MISSILE;
	} else {
		wT = NONE;
	}
	//make the projectile
	projectileContainer->add(wT, wI);
	
	v->resetTime();
	v->decrementAmmo();

}
void InputDevice::MoveReticle(LPARAM lp)
{
	mouseX = LOWORD(lp);
	mouseY = HIWORD(lp);
	//Need to create a reticle to draw at the given location of x and y
}

void InputDevice::ProcessKeyboard()
{
	Vehicle* v = vehicleContainer->get(0);
	float orientation[3];
		v->getOrientation(orientation);
	
	float currentSpeed = v->getSpeed();

	if (GetKeyState(VK_LBUTTON) < 0)
	{
		FireWeapon(0, NULL);
	}

	if (GetKeyState(VK_LEFT) < 0 || GetKeyState(VK_a) < 0)
	{
		float amount = 0.15f;
		if(v->getSpeed() < -0.05f)
			amount*=-1.0f;
		v->setRateOfTurn(v->getRateOfTurn() - amount);
	}
	
	if (GetKeyState(VK_UP) < 0 || GetKeyState(VK_w) < 0)
	{
		v->setSpeed( currentSpeed + 0.05f);
	}
	
	if (GetKeyState(VK_RIGHT) < 0 || GetKeyState(VK_d) < 0)
	{
		float amount = 0.15f;
		if(v->getSpeed() < -0.05f)
			amount*=-1.0f;
		v->setRateOfTurn(v->getRateOfTurn() + amount);
	}
	
	if (GetKeyState(VK_DOWN) < 0 || GetKeyState(VK_s) < 0)
	{
		v->setSpeed( currentSpeed - 0.05f);
	}
	
}


menuReturnValue InputDevice::ProcessMenu()
{
	if (GetKeyState(VK_LBUTTON)<0)
	{
		//handle the click with the menuContainer

		double modelViewTemp[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelViewTemp);
		double projectionViewTemp[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projectionViewTemp);
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
				
		double obj1X, obj1Y, obj1Z, obj2X, obj2Y, obj2Z;
		double mX = (double) mouseX;
		double mY = windowYSize - (double)mouseY;//(double) mouseY;
		
		gluUnProject(mX,mY,0.0,modelViewTemp,projectionViewTemp,viewport,&obj1X,&obj1Y,&obj1Z);
		gluUnProject(mX,mY,1.0,modelViewTemp,projectionViewTemp,viewport,&obj2X,&obj2Y,&obj2Z);

		double vX = (obj2X - obj1X);
		double vY = (obj2Y - obj1Y);
		double vZ = (obj2Z - obj1Z);

		double distance = 2.0 / vZ;


		
		double convertedMouseX = -vX*distance;
		double convertedMouseY = -vY*distance;
		return menuContainer->click(convertedMouseX, convertedMouseY);
	}
	else return MENU_RETURN_NONE;
}


void InputDevice::changeCamera(WPARAM w)
{

	//Camera rotate left
	if(w == (VK_1)){
		float angle = camera->getAngle() + 2;
		camera->setAngle(angle);
	}
	//Camera rotate right
	if(w == (VK_2)){
		float angle = camera->getAngle() - 2;
		camera->setAngle(angle);
	}
	//toggle rendering of bounding box
	/*if(w == (VK_b)){
		physics->flipBoundingBoxes();
	}
	//fade to/from black
	if(w == (VK_SPACE)){

	}*/
	//Pull camera back from vehicle
	if(w == (VK_PRIOR)){
		float dist = camera->getFollowDist() + 2;
		camera->setFollowDist(dist);
	}
	//Zoom closer to vehicle
	if(w == (VK_NEXT)){
		float dist = camera->getFollowDist() - 2;
		if(dist > 4) camera->setFollowDist(dist);
	}
	//reset camera 
	if(w == (VK_0)){
		camera->reset();
	}
	//empty physics
	/*if(w == (VK_p)){
		//enter call to empty physics
	}
	//Toggle vehicle that you are following
	if(w == (VK_t)){
		camera->toggleFollowVehicle();
	}*/
}
InputDevice::~InputDevice(void){ }