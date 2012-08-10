#ifndef _RENDER_H_
#define _RENDER_H_

#include "Graphics.h"
#include "Renderable.h"
#include <vector>

using namespace std;

class Render{

private:
	vector<Renderable*> toRender;

public:
	void renderAll( void )
	{

		glLoadIdentity();
		//glPushMatrix();
		// Set background clearing color to dark cyan
		glClearColor(0.035f, 0.106f, 0.114f, 1.0f);
	
		// Clear the window with current clearing color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(unsigned int x = 0; x < toRender.size(); x++)
		{
			Renderable* r = toRender.at(x);
			if(r != NULL)
			{
				if(r->renderEnabled())
				{
					r->render();
				}
			}

		}
		//glPopMatrix();
	}

	void add( Renderable* r)
	{
		r->enableRender();
		toRender.push_back(r);
	}

	void remove( Renderable* r)
	{
		for(int x=0; x<(int)toRender.size(); x++)
		{
			if(toRender.at(x) == r)
			{
				toRender.erase(toRender.begin()+x);
				return;
			}
		}
	}
			
};

#endif