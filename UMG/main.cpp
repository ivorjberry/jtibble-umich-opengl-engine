#include <time.h>
#include "Settings.h"

#include "InputDevice.h"
#include "World.h"
#include "FrameTimer.h"
#include "Render.h"
#include "MenuRender.h"
#include "Effect.h"
#include "Physics.h"
#include "Move.h"
#include "Camera.h"
#include "Projectile.h"
#include "SystemTimer.h"
#include "Target.h"
#include "Item.h"
#include "AI.h"
#include "Scenery.h"
#include "Texture.h"
#include "Model.h"
#include "Menu.h"
#include "Sound.h"

MSG			msg;		// Windows message structure

//Managers
Render* render;
MenuRender* menu;
Move* mover;
Physics* physics;


OpenGLContext* context;
InputDevice* input;
World* world;
FrameTimer* timer;
Camera* camera;
SystemTimer *milliTimer;
bool soundOn;
Sound* menuMusic;
Sound* gameMusic;

//testing, temp
SystemTimer* endGameTimer;
bool endGame;
bool winGame;
//Effect* effect;

//Global Container Classes
VehicleContainer* vehicleContainer;
EffectContainer* effectContainer;
ProjectileContainer* projectileContainer;
ObstacleContainer* obstacleContainer;
TargetContainer* targetContainer;
ItemContainer* itemContainer;
AIContainer* aiContainer;
SceneryContainer* sceneryContainer;
TextureContainer* textureContainer;
ModelContainer* modelContainer;
MenuContainer* menuContainer;
SoundContainer* soundContainer;

static LPCTSTR lpszAppName = L"Thrash 'N' Burn";

bool MENU_ACTIVE = false;

void killProgram(void){
			delete context;
			delete render;
			delete mover;
			delete physics;
			//delete world;
			//world = NULL;
			//delete input;
}

void addItems(levelChoice);
void initGame(levelChoice);
void initializeMenu(MenuContainer*);
void cleanVehicles(void);

// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(	HWND 	hWnd,
							UINT	message,
							WPARAM	wParam,
							LPARAM	lParam)
{
	if(context==NULL)
		return (0L);

	static HGLRC hRC;		// Permenant Rendering context
	static HDC hDC;			// Private GDI Device context

	

	switch (message)
	{
		// Window creation, setup for OpenGL
		case WM_CREATE:
			// Store the device context
			hDC = GetDC(hWnd);			

			// Select the pixel format
			context->SetDCPixelFormat(hDC);		

			// Create the rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);

			// Create a timer that fires 30 times a second
			SetTimer(hWnd,500/*33*/,1,NULL);
			break;

		// Window is being destroyed, cleanup
		case WM_DESTROY:
			// Kill the timer that we created
			KillTimer(hWnd,101);

			// Deselect the current rendering context and delete it
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);
			// Tell the application to terminate after the window
			// is gone.
			killProgram();
			PostQuitMessage(0);
			break;
        
		// Window is resized.
		case WM_SIZE:
			// Call our function which modifies the clipping
			// volume and viewport
			context->ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;

		// Timer does the work then invalidates the 
		// window so it will be redrawn.
		case WM_TIMER:
			{
				if(endGame)
				{
					float eTime = endGameTimer->elapsedTime();
					if(eTime > 2.0f && eTime <= 5.0f ){
						if(winGame)
							menuContainer->setActive(MENU_WIN);
						else
							menuContainer->setActive(MENU_LOSE);
						MENU_ACTIVE = true;
					}
					else if(eTime > 5.0f){
						if (soundOn == true) {
							if (!(menuMusic->isPlaying())) {
								gameMusic->stop();
								menuMusic->play();
							}
						}
						menuContainer->setActive(MENU_MAIN);
						endGame = false;
					}
				}
				
				if( !MENU_ACTIVE )
				{
					if(input!=NULL)
						input->ProcessKeyboard();

					//Update AI thoughts
					if(aiContainer!=NULL)
						aiContainer->thinkOnAll();

					if(physics!=NULL) 
						physics->step();
					
					if(mover!=NULL)
						mover->moveAll();
				
					if(camera!=NULL)
					{
						camera->followCurrent();
						//camera->render();
					}


					if(vehicleContainer != NULL)
					{
						Vehicle* v = vehicleContainer->get(0);
					
						ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
						ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
						ALfloat ListenerOri[] = { 0.0, 0.0, 0.0 };

						v->getPosition(ListenerPos);
						v->getVelocity(ListenerVel);
						v->getOrientation(ListenerOri);

						alListenerfv(AL_POSITION,    ListenerPos);
						alListenerfv(AL_VELOCITY,    ListenerVel);
						alListenerfv(AL_ORIENTATION, ListenerOri);
					}

					//clean up projectiles
					if(projectileContainer != NULL)
					{
						projectileContainer->clean();
					}
					if(targetContainer != NULL)
					{
						targetContainer->clean();
					}
					if(effectContainer != NULL)
					{
						effectContainer->clean();
					}
					if(vehicleContainer != NULL)
					{
						cleanVehicles();
					}
				}
				else //the menu is active
				{
					menuReturnValue returnVal;
					
					if(input!=NULL)
						returnVal = input->ProcessMenu();
					
					if(returnVal == MENU_RETURN_LEVEL1)
					{
						//make sure physics, render, move, etc are all cleared
						projectileContainer->cleanAll();
						effectContainer->cleanAll();
						targetContainer->cleanAll();
						//sceneryContainer->clean();
						obstacleContainer->clean();
						aiContainer->cleanAll();
						//clean up the world, render, move, physics,
						itemContainer->cleanAll();

						render->remove(world);

						menuContainer->setActive(MENU_LOADING);
						//launchGame(level1, #ais, item, settings);
						initGame(LEVEL_CHOICE_COLUMNS);

						menuContainer->setActive(MENU_PAUSE);
					}
					else if(returnVal == MENU_RETURN_LEVEL2)
					{
						//make sure physics, render, move, etc are all cleared
						projectileContainer->cleanAll();
						effectContainer->cleanAll();
						targetContainer->cleanAll();
						//sceneryContainer->clean();
						obstacleContainer->clean();
						aiContainer->cleanAll();
						//clean up the world, render, move, physics,
						itemContainer->cleanAll();
						render->remove(world);

						menuContainer->setActive(MENU_LOADING);

						if(menu!=NULL)
							menu->renderAll();

						initGame(LEVEL_CHOICE_MAZE);

						menuContainer->setActive(MENU_PAUSE);
					}
					else if(returnVal == MENU_RETURN_LEVEL3)
					{
						//make sure physics, render, move, etc are all cleared
						projectileContainer->cleanAll();
						effectContainer->cleanAll();
						targetContainer->cleanAll();
						//sceneryContainer->clean();
						obstacleContainer->clean();
						aiContainer->cleanAll();
						//clean up the world, render, move, physics,
						itemContainer->cleanAll();

						render->remove(world);
						
						menuContainer->setActive(MENU_LOADING);

						if(menu!=NULL)
							menu->renderAll();
						
						initGame(LEVEL_CHOICE_BLOCK_FORT);
						menuContainer->setActive(MENU_PAUSE);
					}
					else if(returnVal == MENU_RETURN_QUIT_GAME)
					{					
						// Kill the timer that we created
						KillTimer(hWnd,101);

						// Deselect the current rendering context and delete it
						wglMakeCurrent(hDC,NULL);
						wglDeleteContext(hRC);
	
						// Tell the application to terminate after the window
						// is gone.
						killProgram();
						PostQuitMessage(0);
					}
					else if(returnVal == MENU_RETURN_QUIT_ROUND)
					{
						if (soundOn == true) {
							if (!(menuMusic->isPlaying())) {
								gameMusic->stop();
								menuMusic->play();
							}
						}
						//set the menu back to MENU_MAIN
						menuContainer->setActive(MENU_MAIN);

					}
					else if(returnVal == MENU_RETURN_MUSIC_ON)
					{
						if (soundOn == false) {
							menuMusic->play();
							soundOn = true;
						}
						//set the menu back to MENU_MAIN
						menuContainer->setActive(MENU_MAIN);						
					}
					else if(returnVal == MENU_RETURN_MUSIC_OFF)
					{
						if (soundOn == true) {
							menuMusic->stop();
							soundOn = false;
						}
						//set the menu back to MENU_MAIN
						menuContainer->setActive(MENU_MAIN);						
					}
					else if(returnVal == MENU_RETURN_UNPAUSE)
					{
						//set the menu back to MENU_MAIN
						//menuContainer->setActive(MENU_MAIN);						
						MENU_ACTIVE = false;
					}
					
				}

			InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

			
		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
		case WM_PAINT:
			{
				
				if(render!=NULL){
					milliTimer->startTimer();
					render->renderAll();
				}

				if(MENU_ACTIVE)
				{
					if(menu!=NULL)
						menu->renderAll();
				}
					


				// Call function to swap the buffers
				SwapBuffers(hDC);

				// Validate the newly painted client area
				ValidateRect(hWnd,NULL);
			}
			break;
		//Left button click
		case WM_LBUTTONDOWN:
			{
				//input->FireWeapon(0, NULL);
			}
			break;

		case WM_MOUSEMOVE:
			{
				if(input!=NULL)
					input->MoveReticle(lParam);
			}
			break;
		//A keyboard event (arrows)
		case WM_KEYDOWN:
			{
				if(wParam == VK_ESCAPE)
				{
					if(world != NULL && menuContainer->getCurrentMenu() == MENU_PAUSE)
						MENU_ACTIVE = !MENU_ACTIVE;

				}
				/*if(wParam == 78){ //the n key
					aiContainer->toggleLOS();
				}*/
				else if(input!=NULL)
					input->changeCamera(wParam);

			}
			break;
		//Other keyboard events(for most other characters)
		case WM_CHAR:
			{
				NULL;
			}
			break;
        default:   // Passes it on if unproccessed
            return (DefWindowProc(hWnd, message, wParam, lParam));

    }

    return (0L);
}




// Entry point of all Windows programs
int APIENTRY WinMain(	HINSTANCE 	hInstance,
						HINSTANCE 	hPrevInstance,
						LPSTR 		lpCmdLine,
						int			nCmdShow)
{
	//Randomize the rand function
	srand( (unsigned int)time(NULL) );

	//Create OpenGL context window
	context = new OpenGLContext();
	
	context->initialize(	hInstance,
							hPrevInstance,
							lpCmdLine,
							lpszAppName,
							nCmdShow );
	

	//We need to set up GLEW
	glewExperimental;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		MessageBox(NULL, (LPCWSTR)L"GLEW failed to init", NULL, NULL);
		return msg.wParam;
	}

#ifdef ENABLE_SHADERS
	if(!GLEW_VERSION_2_1)//!glewIsSupported("GL_VERSION_2.1"))
	{
		MessageBox(NULL, (LPCWSTR)L"Your card does not support OpenGL 2.1. This game cannot run without it.", NULL, NULL);
		return msg.wParam;
	}
#endif

	/*
	// Light values and coordinates
				GLfloat  ambientLight[] = { 0.8f, 0.3f, 0.3f, 1.0f };
				GLfloat  diffuseLight[] = { 0.8f, 0.4f, 0.1f, 1.0f };
				GLfloat  positionLight[] = {500.0f, 20.0f, -500.0f, 0.0f};

				glFrontFace(GL_CW);		// Counter clock-wise polygons face out

				// Enable lighting
				glEnable(GL_LIGHTING);
				//glEnable(GL_NORMAL);

				// Setup and enable light 0
				glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
				glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
				glLightfv(GL_LIGHT0,GL_POSITION,positionLight);
				glEnable(GL_LIGHT0);
	*/
	//Initialize necessary elements/classes
	
	//Start time
	endGameTimer = new SystemTimer();
	timer = new FrameTimer();
	timer->startTime();
	milliTimer = new SystemTimer();


	//set up the engine classes needed by the Containers

	//Set up Render
	render = new Render();

	//Set up MenuRender
	menu = new MenuRender();

	//Set up Move(r)
	mover = new Move();

	//Set up Physics
	physics = new Physics( render ); //pass in render so we can have a chance of debugging the physics


	//Then, set up the Containers

	//Set up Container classes
	//Note, targetContainer and projectileContainer now need effect container to make targets explode, so
	//effectContainer MUST be initialized before targetContainer and projectileContainer
	textureContainer = new TextureContainer();
	menuContainer = new MenuContainer(menu, textureContainer);
	modelContainer = new ModelContainer(render);
	soundContainer = new SoundContainer();

	//load textures here
	int temp = textureContainer->add("Resources/fireball.tga");
	temp = textureContainer->add("Resources/crack2.tga");
	temp = textureContainer->add("Resources/Cloud.tga");
	GLuint vehicleTextureNumber = textureContainer->add("Resources/map.tga");


	//load models here
	Model* vehicleModel = modelContainer->add("Resources/vehicle final.obj");
	Model* tempModel = modelContainer->add("Resources/fireball.obj");
	tempModel = modelContainer->add("Resources/mine.obj");
	tempModel = modelContainer->add("Resources/missile.obj");
	tempModel = modelContainer->add("Resources/bomb.obj");
	tempModel = modelContainer->add("Resources/minigun.obj");

	vehicleContainer = new VehicleContainer(render, mover, physics, vehicleModel, vehicleTextureNumber);
	camera = new Camera(vehicleContainer, 0);
	effectContainer = new EffectContainer(camera, render, mover, textureContainer, modelContainer);
	projectileContainer = new ProjectileContainer(render, mover, physics, effectContainer, soundContainer, modelContainer);
	obstacleContainer = new ObstacleContainer(render, physics);
	targetContainer = new TargetContainer(render, mover, physics, effectContainer);
	itemContainer = new ItemContainer(render, physics);
	sceneryContainer = new SceneryContainer(render, textureContainer);

	//Initialize all sounds
	soundContainer->add("Resources/menu loop.wav"); //Placeholder for MenuMusic
	soundContainer->add("Resources/boom.wav"); //Placeholder for in game music
	soundContainer->add("Resources/metal_impact.wav");
	soundContainer->add("Resources/minigunshot.wav");
	soundContainer->add("Resources/missile.wav");
	soundContainer->add("Resources/game loop.wav");

	initializeMenu(menuContainer);

	//Next, initialize necessary devices that need the Containers

	//Create input device
	int windowDimensions[2];
	context->getWindowSize(windowDimensions);
	input = new InputDevice( menuContainer,
						vehicleContainer,		//to send input to appropriate vehicle
						projectileContainer,	//to fire weapons on mouseclick events
						soundContainer,
						camera,					//to calculate aimpoint as a function of camera, mouse position
						physics,				//to enable/disable the drawing of the bounding boxes
						windowDimensions[0], windowDimensions[1] );

	aiContainer = new AIContainer(	physics, 
									render,
									vehicleContainer,
									projectileContainer,
									obstacleContainer,
									itemContainer,
									input);
	//Set up Camera, add to render
	//Needs to be the first thing initialized after containers so it gets rendered correctly
	//You have to set the vehicle to start following with the camera

	render->add(camera);
	physics->add(camera);

	//Create light for the world
	Light* worldLight = new Light();
	float colorAmbient[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	float colorDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float colorSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float lightPosition[3] = {500.0f, 200.0f, -500.0f};
	worldLight->setColorAmbient(colorAmbient);
	worldLight->setColorDiffuse(colorDiffuse);
	worldLight->setColorSpecular(colorSpecular);
	worldLight->setPosition(lightPosition);
	render->add(worldLight);

	
	sceneryContainer->createStandardScenery();
	MENU_ACTIVE = true;
	soundOn = true;
	menuMusic = soundContainer->get(0);
	menuMusic->loop();
	menuMusic->play();
	endGame = false;
	winGame = false;
	// Process application messages until the application closes
	while( GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	//delete world;
	//what is this doing here?! I'm not sure this is appropriate any more
	delete input;
	return msg.wParam;
}

void initGame(levelChoice l)
{
	if (soundOn == true) {
		menuMusic->stop();
		gameMusic = soundContainer->get(5);
		gameMusic->loop();
		gameMusic->play();
	}

	endGame = false;
	winGame = false;
	if(world != NULL)
		delete world;
	
	
	//Initialize the world
	world = new World(obstacleContainer, targetContainer, l);

	render->add(world);

	//add a new vehicle (player vehicle, #0)
	if(vehicleContainer->numberOfVehicles() == 0)
	{
		//Add Player
		vehicleContainer->add();

		//Add dummy vehicl
		vehicleContainer->add();
		//Add dummy vehic2
		vehicleContainer->add();
		//Add dummy vehic3
		vehicleContainer->add();
	}
	Vehicle* v = vehicleContainer->get(0);
	v->resetPlayer();
	v->inPlay();
	

	

	//AI VEHICLES
	float position5[3]={900.0, 0.0, -950.0};
	v = vehicleContainer->get(1);
	v->inPlay();
	v->resetHP();
	v->setPosition(position5);
	float orientation[] = {0.0f, 0.0f, (float)(rand()%360)};
	v->setOrientation(orientation);
	v->setSpeed(1.2f);
	v->setDecayFactor(1.0f);
	v->setPhysicsActive(true);

	
	//2
	float position6[3]={900.0, 0.0, -950.0};
	v = vehicleContainer->get(2);
	v->inPlay();
	v->resetHP();
	v->setPosition(position6);
	float orientation1[] = {0.0f, 0.0f, (float)(rand()%360)};
	v->setOrientation(orientation1);
	v->setSpeed(1.2f);
	v->setDecayFactor(1.0f);
	v->setPhysicsActive(true);

	
	//3
	float position7[3]={900.0, 0.0, -950.0};
	v = vehicleContainer->get(3);
	v->inPlay();
	v->resetHP();
	v->setPosition(position7);
	float orientation2[] = {0.0f, 0.0f, (float)(rand()%360)};
	v->setOrientation(orientation2);
	v->setSpeed(1.2f);
	v->setDecayFactor(1.0f);
	v->setPhysicsActive(true);

	aiContainer->add(1);
	aiContainer->add(2);
	aiContainer->add(3);



	addItems(l);

	MENU_ACTIVE = false;
	return;
}

void addItems(levelChoice l)
{
	float positions[] = {	50.0f, -50.0f,
							50.0f, -500.0f,
							50.0f, -950.0f,
							500.0f, -50.0f,
							//550.0f, -400.0f,
							500.0f, -900.0f,
							950.0f, -50.0f,
							950.0f, -500.0f,
							950.0f, -950.0f };


	int numItems = rand()%3 + 5;
	
	vector<int> positionsTaken;

	for(int x=0; x<numItems; x++)
	{
		int type = rand()%6;
		CollidableType t;
		if(type == 0)
			t = ITEM_MINIGUN;
		else if(type == 1)
			t = ITEM_MINE;
		else if(type == 2)
			t = ITEM_MISSILE;
		else if(type == 3)
			t = ITEM_BOMB;
		else if(type == 4)
			t = ITEM_SPEED_MISSILE;
		else if(type == 5)
			t = ITEM_POWER_MISSILE;

		int position = rand()%7 + 1; //1 - 7 inclusive

		bool allowedPosition = true;
		Item* i;
		if( positionsTaken.size() == 0 )
		{
			itemContainer->add(t);
			i = itemContainer->getLast();
			allowedPosition = true;

			i->setPosition(positions[2*position], 0.5f, positions[2*position+1]);
			positionsTaken.push_back(position);
		}
		else
		{
			for(int y=0; y<(int)positionsTaken.size(); y++)
			{
				if(positionsTaken.at(y) == position)
				{
					allowedPosition = false;
					x--;
					break;
				}
			}

			if(allowedPosition)
			{
				itemContainer->add(t);
				i = itemContainer->getLast();

				i->setPosition(positions[2*position], 0.5f, positions[2*position+1]);
				positionsTaken.push_back(position);
			}
		}
	}
	CollidableType t = ITEM_HEALTH;
	itemContainer->add(t);

	Item* i = itemContainer->getLast();
	i->setPosition(200.0f, 0.5f, -50.0f);
}

void cleanVehicles(void)
{
	unsigned int numVehicles = vehicleContainer->numberOfVehicles();
	for(unsigned int i=0; i<numVehicles; i++)
	{
		if(vehicleContainer->clean(i))
		{
			Vehicle* v = vehicleContainer->get(i);
			float p[3];
			v->getPosition(p);
			v->setSpeed(0.0f);
			effectContainer->add(FIREBALL,p);
			effectContainer->add(EXPLOSION, p);
			effectContainer->add(CRACK,p);
			effectContainer->add(SMOKE,p);
			p[0] += rand()%10 - 5;
			p[2] += rand()%10 - 5;
			effectContainer->add(CRACK,p);
			effectContainer->add(SMOKE,p);
			p[0] += rand()%10 - 5;
			p[2] += rand()%10 - 5;
			effectContainer->add(CRACK,p);
			effectContainer->add(SMOKE,p);
			p[0] += rand()%10 - 5;
			p[2] += rand()%10 - 5;
			effectContainer->add(CRACK,p);
			effectContainer->add(SMOKE,p);
			p[0] += rand()%10 - 5;
			p[2] += rand()%10 - 5;
			effectContainer->add(CRACK,p);
			effectContainer->add(SMOKE,p);
			p[0] += rand()%10 - 5;
			p[2] += rand()%10 - 5;
			effectContainer->add(CRACK,p);
			effectContainer->add(SMOKE,p);

			//vehicle explosion

			if(endGame == false)
			{
				if(i==0)
				{
					endGame = true;
					endGameTimer->startTimer();
				}
				else
				{
					aiContainer->clean(i-1);

					p[0] = 5000.0f;
					p[1] = 5000.0f;
					p[2] = 5000.0f;
					v->setPosition(p);
					int numAI = aiContainer->numberofAI();
					if(numAI == 0)
					{
						//you win!!
						winGame = true;
						endGame = true;
						endGameTimer->startTimer();
					}
				}
				v->inPlay();
		
			}
		}
	}
}


void initializeMenu(MenuContainer* mC)
{
	//////////////////////////////

	menuGroup loadingGroup;
	loadingGroup.myType = MENU_LOADING;

	menuItem* loadingTitle = new menuItem("Resources/Menus/loading.tga");
	//loadingTitle->destinationGroup = MENU_NONE;//MENU_MAIN;

	loadingGroup.add(loadingTitle);

	mC->add(loadingGroup);

	//////////////////////////////

	menuGroup winGroup;
	winGroup.myType = MENU_WIN;

	menuItem* winTitle = new menuItem("Resources/Menus/win.tga");
	
	winTitle->setAttributes(0.0f, 0.0f, 1.0f, 0.5f);
	winGroup.add(winTitle);

	mC->add(winGroup);

	//////////////////////////////

	menuGroup loseGroup;
	loseGroup.myType = MENU_LOSE;

	menuItem* loseTitle = new menuItem("Resources/Menus/lose.tga");
	//loadingTitle->destinationGroup = MENU_NONE;//MENU_MAIN;
	
	loseTitle->setAttributes(0.0f, 0.0f, 1.0f, 0.5f);
	loseGroup.add(loseTitle);

	mC->add(loseGroup);
	

	//////////////////////////////

	menuGroup mainMenuGroup;
	mainMenuGroup.myType = MENU_MAIN;

	menuItem* logoTitle = new menuItem("Resources/Menus/logo.tga");
	//logoTitle->destinationGroup = MENU_NONE;
	logoTitle->setAttributes(0.0f, 0.75f, 1.0f, 0.5f);

	menuItem* newGameButton = new menuItem("Resources/Menus/newgame.tga");
	newGameButton->destinationGroup = MENU_NEW_GAME;
	newGameButton->setAttributes(0.0f, 0.4f, 1.0f, 0.25f);

	menuItem* settingsButton = new menuItem("Resources/Menus/settings.tga");
	settingsButton->destinationGroup = MENU_SETTINGS;
	settingsButton->setAttributes(0.0f, 0.0f, 1.0f, 0.25f);

	menuItem* creditsButton = new menuItem("Resources/Menus/credits.tga");
	creditsButton->destinationGroup = MENU_CREDITS;
	creditsButton->setAttributes(0.0f, -0.2f, 1.0f, 0.25f);

	menuItem* quitButton = new menuItem("Resources/Menus/quit.tga");
	//quitButton->destinationGroup = MENU_NONE;
	quitButton->destinationAction = MENU_RETURN_QUIT_GAME;
	quitButton->setAttributes(0.0f, -0.6f, 1.0f, 0.5f);

	mainMenuGroup.add(logoTitle);
	mainMenuGroup.add(newGameButton);
	mainMenuGroup.add(settingsButton);
	mainMenuGroup.add(creditsButton);
	mainMenuGroup.add(quitButton);

	mC->add(mainMenuGroup);

	
	//////////////////////////////
	
	menuGroup settingsMenuGroup;
	settingsMenuGroup.myType = MENU_SETTINGS;

	menuItem* settingsTitle = new menuItem("Resources/Menus/settings.tga");
	settingsTitle->setAttributes(0.0f, 0.75f, 1.0f, 0.25f);

	menuItem* musicTitle = new menuItem("Resources/Menus/music.tga");
	musicTitle->setAttributes(0.0f, 0.3f, 1.0f, 0.5f);

	menuItem* yesButton = new menuItem("Resources/Menus/yes.tga");
	yesButton->destinationAction = MENU_RETURN_MUSIC_ON;
	yesButton->setAttributes(-0.3f, 0.0f, 0.5f, 0.25f);

	menuItem* noButton = new menuItem("Resources/Menus/no.tga");
	noButton->destinationAction = MENU_RETURN_MUSIC_OFF;
	noButton->setAttributes(0.3f, 0.0f, 0.5f, 0.25f);

	menuItem* settingsBackButton = new menuItem("Resources/Menus/mainmenu.tga");
	settingsBackButton->destinationGroup = MENU_MAIN;
	settingsBackButton->setAttributes(0.0f, -0.5f, 1.0f, 0.5f);

	settingsMenuGroup.add(settingsTitle);
	settingsMenuGroup.add(musicTitle);
	settingsMenuGroup.add(yesButton);
	settingsMenuGroup.add(noButton);
	settingsMenuGroup.add(settingsBackButton);

	mC->add(settingsMenuGroup);

	
	//////////////////////////////

	menuGroup newGameMenuGroup;
	newGameMenuGroup.myType = MENU_NEW_GAME;

	menuItem* newgameTitle = new menuItem("Resources/Menus/newgame.tga");
	newgameTitle->setAttributes(0.0f, 0.9f, 2.0f, 0.5f);
	
	menuItem* level1 = new menuItem("Resources/Menus/level1.tga");
	level1->setAttributes(0.0f, 0.3f, 0.5f, 0.5f);
	level1->destinationAction = MENU_RETURN_LEVEL1;

	menuItem* level2 = new menuItem("Resources/Menus/level2.tga");
	level2->setAttributes(-0.6f, -0.3f, 0.5f, 0.5f);
	level2->destinationAction = MENU_RETURN_LEVEL2;

	menuItem* level3 = new menuItem("Resources/Menus/level3.tga");
	level3->setAttributes(0.6f, -0.3f, 0.5f, 0.5f);
	level3->destinationAction = MENU_RETURN_LEVEL3;

	menuItem* newGameBackButton = new menuItem("Resources/Menus/mainmenu.tga");
	newGameBackButton->destinationGroup = MENU_MAIN;
	newGameBackButton->setAttributes(0.0f, -0.8f, 1.0f, 0.5f);

	newGameMenuGroup.add(newgameTitle);
	newGameMenuGroup.add(level1);
	newGameMenuGroup.add(level2);
	newGameMenuGroup.add(level3);
	newGameMenuGroup.add(newGameBackButton);

	mC->add(newGameMenuGroup);

		
	//////////////////////////////

	menuGroup creditsMenuGroup;
	creditsMenuGroup.myType = MENU_CREDITS;

	menuItem* creditsTitle = new menuItem("Resources/Menus/credits.tga");
	creditsTitle->setAttributes(0.0f, 0.9f, 1.0f, 0.25f);
	
	menuItem* developers = new menuItem("Resources/Menus/developers.tga");
	developers->setAttributes(0.0f, 0.4f, 1.0f, 0.25f);
	
	menuItem* creditsContent = new menuItem("Resources/Menus/creditsus.tga");
	creditsContent->setAttributes(0.0f, 0.1f, 2.0f, 0.5f);

	menuItem* musicByContent = new menuItem("Resources/Menus/musicby.tga");
	musicByContent->setAttributes(0.0f, -0.3f, 0.75f, 0.18f);

	menuItem* evanCredit = new menuItem("Resources/Menus/evanperry.tga");
	evanCredit->setAttributes(0.0f, -0.5f, 1.0f, 0.25f);

	menuItem* creditsBackButton = new menuItem("Resources/Menus/mainmenu.tga");
	creditsBackButton->destinationGroup = MENU_MAIN;
	creditsBackButton->setAttributes(0.0f, -0.8f, 1.0f, 0.5f);

	creditsMenuGroup.add(creditsTitle);
	creditsMenuGroup.add(developers);
	creditsMenuGroup.add(creditsContent);
	creditsMenuGroup.add(musicByContent);
	creditsMenuGroup.add(evanCredit);
	creditsMenuGroup.add(creditsBackButton);

	mC->add(creditsMenuGroup);

	
	//////////////////////////////
	menuGroup pauseMenuGroup;
	pauseMenuGroup.myType = MENU_PAUSE;

	menuItem* pauseTitle = new menuItem("Resources/Menus/paused.tga");
	pauseTitle->setAttributes(0.0f, 0.75f, 1.0f, 0.5f);
	
	menuItem* pauseReturn = new menuItem("Resources/Menus/return.tga");
	pauseReturn->destinationAction = MENU_RETURN_UNPAUSE;
	pauseReturn->setAttributes(0.0f, 0.3f, 1.0f, 0.5f);

	menuItem* pauseQuitRound = new menuItem("Resources/Menus/quitround.tga");\
	pauseQuitRound->destinationAction = MENU_RETURN_QUIT_ROUND;
	pauseQuitRound->setAttributes(0.0f, -0.1f, 1.0f, 0.5f);

	menuItem* pauseQuitGame = new menuItem("Resources/Menus/quitgame.tga");
	pauseQuitGame->destinationAction = MENU_RETURN_QUIT_GAME;
	pauseQuitGame->setAttributes(0.0f, -0.5f, 1.0f, 0.5f);



	pauseMenuGroup.add(pauseTitle);
	pauseMenuGroup.add(pauseReturn);
	pauseMenuGroup.add(pauseQuitRound);
	pauseMenuGroup.add(pauseQuitGame);

	mC->add(pauseMenuGroup);

	
	//////////////////////////////

	//Tell MenuContainer to render the main menu
	mC->setActive(MENU_MAIN);
	

}
