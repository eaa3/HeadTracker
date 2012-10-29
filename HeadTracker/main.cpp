
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <glut.h>
#include <opencv2/opencv.hpp>
#include "Model3D.h"
#include "tld/Predator.h"


Model3D m;

const int FRAMEW = 640;
const int FRAMEH = 480;
bool* key = new bool[256];
float x = 100,y,z = -200;


GLfloat ambientColor[] = {0.3f, 0.4f, 0.3f, 1.0f};

//Positioned light
GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPos0[] = {0.0f, 0.0f, 0.0f, 1.0f};

//Directed diffuse light
GLfloat lightColor1[] = {5.0f, 2.0f, 2.0f, 1.0f};
GLfloat lightPos1[] = {30.0f, -30.0f, 0.0f, 0.0f};

//Directed specular light
GLfloat lightColor2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos2[] = {x, y, z, 0.0f};
GLfloat specularity[4]= {0.9f,0.9f,0.9f,1.0};
GLint specMaterial = 40;


void keyDown(unsigned char keyCode, int x, int y)
{
	key[keyCode] = true;
	printf("down!\n");
}

void keyUp(unsigned char keyCode, int x, int y)
{
	key[keyCode] = false;
	printf("up!\n");
}

void keyOp()
{

	if( key['a'] ) x-=10;
	if( key['d'] ) x+=10;
	if( key['w'] ) y-=10;
	if( key['s'] ) y+=10;
	if( key['q'] ) z+=10;
	if( key['e'] ) z-=10;

	glutPostRedisplay();
}


void display(void)
{
	

	/* clear all pixels */
	glClear (GL_COLOR_BUFFER_BIT);
	/* draw white polygon (rectangle) with corners at
	* (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	*/
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Sets the camera position and orientation
	

	gluLookAt(x,y,z,0,0,0,0,1,0);
	lightPos2[0] = x;
	lightPos2[1] = y;
	lightPos2[2] = z;

	//Setting up lights
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    glMaterialfv(GL_FRONT,GL_SPECULAR, specularity);

    glMateriali(GL_FRONT,GL_SHININESS,specMaterial);

    glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightPos2);
	
	//Sets the object position and orientation
	m.draw();

	
	/* don't wait!
	* start processing buffered OpenGL routines
	*/

	for(int i = 0; i < 256; i++ ) key[i] = false;
	glFlush();
	glutSwapBuffers();
	
}
void init (void)
{
	
	/* select clearing (background) color */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, double(FRAMEW)/FRAMEH, 10, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m.load("models/eagle.obj");

	//glEnable(GL_DEPTH_TEST);


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);


	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}

void deinit()
{
	delete [] key;
}
/*
* Declare initial window size, position, and display mode
* (single buffer and RGBA). Open window with "hello"
* in its title bar. Call initialization routines.
* Register callback function to display graphics.
* Enter main loop and process events.
*/
int main(int argc, char** argv)
{
	atexit(deinit);
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (FRAMEW, FRAMEH);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Voxar - HeadTracker");
	init ();

	

	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(keyOp);
	glutMainLoop();
	return 0; /* ISO C requires main to return int. */
}