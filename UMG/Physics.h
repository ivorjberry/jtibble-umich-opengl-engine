#pragma once
#include "Collidable.h"
#include "Render.h"
#include <vector>
#include <cfloat>
#include <algorithm>



class BoundingBox: public Renderable
{
	Collidable* myCollidable;
public:
	BoundingBox( Collidable* c)
	{
		myCollidable = c;
	}

	bool boxForCollidable( Collidable* c)
	{
		if(c == myCollidable)
			return true;
		else return false;
	}

	void render( void )
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if( myCollidable->isPhysicsActive() )
		{
		
			if( myCollidable->shape == COLLIDABLE_BOX)
			{
				renderBox();
			}
			else if( myCollidable->shape == COLLIDABLE_POINT)
			{
				renderPoint();
			}
			else
			{
				renderFailShape();
			}
		}
	}


	//IF the type is a COLLIDABLE_BOX, draw it
	void renderBox()
	{
	
		float box[24];

		for(int x=0; x<myCollidable->getNumElements(); x++)
		{

			myCollidable->getBox(x, box);

			glPushMatrix();
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				
				glBegin(GL_QUADS);
					glVertex3f(box[0], box[1], box[2]); //front
					glVertex3f(box[3], box[4], box[5]);
					glVertex3f(box[6], box[7], box[8]);
					glVertex3f(box[9], box[10], box[11]);
					
					glVertex3f(box[12], box[13], box[14]); //back
					glVertex3f(box[15], box[16], box[17]);
					glVertex3f(box[18], box[19], box[20]);
					glVertex3f(box[21], box[22], box[23]);

					glVertex3f(box[12], box[13], box[14]); //left
					glVertex3f(box[15], box[16], box[17]);
					glVertex3f(box[3], box[4], box[5]);
					glVertex3f(box[0], box[1], box[2]);
					
					glVertex3f(box[9], box[10], box[11]); //right
					glVertex3f(box[6], box[7], box[8]);
					glVertex3f(box[18], box[19], box[20]);
					glVertex3f(box[21], box[22], box[23]);

				glEnd();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPopMatrix();

		} //end for loop
	}

	void renderPoint()
	{
		float p[3];

		int n = myCollidable->getNumElements();
		for(int x = 0; x<n; x++)
		{
			myCollidable->getPoint(x,p);
			//p[1]+=0.5;
			float size = 0.3f;
			
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINES);
				glVertex3f(p[0] - size, p[1]       , p[2]       );
				glVertex3f(p[0] + size, p[1]       , p[2]       );
				glVertex3f(p[0]       , p[1] - size, p[2]       );
				glVertex3f(p[0]       , p[1] + size, p[2]       );
				glVertex3f(p[0]       , p[1]       , p[2] - size);
				glVertex3f(p[0]       , p[1]       , p[2] + size);
			glEnd();
		}
	}

	void renderFailShape()
	{

	}

};






using namespace std;





class Physics
{
	vector<Collidable*> list;
	vector<Collidable*> obstacleList;
	vector<BoundingBox*> boundingBoxes;
	bool drawBoundingBoxes;
	vector<vector<bool> >collisionLegal;

	//THE PAIN; THE GAME
	bool collisionBoxBox(float*, float*);
	bool collisionPointBox(float*, float*);

	Render* renderer;

public:
	Physics( Render* r)
	{
		drawBoundingBoxes = false;
		renderer = r;
		list = vector<Collidable*>();
		collisionLegal = vector<vector<bool> >();
		fillCollisionLegalArray(collisionLegal);
	}
	~Physics(void){}


	//This is the function that is responsible for iterating through all the physics 
	//objects and checking if they do or do not collide with each other. 
	void step(void);
	
	void LOS( Collidable* );

	void add( Collidable* c )
	{
		list.push_back(c);
		BoundingBox* bb = new BoundingBox(c);
		boundingBoxes.push_back(bb);
		renderer->add(bb);

		if(drawBoundingBoxes)
			bb->enableRender();
		else 
			bb->disableRender();
	}

	void addAsObstacle( Collidable* c)
	{
		obstacleList.push_back(c);
	}

	void removeAsMoving( Collidable* c)
	{
		//remove from movingObjects list
		for(int x=0; x<(int)list.size(); x++)
		{
			if(list.at(x) == c)
			{
				list.erase(list.begin() + x);
				break;
			}
		}

		//remove bounding box graphics from renderer
		for(int x=0; x<(int)boundingBoxes.size(); x++)
		{
			if(boundingBoxes.at(x)->boxForCollidable(c))
			{
				renderer->remove(boundingBoxes.at(x));
				delete boundingBoxes.at(x);
				boundingBoxes.at(x) = 0;
				boundingBoxes.erase(boundingBoxes.begin() + x);
				return;
			}
			
		}
	}

	void removeAsObstacle( Collidable* c)
	{
		//remove from list
		for(int x=0; x<(int)list.size(); x++)
		{
			if(list.at(x) == c)
			{
				list.erase(list.begin() + x);
				break;
			}
		}

		//remove from obstacleList
		for(int x=0; x<(int)obstacleList.size(); x++)
		{
			if(obstacleList.at(x) == c)
			{
				obstacleList.erase(obstacleList.begin() + x);
				break;
			}
		}

		//remove bounding box graphics from renderer
		for(int x=0; x<(int)boundingBoxes.size(); x++)
		{
			if(boundingBoxes.at(x)->boxForCollidable(c))
			{
				renderer->remove(boundingBoxes.at(x));
				delete boundingBoxes.at(x);
				boundingBoxes.at(x) = 0;
				boundingBoxes.erase(boundingBoxes.begin() + x);
				return;
			}
			
		}
	}

	bool collisionDetectionAllowed( CollidableType, CollidableType );

	void flipBoundingBoxes( void );

	void fillCollisionLegalArray( vector<vector<bool> >& );


};




