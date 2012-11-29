#ifndef _GL_SURFACE_
#define _GL_SURFACE_

#include "Vector3.h"
#include <Windows.h>
#include <gl/GL.h>

class glSurface
{
public:

	Vector3 pos;
	Vector3 vrot; //Rotation vector
	Vector3 normal; //Normal vector, for ilumination purposes
	Vector3 rgbColor;
	float w, h;
	int nLines;

	glSurface(float w, float h, int nLines = 0, Vector3 pos = Vector3( 0.0f, 0.0f, 0.0f), Vector3 vrot = Vector3(0.0f, 0.0f, 0.0f), Vector3 rgbColor = Vector3(1.0f, 1.0f, 1.0f), Vector3 normal = Vector3(0,1,0));

	void draw();
	void draw(GLuint texture);
};

	


#endif