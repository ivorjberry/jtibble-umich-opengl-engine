#ifndef _MENUCONTAINER_H_
#define _MENUCONTAINER_H_

#include "Render.h"
#include "MenuRender.h"
#include "Texture.h"
#include "SystemTimer.h"
#include <string>

using namespace std;

enum menuType{
	MENU_NONE,
	MENU_MAIN,
	MENU_LOADING,
	MENU_WIN,
	MENU_LOSE,
	MENU_SETTINGS,
	MENU_CREDITS,
	MENU_NEW_GAME,
	MENU_PAUSE,
	MENU_END
};

enum menuReturnValue{
	MENU_RETURN_NONE,
	MENU_RETURN_QUIT_GAME,
	MENU_RETURN_QUIT_ROUND,
	MENU_RETURN_LEVEL1,
	MENU_RETURN_LEVEL2,
	MENU_RETURN_LEVEL3,
	MENU_RETURN_MUSIC_ON,
	MENU_RETURN_MUSIC_OFF,
	MENU_RETURN_UNPAUSE
};



class menuItem: public Renderable
{

public:
	float cX, cY, w, h;
	string name;
	GLuint textureNumber;
	menuType destinationGroup;
	menuReturnValue destinationAction;

	menuItem(string filename)
	{
		name = filename;
		textureNumber = 0;
		cX = 0.0f;
		cY = 0.0f;
		w = 1.0f;
		h = 1.0f;
		destinationGroup = MENU_NONE;
		destinationAction = MENU_RETURN_NONE;
	}

	void setAttributes(	float centerX,
						float centerY,
						float width,
						float height )
	{
		cX = centerX;
		cY = centerY;
		w = width;
		h = height;
	}

	void render(void)
	{	

		
		glBindTexture(GL_TEXTURE_2D, textureNumber);
		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0);

		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(cX - w/2.0f, cY - h/2.0f, -2.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(cX - w/2.0f, cY + h/2.0f, -2.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(cX + w/2.0f, cY + h/2.0f, -2.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(cX + w/2.0f, cY - h/2.0f, -2.0f);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
};



class menuGroup
{
public:
	vector<menuItem*> list;
	menuType myType;
	menuGroup(){}
	void add( menuItem* m )
	{
		list.push_back(m);
	}
};



class MenuContainer
{
	vector<menuGroup> list;
	TextureContainer* tC;
	MenuRender* mR;
	menuType currentMenu;
	float menuDelay;
	SystemTimer st;

public:
	MenuContainer( MenuRender* menuRender, TextureContainer* t)
	{
		list.resize(MENU_END);
		mR = menuRender;
		tC = t;
		menuDelay = 0.4f;
		st.startTimer();
	}

	void add( menuGroup mG )
	{
		for(int x=0; x<(int)mG.list.size(); x++)
		{
			menuItem* t = mG.list.at(x);
			t->textureNumber = tC->add(t->name);
		}
		list.at(mG.myType) = mG;
	}

	void setActive( menuType mT )
	{		

		currentMenu = mT;
		//clear the MenuRender
		mR->clear();

		//add mST to the MenuRender
		//go through all items in this group, add them to menurender
		menuGroup temp = list.at(mT);
		int numItemsInGroup = temp.list.size();
		for(int x=0; x<numItemsInGroup; x++)
		{
			mR->add(temp.list.at(x));
		}
	}

	menuType getCurrentMenu(void)
	{
		return currentMenu;
	}

	menuReturnValue click( double mx, double my)	//in the coordinates of the menus,
	{										//with 0,0 being in the exact center of the screen

		if( st.elapsedTime() < menuDelay) return MENU_RETURN_NONE;

		//go thorugh current group's items
		menuGroup currentGroup = list.at(currentMenu);
		int numItems = (int)currentGroup.list.size();

		for(int x=0; x<numItems; x++)
		{
			menuItem* m = currentGroup.list.at(x);
			double leftX = m->cX - m->w/2;
			double rightX = m->cX + m->w/2;
			double topY = m->cY + m->h/2;
			double bottomY = m->cY - m->h/2; 
			if( mx > leftX && mx < rightX && my > bottomY && my < topY )
			{
				st.startTimer();
				if(m->destinationGroup != MENU_NONE)
				{
					setActive(m->destinationGroup);
					break;
				}
				else
				{
					return m->destinationAction;
				}
			}

		}

		return MENU_RETURN_NONE;
	}

	

};

#endif