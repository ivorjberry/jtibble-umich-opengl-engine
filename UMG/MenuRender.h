#ifndef _MENU_RENDER_H_
#define _MENU_RENDER_H_

#include <vector>
#include "Renderable.h"

class MenuRender
{
	vector<Renderable*> list;

public:
	MenuRender()
	{
	}

	void renderAll()
	{
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	
		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glBegin(GL_QUADS);
			glVertex3f(-3.0f, -2.0f, -2.5f);
			glVertex3f(-3.0f, 2.0f, -2.5f);
			glVertex3f(3.0f, 2.0f, -2.5f);
			glVertex3f(3.0f, -2.0f, -2.5f);
		glEnd();

		//go through render all menu items
		for(int x = 0; x<(int)list.size(); x++)
		{
			list.at(x)->render();
		}

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);

	}

	void clear()
	{
		list.clear();
	}

	void add( Renderable* r)
	{
		list.push_back(r);
	}


};



#endif