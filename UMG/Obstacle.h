#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "Settings.h"
#include "Render.h"
#include "Physics.h"

class Obstacle: public Collidable, public Renderable
{
	//Obstacles are assumed to be rectangular solids that are axis-aligned
	//Feel free to put member variables in here like position, length, width, height, color, etc
	//Also feel free to add public methods like setPosition/Width/Height/Length/Color
	float length;
	float width;
	float height;
	float position[3]; //x,y,z
	float diameterXYZ[3];
	float color[4]; //rgb
	bool physicsEnabled;

	LTGA* myTexture;
	GLuint myTextureNum;

	#ifdef ENABLE_SHADERS
		Shader vertShader;
		Shader fragShader;
		GLhandleARB shaderSPO;
	#endif
	
public:
	Obstacle ( void )
	{
		length = width = height = 0;
		position[0] = position[1] = position[2] = 0;
		color[0] = color[1] = color[2] = color[3] = 0.0f;
		physicsEnabled = true;

		
#ifdef ENABLE_SHADERS
		//Create, link, ground shaders!
		vertShader.initialize("Resources/wallvs.shdr", GL_VERTEX_SHADER);
		fragShader.initialize("Resources/wallfs.shdr", GL_FRAGMENT_SHADER);

		//link the two shaders
		shaderSPO = glCreateProgram();
		glAttachShader(shaderSPO, vertShader.object());
		glAttachShader(shaderSPO, fragShader.object());
		glLinkProgram(shaderSPO);
#else

		glEnable(GL_TEXTURE_2D);
		//Setup texture!
		myTexture = new LTGA("Resources/UMG_grid.tga");
		byte* myPixels = myTexture->GetPixels();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &myTextureNum);
		glBindTexture(GL_TEXTURE_2D, myTextureNum);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		int imageWidth = myTexture->GetImageWidth();
		int imageHeight = myTexture->GetImageHeight();

		int pixelDepth = myTexture->GetPixelDepth();
		int alphaDepth = myTexture->GetAlphaDepth();
		int imageType = myTexture->GetImageType();

		//copy to the graphics card
		if(alphaDepth == 8)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, myPixels); 
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, myPixels); 
		}

		delete myTexture;
		myTexture = NULL;
		glDisable(GL_TEXTURE_2D);
#endif



	}




	//at the moment, we can just flat-shade them with a solid color
	void render( void )
	{
		glPushMatrix();
		


#ifdef ENABLE_SHADERS
			glUseProgramObjectARB(shaderSPO); //enable shaders if appropriate
#else 
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, myTextureNum);
#endif
			glColor4f(color[0], color[1], color[2], color[3]);
			//glTranslatef( position[0], position[1], position[2]);
			//glScalef( length, height, width);
			PrimitiveCube newObst( 1.0f, true );
			newObst.setCenter(position);
			newObst.setDiameterXYZ(length, height, width);
			newObst.render();

#ifdef ENABLE_SHADERS
			glUseProgramObjectARB(0); //disable shaders for the rest of the rendering
#else
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
#endif
		glPopMatrix();

	}

	int getNumElements(void)
	{
		return 1;
	}

	//write the 8 verteces into the passed-in float array (
	void getBox(int n, float* box)
	{
		float maxXArg = abs( length / 2.0f);
		float maxZArg = abs( width  / 2.0f);

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

	void getPoint(int n, float* f)
	{
	}

	//self-explanitory
	void getCenterOfCollidable(float* box)
	{
		box[0] = position[0];
		box[1] = position[1];
		box[2] = position[2];
	}

	//obstacles probably won't react to collisions, so you don't have to do much with this
	void registerCollision(CollidableType cT, int collidingBoxNum)
	{
		/*color[0] = 0.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;*/
	}

	//obstacles touching obstacles should NOT generate a physics event
	CollidableType getCollidableType(void)
	{
		return OBSTACLE;
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
	
	void processCollisions()
	{
		clearCurrentCollisions();
	}
};

//Also add an ObstacleContainer that allows us to add Obstacles into the Physics manager
// as STATIC collidable objects ---  physics->addAsStatic()
// Copy the Vehicle or Projectile Classes' Containers; they are the most thorough
class ObstacleContainer{
	vector<Obstacle*> list;
	Render* renderer;
	Physics* physics;
public:
	ObstacleContainer(Render* r, Physics* p){
		list = vector<Obstacle*>();
		renderer = r;
		physics = p;
	}
	~ObstacleContainer()
	{
		while(list.size()>0)
		{
			Obstacle* o = list.at(0);
			delete o;
			list.erase(list.begin());
		}
	}

	//Creates and allocates new obstacle
	void add( void )
	{
		Obstacle* o = new Obstacle();
		list.push_back(o);
		renderer->add(o);
		physics->add(o);
		physics->addAsObstacle(o); //for physics LOS check
		o->setCollidableShape(COLLIDABLE_BOX);
	}

	void remove( int i )
	{
		Obstacle* o = list.at(i);
		delete o;
		list.erase(list.begin()+i);
	}

	void clean( void )
	{
		//unsigned int n = list.size();

		for(unsigned int i = 0; i < (int) list.size(); i++)
		{
			Obstacle* o = list.at(i);
			renderer->remove(o);
			physics->removeAsObstacle(o);
			remove(i);
			i--; 
		}
	}

	Obstacle* get( int i )
	{
		if(list.size() < (unsigned int)i) return 0;
		return list.at(i);
	}

	Obstacle* getLast( void )
	{
		if(list.size() > 0)
			return list.at(list.size()-1);
		else return NULL;
	}

};

#endif