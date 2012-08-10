#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "Vehicle.h"
#include "Item.h"
#include "Obstacle.h"
#include "Scenery.h"
#include "Target.h"

using namespace std;

enum levelChoice{
	LEVEL_CHOICE_NONE,
	LEVEL_CHOICE_MAZE,
	LEVEL_CHOICE_DEMO_2,
	LEVEL_CHOICE_BLOCK_FORT,
	LEVEL_CHOICE_COLUMNS
};

class Light : public Renderable
{
	float position[3];
	float colorAmbient[4];
	float colorDiffuse[4];
	float colorSpecular[4];
public:
	Light(){}
	~Light(){}
	void setPosition(float* p)
	{
		position[0] = p[0];
		position[1] = p[1];
		position[2] = p[2];
	}
	void setColorAmbient(float* c)
	{
		colorAmbient[0] = c[0];
		colorAmbient[1] = c[1];
		colorAmbient[2] = c[2];
		colorAmbient[3] = c[3];
	}
	void setColorDiffuse(float* c)
	{
		colorDiffuse[0] = c[0];
		colorDiffuse[1] = c[1];
		colorDiffuse[2] = c[2];
		colorDiffuse[3] = c[3];
	}
	void setColorSpecular(float* c)
	{
		colorSpecular[0] = c[0];
		colorSpecular[1] = c[1];
		colorSpecular[2] = c[2];
		colorSpecular[3] = c[3];
	}
	void render( void )
	{
		glEnable(GL_LIGHT0);

		// Assign created components to GL_LIGHT0
		glLightfv(GL_LIGHT0, GL_AMBIENT, colorAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, colorSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}
};


class World : public Renderable
{
	int numVehicles;
	int numItems;
	unsigned int timeSinceDrop, change;
	levelChoice level;

#ifdef ENABLE_SHADERS
	Shader groundVertShader;
	Shader groundFragShader;
	GLhandleARB groundShaderSPO;
#endif
	
public:
	World(ObstacleContainer *, TargetContainer *, levelChoice);
	~World(void);

	void render();	

	void createMap(ObstacleContainer*, TargetContainer*);
};


#endif