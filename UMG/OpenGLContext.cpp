#include "OpenGLContext.h"


LRESULT CALLBACK WndProc(	HWND 	hWnd,
								UINT	message,
								WPARAM	wParam,
								LPARAM	lParam	);


OpenGLContext::OpenGLContext(void){ }

void OpenGLContext::initialize(	HINSTANCE 	hInstance,
								HINSTANCE 	hPrevInstance,
								LPSTR 		lpCmdLine,
								LPCTSTR		appName,
								int			nCmdShow )
{
	// Register Window style
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance 		= hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush for OpenGL window
	wc.hbrBackground	= NULL;		
	
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= appName;

	// Register the window class
	int success = (int)RegisterClass(&wc);
	//	return FALSE;

    // Get the Window handle and Device context to the desktop
	hDesktopWnd = GetDesktopWindow();
	hDesktopDC = GetDC(hDesktopWnd);

    // Get the screen size
	nScreenX = GetDeviceCaps(hDesktopDC, HORZRES);
	nScreenY = GetDeviceCaps(hDesktopDC, VERTRES);

    // Release the desktop device context
    ReleaseDC(hDesktopWnd, hDesktopDC);

	// Create the main application window
	hWnd = CreateWindow(
				appName,
				appName,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
				WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	
				// Window position and size
				0, 0,
				nScreenX, nScreenY,
				NULL,
				NULL,
				hInstance,
				NULL);

	int errorCode = GetLastError();
	// If window was not created, quit
	//if(hWnd == NULL)
	//	return FALSE;

	//Make sure window is enabled for accepting keyboard and mouse inputs
	if (IsWindowEnabled(hWnd) == 0)
		EnableWindow(hWnd,TRUE);

	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
}


OpenGLContext::~OpenGLContext(void){ }

// Select the pixel format for a given device context
void OpenGLContext::SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		32,								// Want 32 bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		16,								// Size of depth buffer
		0,								// Not used to select mode
		0,								// Not used to select mode
		0,	            				// Not used to select mode
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void OpenGLContext::ChangeSize(GLsizei w, GLsizei h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


	// Set the clipping volume
	float aspectRatio = (float)nScreenX / (float)nScreenY;
	float reversedRatio = (float)nScreenY / (float)nScreenX;


	if(reversedRatio > aspectRatio)
		gluPerspective(90.0, aspectRatio, 1.0, 2900.0);
	else //default
		gluPerspective(35.0*aspectRatio, aspectRatio, 1.0, 2900.0);

    //WHERE GLULOOKAT() CAME FROM
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_FOG);
	//glClearColor(0.035f, 0.106f, 0.114f, 1.0f);
	float FogCol[3]={0.035f,0.106f,0.114f}; // Define a nice light grey (clear color)

	//float FogCol[3]={1.0f, 1.0f, 1.0f}; // Define a nice light grey (clear color)
	glFogfv(GL_FOG_COLOR,FogCol);     // Set the fog color 
	glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
	glFogf(GL_FOG_START, 100.f);
	glFogf(GL_FOG_END, 10000.f);


	// Somewhere in the initialization part of your program…
	glEnable(GL_LIGHTING);
/*	glEnable(GL_LIGHT0);
 
	// Create light components
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 500.0f, 500.0f, -500.0f, 1.0f };
 
	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
*/
	glEnable(GL_NORMALIZE);
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_TEXTURE_2D);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
}






// Called by timer routine to effect movement of the rectangle.
void OpenGLContext::IdleFunction(void)
{

		
}

void OpenGLContext::getWindowSize(int* a)
{
	a[0] = nScreenX;
	a[1] = nScreenY;
}