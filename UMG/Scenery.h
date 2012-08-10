#ifndef _SCENERY_H_
#define _SCENERY_H_

#include "Settings.h"
#include "Render.h"
#include "Texture.h"
#include <string>

class Scenery: public Renderable
{
	TextureContainer* tC;

	bool isTextured;
	bool isShaded;
	bool textureCoordinatesAreLocal;
	float length;
	float width;
	float height;
	float position[3]; //x,y,z
	float color[4]; //rgba

	GLuint myTextureNum;

	#ifdef ENABLE_SHADERS
		Shader vertShader;
		Shader fragShader;
		GLhandleARB shaderSPO;
	#endif
	
public:
	Scenery (	TextureContainer* TC,
				bool textured, 
				bool shaded,
				bool textureCoordinateFrameIsLocal, 
				string filename,
				string filenameSecond )
	{
		tC = TC;
		isTextured = textured;
		isShaded = shaded;
		textureCoordinatesAreLocal = textureCoordinateFrameIsLocal;

		length = width = height = 0;
		position[0] = position[1] = position[2] = 0;
		color[0] = color[1] = color[2] = color[3] = 1.0f;

		
#ifdef ENABLE_SHADERS
		if(isShaded)
		{
			//Create, link, ground shaders!
			vertShader.initialize(filename, GL_VERTEX_SHADER);
			fragShader.initialize(filenameSecond, GL_FRAGMENT_SHADER);

			//link the two shaders
			shaderSPO = glCreateProgram();
			glAttachShader(shaderSPO, vertShader.object());
			glAttachShader(shaderSPO, fragShader.object());
			glLinkProgram(shaderSPO);
		}
#endif

		if(isTextured){
			glEnable(GL_TEXTURE_2D);
			myTextureNum = tC->add(filename);
			glDisable(GL_TEXTURE_2D);
		}




	}




	//at the moment, we can just flat-shade them with a solid color
	void render( void )
	{
		glPushMatrix();
		


#ifdef ENABLE_SHADERS
		if(isShaded)
		{
			glUseProgramObjectARB(shaderSPO); //enable shaders if appropriate
		}
#endif

		if(isTextured){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, myTextureNum);
		}

		glColor4f(color[0], color[1], color[2], color[3]);
		//glTranslatef( position[0], position[1], position[2]);
		//glScalef( length, height, width);
			
		PrimitiveCube newObst(1.0, !textureCoordinatesAreLocal);
		if(!textureCoordinatesAreLocal){
			newObst.setCenter(position);
			newObst.setDiameterXYZ(length, height, width);
		}
		else
		{
			glTranslatef(position[0], position[1], position[2]);
			glScalef(length, height, width);
		}
		newObst.render();

#ifdef ENABLE_SHADERS
		if(isShaded)
		{
			glUseProgramObjectARB(0); //disable shaders for the rest of the rendering
		}
#endif
		if(isTextured)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

		glPopMatrix();

	}

	void setPosition(float x, float y, float z)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	void setColor(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = 1.0f;
	}

	void setLHW (float l, float h, float w)
	{
		length = l;
		height = h;
		width = w;
		//must call setPosition just after initialization
	}
};

class SceneryContainer{
	vector<Scenery*> list;
	Render* renderer;
	TextureContainer* tC;
public:
	SceneryContainer(Render* r, TextureContainer* TC){
		list = vector<Scenery*>();
		renderer = r;
		tC = TC;
	}

	~SceneryContainer()
	{
		while(list.size()>0)
		{
			Scenery* s = list[0];
			delete s;
			list.erase(list.begin());
		}
	}

	//Creates and allocates new obstacle
	void add(	bool textured,
				bool shaded,
				bool local,
				string fileName,
				string fileNameSecond)
	{
		Scenery *s = new Scenery(	tC,
									textured,
									shaded,
									local,
									fileName,
									fileNameSecond );
		list.push_back(s);
		renderer->add(s);
	}

	void remove( int i )
	{
		Scenery* s = list[i];
		delete s;
		list.erase(list.begin()+i);
	}

	void clean( void )
	{
		//unsigned int n = list.size();
		//int i = 0;
		while(list.size() > 0)
		{
			Scenery* s = list.at(0);
			renderer->remove(s);
			remove(0); 
		}
	}

	Scenery* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list[i];
	}

	Scenery* getLast( void )
	{
		if(list.size() > 0)
			return list.at(list.size()-1);
		else return NULL;
	}

	void createStandardScenery ( void )
	{
		
		Scenery* s;


		add(true, false, true, "Resources/Stars.tga", "");
		s = getLast();
		s->setPosition(500.0f, 750.0f, -500.0f);
		s->setLHW(3000.0f, 3000.0f, 3000.0f);
		s->setColor(0.3f, 0.3f, 0.3f);
			
		add(false, true, false, "Resources/groundundervs.shdr", "Resources/groundunderfs.shdr");
		s = getLast();
		s->setPosition(500.0f, -3.0f, -500.0f);
		s->setLHW(3000.0f, 1.0f, 3000.0f);
		s->setColor(0.0f, 0.302f, 0.263f);
	
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1550.0f, 250.0f, 650.0f);
		s->setLHW(200.0f, 500.0f, 200.0f);
		s->setColor(1.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1600.0f, 150.0f, 500.0f);
		s->setLHW(100.0f, 300.0f, 100.0f);
		s->setColor(1.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1400.0f, 100.0f, 500.0f);
		s->setLHW(300.0f, 200.0f, 300.0f);
		s->setColor(1.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1400.0f, 200.0f, 700.0f);
		s->setLHW(100.0f, 400.0f, 100.0f);
		s->setColor(1.0f, 1.0f, 0.0f);
		
		//other buildings
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1200.0f, 50.0f, 100.0f);
		s->setLHW(150.0f, 100.0f, 150.0f);
		s->setColor(0.0f, 0.0f, 1.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1650.0f, 150.0f, -300.0f);
		s->setLHW(150.0f, 300.0f, 500.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1300.0f, 100.0f, -600.0f);
		s->setLHW(250.0f, 200.0f, 150.0f);
		s->setColor(0.0f, 1.0f, 1.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1600.0f, 200.0f, -1000.0f);
		s->setLHW(300.0f, 400.0f, 300.0f);
		s->setColor(1.0f, 0.0f, 1.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1100.0f, 50.0f, -1100.0f);
		s->setLHW(75.0f, 100.0f, 75.0f);
		s->setColor(1.0f, 0.5f, 0.5f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(1300.0f, 150.0f, -1600.0f);
		s->setLHW(250.0f, 300.0f, 200.0f);
		s->setColor(0.5f, 1.0f, 0.0f);

		//left

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-350.0f, 250.0f, -1650.0f);
		s->setLHW(200.0f, 500.0f, 200.0f);
		s->setColor(1.0f, 0.0f, 1.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-200.0f, 150.0f, -1700.0f);
		s->setLHW(100.0f, 300.0f, 100.0f);
		s->setColor(1.0f, 0.0f, 1.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-200.0f, 100.0f, -1575.0f);
		s->setLHW(300.0f, 200.0f, 150.0f);
		s->setColor(1.0f, 0.0f, 1.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-400.0f, 200.0f, -1500.0f);
		s->setLHW(100.0f, 400.0f, 100.0f);
		s->setColor(1.0f, 0.0f, 1.0f);

		//other buildings
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(0.0f, 100.0f, -1300.0f);
		s->setLHW(100.0f, 200.0f, 100.0f);
		s->setColor(0.0f, 1.0f, 1.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(250.0f, 50.0f, -1200.0f);
		s->setLHW(75.0f, 100.0f, 100.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(600.0f, 40.0f, -1100.0f);
		s->setLHW(250.0f, 80.0f, 100.0f);
		s->setColor(0.0f, 0.5f, 1.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(800.0f, 150.0f, -1150.0f);
		s->setLHW(50.0f, 300.0f, 50.0f);
		s->setColor(1.0f, 0.0f, 0.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(650.0f, 200.0f, -1600.0f);
		s->setLHW(500.0f, 400.0f, 200.0f);
		s->setColor(0.5f, 0.0f, 0.5f);

		//right

		//other buildings
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(0.0f, 100.0f, 300.0f);
		s->setLHW(100.0f, 200.0f, 100.0f);
		s->setColor(0.0f, 1.0f, 1.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(250.0f, 50.0f, 200.0f);
		s->setLHW(75.0f, 100.0f, 100.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(600.0f, 40.0f, 100.0f);
		s->setLHW(250.0f, 80.0f, 100.0f);
		s->setColor(0.0f, 0.5f, 1.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(800.0f, 150.0f, 150.0f);
		s->setLHW(50.0f, 300.0f, 50.0f);
		s->setColor(1.0f, 0.0f, 0.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(650.0f, 200.0f, 600.0f);
		s->setLHW(500.0f, 400.0f, 200.0f);
		s->setColor(0.5f, 0.0f, 0.5f);

		//behind

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-600.0f, 250.0f, 350.0f);
		s->setLHW(200.0f, 500.0f, 200.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-650.0f, 150.0f, 150.0f);
		s->setLHW(100.0f, 300.0f, 200.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-500.0f, 100.0f, 250.0f);
		s->setLHW(200.0f, 200.0f, 200.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-425.0f, 200.0f, 400.0f);
		s->setLHW(150.0f, 400.0f, 100.0f);
		s->setColor(0.0f, 1.0f, 0.0f);

		//other buildings
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-700.0f, 200.0f, -1150.0f);
		s->setLHW(200.0f, 400.0f, 400.0f);
		s->setColor(1.0f, 0.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-100.0f, 50.0f, -900.0f);
		s->setLHW(75.0f, 100.0f, 100.0f);
		s->setColor(0.0f, 1.0f, 0.0f);
		
		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-500.0f, 125.0f, -750.0f);
		s->setLHW(250.0f, 250.0f, 100.0f);
		s->setColor(0.0f, 0.5f, 1.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-300.0f, 75.0f, -300.0f);
		s->setLHW(200.0f, 150.0f, 150.0f);
		s->setColor(1.0f, 0.0f, 0.0f);

		add(false, true, false, "Resources/sceneryvs.shdr", "Resources/sceneryfs.shdr");
		s = getLast();
		s->setPosition(-650.0f, 200.0f, -350.0f);
		s->setLHW(150.0f, 400.0f, 500.0f);
		s->setColor(0.5f, 0.0f, 0.5f);
	}

};

#endif