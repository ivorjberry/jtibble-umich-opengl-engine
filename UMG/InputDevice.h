#pragma once
#include "Menu.h"
#include "Vehicle.h"
#include "Projectile.h"
#include "Sound.h"
#include "Camera.h"
#include "Physics.h"
#include "Settings.h"

#include <windows.h>
#include <iostream>

class InputDevice
{
	MenuContainer* menuContainer;
	VehicleContainer* vehicleContainer;
	ProjectileContainer* projectileContainer;
	SoundContainer* soundContainer;
	Camera* camera;
	Physics* physics;
	int mouseX;
	int mouseY;
	int windowXSize;
	int windowYSize;
	float lastValidAimPoint[3];
	
	bool getAimPoint(float*);
	void getProjectilePoint(CollidableType wT, WeaponInfo &);//, float*);

public:
	InputDevice(MenuContainer*,
				VehicleContainer*,
				ProjectileContainer*,
				SoundContainer*,
				Camera*,
				Physics*,
				int, int );

	void FireWeapon(int, float*);
	void MoveReticle(LPARAM lp);

	void ProcessKeyboard(); //normal gameplay
	menuReturnValue ProcessMenu(); //when menus are active


	void changeCamera(WPARAM w);
	~InputDevice(void);
};

