#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "Graphics.h"

class Renderable{
	bool renderEnabledFlag;

public:
	virtual void render( void ) = 0;

	bool renderEnabled( void )
	{
		return renderEnabledFlag;
	}

	void disableRender( void )
	{
		renderEnabledFlag = false;
	}

	void enableRender( void )
	{
		renderEnabledFlag = true;
	}
};

#endif