#ifndef PRIM_CONE
#define PRIM_CONE
#include <cmath>


//The cylinder is by default, oriented along the Y-axis,
//with the cylinder begin 
class PrimitiveCone
{

	static const int detail = 10;
	float length;
	float radius;

public:
	PrimitiveCone(float l, float d)
	{
		length = l;
		radius = d/2.0f;
	}

	void render(void)
	{
		float h = length;
		float TWOPI = 3.14159265f * 2.0f;
		

		glPushMatrix();

			glBegin(GL_TRIANGLE_FAN);
				//top center
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, h, 0.0f);
				
				//top surface (facing +Y axis)
				for(int x=0; x<detail+1; x++)
				{
					float angle = TWOPI * (float)x / (float)detail;
					float xVal = radius * (float)cos( angle );
					float zVal = radius * (float)sin( angle );
					glNormal3f(xVal, 1.0f, zVal);
					glVertex3f(xVal, 0, zVal);
				}
			glEnd();

			glBegin(GL_TRIANGLE_FAN);				
				//bottom center
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);

				//bottom circle (facing -Y axis)
				for(int x=0; x<detail+1; x++)
				{
					float angle = TWOPI * (float)x / (float)detail;
					float xVal = radius * (float)cos( angle );
					float zVal = radius * (float)sin( angle );
					glNormal3f(0.0f, -1.0f, 0.0f);
					glVertex3f(xVal, 0.0f, zVal);
				}
			glEnd();			

		glPopMatrix();
	}
};

#endif