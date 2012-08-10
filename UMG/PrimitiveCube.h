#ifndef PRIM_CUBE
#define PRIM_CUBE
#include <cmath>

class PrimitiveCube
{
	float size;
	bool GLOBAL; //opposite of local
	float xdiam, ydiam, zdiam;
	float center[3];

public:
	PrimitiveCube(float SIZE, bool GLOBALCOORDINATES)
	{ 
		size = SIZE;
		GLOBAL = GLOBALCOORDINATES;
	}
	void setDiameterXYZ(float x, float y, float z)
	{
		xdiam = x;
		ydiam = y;
		zdiam = z;
	}
	void setDiameterXYZ(float* d)
	{
		xdiam = d[0];
		ydiam = d[1];
		zdiam = d[2];
	}
	void setCenter(float* c)
	{
		center[0] = c[0];
		center[1] = c[1];
		center[2] = c[2];
	}
	void render(void)
	{
		glPushMatrix();

			if(GLOBAL)
			{
				float x = xdiam / 2.0f;
				float y = ydiam / 2.0f;
				float z = zdiam / 2.0f;
				glBegin(GL_QUADS);
				//looking towards +X, +Y is up
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(center[0] - x, center[1] + y, center[2] - z); //top (+Y)
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(0.0f, xdiam);
					glVertex3f(center[0] + x, center[1] + y, center[2] - z);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(zdiam, xdiam);
					glVertex3f(center[0] + x, center[1] + y, center[2] + z);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(zdiam, 0.0f);
					glVertex3f(center[0] - x, center[1] + y, center[2] + z);
					
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(center[0] + x, center[1] - y, center[2] + z); //bottom (-Y)
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(0.0f, xdiam);
					glVertex3f(center[0] + x, center[1] - y, center[2] - z);
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(zdiam, xdiam);
					glVertex3f(center[0] - x, center[1] - y, center[2] - z);
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(zdiam, 0.0f);
					glVertex3f(center[0] - x, center[1] - y, center[2] + z);
					
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(center[0] - x, center[1] - y, center[2] - z); //front (-X)
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, ydiam);
					glVertex3f(center[0] - x, center[1] + y, center[2] - z);
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(zdiam, ydiam);
					glVertex3f(center[0] - x, center[1] + y, center[2] + z);
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(zdiam, 0.0f);
					glVertex3f(center[0] - x, center[1] - y, center[2] + z);
					
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(center[0] + x, center[1] - y, center[2] + z); //back (+X)
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, ydiam);
					glVertex3f(center[0] + x, center[1] + y, center[2] + z);
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(zdiam, ydiam);
					glVertex3f(center[0] + x, center[1] + y, center[2] - z);
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(zdiam, 0.0f);
					glVertex3f(center[0] + x, center[1] - y, center[2] - z);
					
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(center[0] + x, center[1] - y, center[2] - z); //left (-Z)
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(0.0f, ydiam);
					glVertex3f(center[0] + x, center[1] + y, center[2] - z);
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(xdiam, ydiam);
					glVertex3f(center[0] - x, center[1] + y, center[2] - z);
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(xdiam, 0.0f);
					glVertex3f(center[0] - x, center[1] - y, center[2] - z);
					
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(center[0] - x, center[1] - y, center[2] + z); //right (+Z)
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(0.0f, ydiam);
					glVertex3f(center[0] - x, center[1] + y, center[2] + z);
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(xdiam, ydiam);
					glVertex3f(center[0] + x, center[1] + y, center[2] + z);
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(xdiam, 0.0f);
					glVertex3f(center[0] + x, center[1] - y, center[2] + z);

				glEnd();
			}
			else
			{
				glScalef(size, size, size);
				glBegin(GL_QUADS);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(-0.5f, 0.5f, 0.5f); //top (+Y)
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(-0.5f, 0.5f, -0.5f);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(0.5f, 0.5f, -0.5f);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(0.5f, 0.5f, 0.5f);
				
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(-0.5f, -0.5f, 0.5f); //bottom (-Y)
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(-0.5f, -0.5f, -0.5f);
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(0.5f, -0.5f, -0.5f);
					glNormal3f(0.0f, -1.0f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(0.5f, -0.5f, 0.5f);
				
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(-0.5f, -0.5f, 0.5f); //front (+Z)
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(-0.5f, 0.5f, 0.5f);
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(0.5f, 0.5f, 0.5f);
					glNormal3f(0.0f, 0.0f, 1.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(0.5f, -0.5f, 0.5f);
				
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(0.5f, -0.5f, -0.5f); //back (-Z)
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(0.5f, 0.5f, -0.5f);
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(-0.5f, 0.5f, -0.5f);
					glNormal3f(0.0f, 0.0f, -1.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(-0.5f, -0.5f, -0.5f);
				
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(-0.5f, -0.5f, 0.5f); //left (-X)
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(-0.5f, 0.5f, 0.5f);
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(-0.5f, 0.5f, -0.5f);
					glNormal3f(-1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(-0.5f, -0.5f, -0.5f);
				
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(1.0f, 0.0f);
					glVertex3f(0.5f, -0.5f, -0.5f); //right (+X)
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(1.0f, 1.0f);
					glVertex3f(0.5f, 0.5f, -0.5f);
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, 1.0f);
					glVertex3f(0.5f, 0.5f, 0.5f);
					glNormal3f(1.0f, 0.0f, 0.0f);
					glTexCoord2d(0.0f, 0.0f);
					glVertex3f(0.5f, -0.5f, 0.5f);
				glEnd();
			}
		glPopMatrix();
	}
};
#endif
