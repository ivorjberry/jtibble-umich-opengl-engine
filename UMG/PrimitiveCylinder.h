#ifndef PRIM_CYL
#define PRIM_CYL
#include <cmath>

//The cylinder is by default, oriented along the Y-axis,
//with the cylinder begin 
class PrimitiveCylinder
{

	static const int detail = 10;
	float length;
	float diameter;

public:
	PrimitiveCylinder(float l, float d)
	{
		length = l;
		diameter = d;
	}

	void render(void)
	{
		float h = length / 2.0f;
		float TWOPI = 3.14159265f * 2.0f;
		float radius = diameter/2.0f;

		glPushMatrix();

			glBegin(GL_TRIANGLE_FAN);
				//top center
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, h, 0.0f);
				
				//top circle (facing +Y axis)
				for(int x=0; x<detail+1; x++)
				{
					float angle = TWOPI * (float)x / (float)detail;
					float xVal = radius * (float)cos( angle );
					float zVal = radius * (float)sin( angle );
					glNormal3f(0.0f, 1.0f, 0.0f);
					glVertex3f(xVal, h, zVal);
				}
			glEnd();

			glBegin(GL_TRIANGLE_FAN);				
				//bottom center
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, -1.0f*h, 0.0f);

				//bottom circle (facing -Y axis)
				for(int x=0; x<detail+1; x++)
				{
					float angle = TWOPI * (float)x / (float)detail;
					float xVal = radius * (float)cos( angle );
					float zVal = radius * (float)sin( angle );
					glNormal3f(0.0f, -1.0f, 0.0f);
					glVertex3f(xVal, -1.0f * h, zVal);
				}
			glEnd();			

			glBegin(GL_QUAD_STRIP);
				//outside
				for(int x=0; x<detail+1; x++)
				{
					float angle = TWOPI * (float)x / (float)detail;
					float xVal = radius * (float)cos( angle );
					float zVal = radius * (float)sin( angle );
					glNormal3f(xVal, 0.0f, zVal);
					glVertex3f(xVal, -1.0f * h, zVal);
					glNormal3f(xVal, 0.0f, zVal);
					glVertex3f(xVal, h, zVal);
				}
			glEnd();

		glPopMatrix();
	}
};

#endif

