#ifndef _GL_SURFACE_
#define _GL_SURFACE_

#include "Vector3.h"
#include <Windows.h>
#include <gl/GL.h>

class glSurface
{
public:

	Vector3 pos;
	Vector3 normal;
	Vector3 rgbColor;
	int w, h;
	int nLines;

	glSurface(int w, int h, int nLines = 0, Vector3 pos = Vector3( 0.0f, 0.0f, 0.0f), Vector3 normal = Vector3(0.0f, 1.0f, 0.0f), Vector3 rgbColor = Vector3(1.0f, 1.0f, 1.0f));

	void draw();
	void draw(GLuint texture);
};

	


#endif