#include "World.h"
#include "Settings.h"
#include <stdio.h>
#include <stdlib.h>


World::World(	ObstacleContainer* obstacleContainer, 
				TargetContainer* targetContainer,
				levelChoice l) 
{
	
#ifdef ENABLE_SHADERS
	//Create, link, ground shaders!
	groundVertShader.initialize("Resources/groundvs.shdr", GL_VERTEX_SHADER);
	groundFragShader.initialize("Resources/groundfs.shdr", GL_FRAGMENT_SHADER);

	//link the two shaders
	groundShaderSPO = glCreateProgram();
    glAttachShader(groundShaderSPO, groundVertShader.object());
    glAttachShader(groundShaderSPO, groundFragShader.object());
    glLinkProgram(groundShaderSPO);
#endif

	//Add scenery
	
	level = l;

	createMap(obstacleContainer, targetContainer);

	
}


World::~World(void)
{
}

//Responsible for rendering the whole world
void World::render(void)
{

#ifdef ENABLE_SHADERS
	glUseProgramObjectARB(groundShaderSPO);
#endif

	//Draw the ground plane
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3f(-1000.0, 0.0, 1000.0);
			glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-1000.0, 0.0, -2000.0);
			glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(2000.0, 0.0, -2000.0);
			glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(2000.0, 0.0, 1000.0);
			glNormal3f(0.0, 1.0, 0.0);
	glEnd();

#ifdef ENABLE_SHADERS
	glUseProgramObjectARB(0);
#endif

}
	


void World::createMap(ObstacleContainer* obstacleContainer, TargetContainer* targetContainer)
{




	obstacleContainer->add();

	Obstacle* wall1 = obstacleContainer->getLast();
	wall1->setLHW(1060.0f, 11.0f, 30.0f);
	wall1->setPosition(500.0f, 5.0f, -1015.0f);
	wall1->setColor(1.0f, 1.0f, 1.0f);
	
	obstacleContainer->add();

	Obstacle* wall2 = obstacleContainer->getLast();
	wall2->setLHW(1060.0f, 11.0f, 30.0f);
	wall2->setPosition(500.0f, 5.0f, 15.0f);
	wall2->setColor(1.0f, 1.0f, 1.0f);

	obstacleContainer->add();

	Obstacle* wall3 = obstacleContainer->getLast();
	wall3->setLHW(30.0f, 11.0f, 1000.0f);
	wall3->setPosition(-15.0f, 5.0f, -500.0f);
	wall3->setColor(1.0f, 1.0f, 1.0f);

	obstacleContainer->add();

	Obstacle* wall4 = obstacleContainer->getLast();
	wall4->setLHW(30.0f, 11.0f, 1000.0f);
	wall4->setPosition(1015.0f, 5.0f, -500.0f);
	wall4->setColor(1.0f, 1.0f, 1.0f);
	
	
	if(level == LEVEL_CHOICE_MAZE)
	{
		obstacleContainer->add();

		Obstacle* o = obstacleContainer->getLast();
		o->setLHW(203.0f, 101.0f, 304.0f);
		o->setPosition(200.0f, 50.0f, -750.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();

		o = obstacleContainer->getLast();
		o->setLHW(350.0f, 101.0f, 150.0f);
		o->setPosition(700.0f, 50.0f, -750.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();

		o = obstacleContainer->getLast();
		o->setLHW(425.0f, 101.0f, 200.0f);
		o->setPosition(302.0f, 50.0f, -401.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();

		o = obstacleContainer->getLast();
		o->setLHW(310.0f, 101.0f, 250.0f);
		o->setPosition(774.0f, 50.0f, -450.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();

		o = obstacleContainer->getLast();
		o->setLHW(400.0f, 101.0f, 100.0f);
		o->setPosition(501.0f, 50.0f, -174.0f);
		o->setColor(1.0f, 0.0f, 0.0f);
		
		
	}


	else if(level == LEVEL_CHOICE_BLOCK_FORT)
	{
	
		float diameter = 200.0f;

		obstacleContainer->add();
		Obstacle* o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter-1.0f, diameter);
		o->setPosition(250.0f, 95.0f, -250.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter-1.0f, diameter);
		o->setPosition(751.0f, 95.0f, -250.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter-1.0f, diameter);
		o->setPosition(250.0f, 95.0f, -750.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter-1.0f, diameter);
		o->setPosition(751.0f, 95.0f, -750.0f);
		o->setColor(1.0f, 0.0f, 0.0f);
	
		//box in the middle
		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter/2.0f, 25.0f-1.0f, diameter/2.0f);
		o->setPosition(500.0f, 12.0f, -500.0f);
		o->setColor(1.0f, 0.0f, 0.0f);

	}

	else if(level == LEVEL_CHOICE_DEMO_2)
	{
	
		float diameter = 150.0f;

		obstacleContainer->add();
		Obstacle* o = obstacleContainer->getLast();
		o->setLHW(900.0f, 10.0f, 25.0f);
		o->setPosition(450.0f, 5.0f, -100.0f);
		o->setColor(1.0f, 1.0f, 1.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter, diameter);
		o->setPosition(250.0f, 70.0f, -300.0f);
		o->setColor(1.0f, 1.0f, 1.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter, diameter);
		o->setPosition(750.0f, 70.0f, -300.0f);
		o->setColor(1.0f, 1.0f, 1.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter, diameter);
		o->setPosition(250.0f, 70.0f, -700.0f);
		o->setColor(1.0f, 1.0f, 1.0f);

		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter, diameter, diameter);
		o->setPosition(750.0f, 70.0f, -700.0f);
		o->setColor(1.0f, 1.0f, 1.0f);
	
		//box in the middle
		obstacleContainer->add();
		o = obstacleContainer->getLast();
		o->setLHW(diameter/2.0f, 25.0f, diameter/2.0f);
		o->setPosition(500.0f, 12.0f, -500.0f);
		o->setColor(1.0f, 1.0f, 1.0f);
	
	}

	else if(level == LEVEL_CHOICE_COLUMNS)
	{
		Obstacle* o;
		for(int x=205; x<1000; x=x+280)
		{
			for(int y=205; y<1000; y=y+280)
			{
				obstacleContainer->add();
				o = obstacleContainer->getLast();
				o->setLHW(118.0f, 101.0f, 120.0f);
				o->setPosition((float)x, 50.0f, -(float)y);
				o->setColor(1.0f, 1.0f, 1.0f);
			}
		}

	}

}