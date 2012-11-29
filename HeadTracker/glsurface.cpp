#include "glsurface.h"



glSurface::glSurface(float w, float h, int nLines, Vector3 pos, Vector3 vrot, Vector3 rgbColor, Vector3 normal)
{
	this->w = w;
	this->h = h;

	this->pos = pos;
	this->vrot = vrot;
	this->normal = normal;
	this->rgbColor = rgbColor;

	this->nLines = nLines;

}

void glSurface::draw()
{
	glPushMatrix();

	//glLoadIdentity();
	glTranslatef(this->pos[0], this->pos[1], this->pos[2]);

	glRotatef(vrot[0], 1, 0, 0);
	glRotatef(vrot[1], 0, 1, 0);
	glRotatef(vrot[2], 0, 0, 1);

	


	glLineWidth(1);
	glColor3fv(this->rgbColor);

	glBegin(GL_LINES);
	
	glNormal3fv(normal);

	float dx = (this->nLines > 0? float(w)/this->nLines : w );
	float dz = (this->nLines > 0? float(h)/this->nLines : h );

	float zLimit = floor(float(h)/2);
	float xLimit = floor(float(w)/2);
	for(float i = -zLimit; i <= zLimit; i += dz)
	{
		glVertex3f(-xLimit, 0, i);
		glVertex3f(xLimit, 0, i);

		for( float j = -xLimit; j <= xLimit; j += dx)
		{
			glVertex3f(i, 0, -zLimit);
			glVertex3f(i, 0, zLimit);
		}

		
	}

	glEnd();

	


	


	glPopMatrix();
}



//draw(GLuint texture);