#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <vector>
#include "Render.h"
#include "Texture.h"
//#include "Graphics.h"
#include "SystemTimer.h"
#include "Camera.h"
#include "Model.h"

enum EffectType{
	EXPLOSION,
	EXPLOSION_MINE,
	SPARK,
	SMOKE,
	SMOKE_SMALL,
	CRACK,
	FIREBALL,
	FIREBALL_SMALL
};

class Effect: public Renderable, public Movable
{
	EffectType type;
	float basePosition[3];
	float position[50][3];
	float velocity[50][3];
	float orientation[3];

	SystemTimer time;

	float Lifetime;
	bool shouldBeDeleted;
	Camera* camera;
	TextureContainer* tC;
	ModelContainer* mC;
	Model* model;
	float rotateCW;
	float startAngle;
	GLuint textureNumber;


public:
	Effect( Camera* cam, 
			TextureContainer* TC, 
			ModelContainer* MC,
			EffectType t,
			float base[3] )
	{
		type = t;
		camera = cam;
		tC = TC;
		mC = MC;

		if(type == SPARK){
			Lifetime = 0.2f;
			for(int i = 0; i < 5; i++)
			{
				position[i][0] = base[0];
				position[i][1] = base[1];
				position[i][2] = base[2];
				velocity[i][0] = (((float)(rand()%70))/100.0f)-0.35f;
				velocity[i][1] = (((float)(rand()%90))/100.0f)-0.35f;
				velocity[i][2] = (((float)(rand()%70))/100.0f)-0.35f;
			}
		}
		else if(type == EXPLOSION)
		{
			Lifetime = 1.5f;
			for(int i = 0; i < 50; i++)
			{
				position[i][0] = base[0];
				position[i][1] = base[1];
				position[i][2] = base[2];
				velocity[i][0] = (((float)(rand()%60))/100.0f)-0.30f;
				velocity[i][1] = (((float)(rand()%90))/100.0f)-0.05f;
				velocity[i][2] = (((float)(rand()%60))/100.0f)-0.30f;
			}
		}
		else if(type == EXPLOSION_MINE)
		{
			Lifetime = 1.5f;
			for(int i = 0; i < 50; i++)
			{
				position[i][0] = base[0];
				position[i][1] = base[1];
				position[i][2] = base[2];
				velocity[i][0] = (((float)(rand()%50))/100.0f)-0.25f;
				velocity[i][1] = (((float)(rand()%50))/100.0f)-0.05f;
				velocity[i][2] = (((float)(rand()%50))/100.0f)-0.25f;
			}
		}
		else if(type == SMOKE || type == SMOKE_SMALL)
		{
			string filename("Resources/Cloud.tga");
			textureNumber = tC->add(filename);
			Lifetime = (float)(rand()%10) / 10 + 1.0f; //random value, just a placeholder
			basePosition[0] = base[0];
			basePosition[1] = base[1];
			basePosition[2] = base[2];
			
			orientation[0] = 0.0f;
			orientation[1] = 0.0f;
			orientation[2] = 0.0f;
			if(rand()%2 == (int)0)
				rotateCW = 1.0f;
			else
				rotateCW = -1.0f;

			startAngle = (float)(rand()%360);
		}
		else if(type == CRACK)
		{
			string filename("Resources/crack2.tga");
			textureNumber = tC->add(filename);
			Lifetime = 300.0f; //random value, just a placeholder
			basePosition[0] = base[0];
			basePosition[1] = (float)(rand()%11) / 1000.0f + 0.01f;
			basePosition[2] = base[2];

			startAngle = (float)(rand()%360);
		}
		else if(type == FIREBALL)
		{
			string filename("Resources/fireball.tga");
			textureNumber = tC->add(filename);
			string modelname("Resources/fireball.obj");
			model = mC->add(modelname);
			Lifetime = 0.25f; //reaaaaally fast!
			basePosition[0] = base[0];
			basePosition[1] = base[1];
			basePosition[2] = base[2];
			startAngle = (float)(rand()%360);
		}
		else if(type == FIREBALL_SMALL)
		{
			string filename("Resources/fireball.tga");
			textureNumber = tC->add(filename);
			string modelname("Resources/fireball.obj");
			model = mC->add(modelname);
			Lifetime = 0.25f; //reaaaaally fast!
			basePosition[0] = base[0];
			basePosition[1] = base[1];
			basePosition[2] = base[2];
			startAngle = (float)(rand()%360);
		}

		shouldBeDeleted = false;
		time.startTimer();
	}


	void render( void )
	{
		glPushMatrix();
		if( type == SMOKE )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureNumber);
			//glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
			//start by getting the angle from effect to camera
			float p[3];
			camera->getPosition(p);

			float angleToCamera = (float)atan2(p[2] - basePosition[2], p[0] - basePosition[0]);
			angleToCamera *= (float)(-180.0f / M_PI);
			if(angleToCamera < 0.0f) angleToCamera += 360.0f;
			else if(angleToCamera > 360.0f) angleToCamera -= 360.0f;

			glPushMatrix();
				glColor4f(1.0f, 1.0f, 1.0f, Lifetime-time.elapsedTime());
				glTranslatef(basePosition[0], basePosition[1], basePosition[2]);
				glRotatef(angleToCamera + 90.0f, 0.0f, 1.0f, 0.0f);

				glRotatef(startAngle + (rotateCW * time.elapsedTime() * 90.0f), 0.0f, 0.0f, 1.0f);

				float s = time.elapsedTime()*3.0f;
				glScalef(1.0f*s, 1.0f*s, 1.0f*s);
				glBegin(GL_QUADS);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(0.5f, 0.5f, 0.0f); 
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(0.5f, -0.5f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(-0.5f, -0.5f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(-0.5f, 0.5f, 0.0f);
				glEnd();
				
			glPopMatrix();
			
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glDisable(GL_TEXTURE_2D);
			//glEnable(GL_BLEND);
		}
		else if( type == SMOKE_SMALL )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureNumber);
			//glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
			//start by getting the angle from effect to camera
			float p[3];
			camera->getPosition(p);

			float angleToCamera = (float)atan2(p[2] - basePosition[2], p[0] - basePosition[0]);
			angleToCamera *= (float)(-180.0f / M_PI);
			if(angleToCamera < 0.0f) angleToCamera += 360.0f;
			else if(angleToCamera > 360.0f) angleToCamera -= 360.0f;

			glPushMatrix();
				glColor4f(1.0f, 1.0f, 1.0f, Lifetime-time.elapsedTime());
				glTranslatef(basePosition[0], basePosition[1], basePosition[2]);
				glRotatef(angleToCamera + 90.0f, 0.0f, 1.0f, 0.0f);

				glRotatef(startAngle + (rotateCW * time.elapsedTime() * 90.0f), 0.0f, 0.0f, 1.0f);

				float s = time.elapsedTime()*1.5f;
				glScalef(1.0f*s, 1.0f*s, 1.0f*s);
				glBegin(GL_QUADS);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(0.5f, 0.5f, 0.0f); 
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(0.5f, -0.5f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(-0.5f, -0.5f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(-0.5f, 0.5f, 0.0f);
				glEnd();
				
			glPopMatrix();
			
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glDisable(GL_TEXTURE_2D);
			//glEnable(GL_BLEND);
		}
		else if( type == EXPLOSION )
		{
			for(int i = 0; i < 50; i++)
			{
				glPushMatrix();
					glColor4f(0.0f, 0.302f, 0.263f, 1.0f);
					glTranslatef(position[i][0], position[i][1], position[i][2]);
					PrimitiveCube pc(0.5f, false);
					pc.render();
				glPopMatrix();
			}
		}
		else if( type == EXPLOSION_MINE )
		{
			for(int i = 0; i < 50; i++)
			{
				glPushMatrix();
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
					glTranslatef(position[i][0], position[i][1], position[i][2]);
					PrimitiveCube pc(0.2f, false);
					pc.render();
				glPopMatrix();
			}
		}
		else if( type == SPARK)
		{
			for(int i = 0; i < 5; i++)
			{
				glPushMatrix();
					glColor4f(1.0f, 242.0f/255.0f, 0.0f, 1.0f);
					glTranslatef(position[i][0], position[i][1], position[i][2]);
					glScalef(1.0f, 2.0f, 1.0f);
					PrimitiveCube pc(0.3f, false);
					pc.render();
				glPopMatrix();
			}

		}
		else if( type == CRACK )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureNumber);
			glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);

			glPushMatrix();
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glTranslatef(basePosition[0], basePosition[1], basePosition[2]);
				glScalef(10.0f, 1.0f, 10.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef(startAngle, 0.0f, 0.0f, 1.0f);
				

				glBegin(GL_QUADS);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(0.5f, 0.5f, 0.0f); 
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(0.5f, -0.5f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(-0.5f, -0.5f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(-0.5f, 0.5f, 0.0f);
				glEnd();
				
			glPopMatrix();
			
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glDisable(GL_TEXTURE_2D);

		}
		else if( type == FIREBALL)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureNumber);
			glTranslatef(basePosition[0], basePosition[1], basePosition[2]);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f - (time.elapsedTime() / Lifetime));
			int layers = 2;
			float finalSize = 20.0f;
			for(int x=0; x<layers; x++)
			{
				float scaleFactor = finalSize / ((float)(x+1)/(float)layers) * (time.elapsedTime() / Lifetime);
				glPushMatrix();
					glScalef(scaleFactor, scaleFactor/2.0f, scaleFactor);
					if(x == layers-1)
						glRotatef(startAngle, 0.0f, 1.0f, 0.0f);
					else
						glRotatef((float)(rand()%360), 0.0f, 1.0f, 0.0f);
					model->render();
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
		}
		else if( type == FIREBALL_SMALL)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureNumber);
			glTranslatef(basePosition[0], basePosition[1], basePosition[2]);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f - (time.elapsedTime() / Lifetime));
			int layers = 2;
			float finalSize = 5.0f;
			for(int x=0; x<layers; x++)
			{
				float scaleFactor = finalSize / ((float)(x+1)/(float)layers) * (time.elapsedTime() / Lifetime);
				glPushMatrix();
					glScalef(scaleFactor, scaleFactor/2.0f, scaleFactor);
					if(x == layers-1)
						glRotatef(startAngle, 0.0f, 1.0f, 0.0f);
					else
						glRotatef((float)(rand()%360), 0.0f, 1.0f, 0.0f);
					model->render();
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}



	void move( void )
	{
		if(time.elapsedTime() > Lifetime)
		{
			shouldBeDeleted = true;
		}
		if(type == SPARK){
			for(int i = 0; i < 5; i++)
			{
				position[i][0] += velocity[i][0];
				position[i][1] += velocity[i][1];
				position[i][2] += velocity[i][2];
			}
		}
		else if(type == EXPLOSION){
			for(int i = 0; i < 50; i++)
			{
				position[i][0] += velocity[i][0];
				position[i][1] += velocity[i][1];
				position[i][2] += velocity[i][2];
				velocity[i][1] -= 0.02f;
			}
		}
		else if(type == EXPLOSION_MINE){
			for(int i = 0; i < 50; i++)
			{
				position[i][0] += velocity[i][0];
				position[i][1] += velocity[i][1];
				position[i][2] += velocity[i][2];
				velocity[i][1] -= 0.02f;
			}
		}
		else if(type == SMOKE){
			
		}
	}
	bool shouldBeRemoved(void){
		return shouldBeDeleted;
	}

};



class EffectContainer
{
	Camera* cam;
	vector<Effect*> list;
	Render* renderer;
	Move* mover;
	TextureContainer* tC;
	ModelContainer* mC;

public:
	EffectContainer( Camera* c, Render* r, Move* m, TextureContainer* TC, ModelContainer* MC)
	{
		cam = c;
		renderer = r;
		mover = m;
		tC = TC;
		mC = MC;
		list = vector<Effect*>();
	}

	~EffectContainer(){ }

	void add( EffectType t, float base[3] )
	{
		Effect* e = new Effect(cam, tC, mC, t, base);
		list.push_back(e);
		renderer->add(e);
		mover->add(e);

	}

	void remove( int i )
	{
		Effect* e = list.at(i);
		delete e;
		list.erase(list.begin()+i);
	}
	void clean( void )
	{
		for(unsigned int i = 0; i < (int) list.size(); i++)
		{
			Effect* e = list.at(i);

			if(e->shouldBeRemoved())
			{
				renderer->remove(e);
				mover->remove(e);
				remove(i);
				i--; 
			}
		}
	}

	void cleanAll( void )
	{
		for(unsigned int i = 0; i < (int) list.size(); i++)
		{
			Effect* e = list.at(i);
			
			renderer->remove(e);
			mover->remove(e);
			remove(i);
			i--; 
		}
	}
};

#endif