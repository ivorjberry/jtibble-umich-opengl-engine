#ifndef OPENGL_CONTEXT_H_
#define OPENGL_CONTEXT_H_

#include <windows.h>

#define GLEW_STATIC
#include "GL/glew.h"


class OpenGLContext
{
	WNDCLASS	wc;			// Windows class structure
	HWND		hWnd;		// Storeage for window handle
	HWND		hDesktopWnd;// Storeage for desktop window handle
	HDC			hDesktopDC; // Storeage for desktop window device context
	int			nScreenX, nScreenY; // Screen Dimensions
	
	// Keep track of windows changing width and height
	GLfloat windowWidth;
	GLfloat windowHeight;

public:


	OpenGLContext(void);
	void initialize(	HINSTANCE 	hInstance,
					HINSTANCE 	hPrevInstance,
					LPSTR 		lpCmdLine,
					LPCTSTR		appName,
					int			nCmdShow );

	~OpenGLContext(void);
	
	void SetDCPixelFormat(HDC hDC);
	void ChangeSize(GLsizei w, GLsizei h);
	void IdleFunction(void);
	void getWindowSize(int*);
	
};

#endif