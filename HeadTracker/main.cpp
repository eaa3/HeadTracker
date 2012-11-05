
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <glut.h>
#include <opencv2/opencv.hpp>
#include "Model3D.h"
#include "tld/Predator.h"
#include "videohandler.h"
#include "glsurface.h"

const int FRAMEW = 640;
const int FRAMEH = 480;
const string tldwindow_name = "TLD Frame View";
const string tldroi_name = "TLD ROI View";

float RADIUS = 400.0f;

using namespace tld;


Model3D m;
Params p(TLD_CONFIG_FILE);
Predator predator(&p);
VideoHandler v(640, 480);
Vector3 color = Vector3(0.3f,0.5f,0.3f);
glSurface surface1(200, 200, 50, Vector3(0, -40, 0), Vector3(0,0,0), color, Vector3(0,1,0));
glSurface surface2(200, 200, 50, Vector3(-100, 0, 0), Vector3(0,0,90), color, Vector3(1,0,0));
glSurface surface3(200, 200, 50, Vector3(100, 0, 0), Vector3(0,0,90), color, Vector3(1,0,0));
glSurface surface4(200, 200, 50, Vector3(0, 0, 100), Vector3(90,0,0), color, Vector3(0,0,-1));



bool* key = new bool[256];


Vector3 originalCamPos(0,0, -RADIUS);
Vector3 camPos = originalCamPos;

BoundingBox originalBB;
BoundingBox selectedBB;
//mouse vars
bool grabbed, grab_allowed, selected, tracking_started;


GLfloat ambientColor[] = {0.3f, 0.4f, 0.3f, 1.0f};

//Positioned light
GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPos0[] = {-100.0f, 50.0f, -50.0f, 1.0f};

//Directed diffuse light
GLfloat lightColor1[] = {5.0f, 2.0f, 2.0f, 1.0f};
GLfloat lightPos1[] = {30.0f, -30.0f, -50.0f, 0.0f};

//Directed specular light
GLfloat lightColor2[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos2[] = {camPos[0], camPos[1], camPos[2], 0.0f};
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
	float d = 10;
	if( key['a'] ) camPos += Vector3(-d, 0, 0);
	if( key['d'] ) camPos += Vector3(d, 0, 0);
	if( key['w'] ) camPos += Vector3(0, -d, 0);
	if( key['s'] ) camPos += Vector3(0, d, 0);
	if( key['q'] ) camPos += Vector3(0, 0, d);
	if( key['e'] ) camPos += Vector3(0, 0, -d);

	if( key['i'] ) RADIUS-=d;
	if( key['o'] ) RADIUS+=d;

	glutPostRedisplay();
	
}

void acquireFrameAndProcess(int value)
{
	if( !v.acquire() )
	{
		printf("could not acquire an image, video source is broken!\n");
	}

	

	if( tracking_started )
	{
		predator.processFrame(v.currentFrame);
		selectedBB = predator.currBB;

		if( predator.currBB.valid )
		{
			Vector3 c1(-predator.prevBB.x + float(predator.prevBB.w)/2, predator.prevBB.y + float(predator.prevBB.h)/2, 0);
			Vector3 c2(-predator.currBB.x + float(predator.currBB.w)/2, predator.currBB.y + float(predator.currBB.h)/2, 0);
			
			Vector3 motion = c2 - c1;

			motion[0] = -motion[0]/2;
			motion[1] = -motion[1]/2;
			camPos += motion;
			
			float zoomFactor = sqrtf(originalBB.getArea())/sqrtf(predator.currBB.getArea()) ;
			
			camPos *= RADIUS*zoomFactor/camPos.norm();
			
			//printf("motion %.2f %.2f %.2f\n", motion[0], motion[1], zoomFactor);
		}
		else
		{
			camPos = originalCamPos;
		}
		

	}

	if( !tracking_started || (predator.currBB.valid && tracking_started)  ) draw_box(selectedBB, v.currentFrame, Scalar(255,0,0));
	imshow(tldwindow_name,v.currentFrame);

	

}

void onMouseCB( int _event, int x, int y, int flags, void* param)
{
	printf("\rgrabbed(%d) grab_allowed(%d) tracking_started(%d)\n", grabbed, grab_allowed, tracking_started);
	//if( grabbed && tracking_started ) return;

    switch( _event )
    {
    case CV_EVENT_LBUTTONDBLCLK:

        //First patch was selected
		//printf("LEFT DOUBLE CLICK!\n");
        //printf("First patch was selected!\n");

        //Some processing


		if ( selectedBB.isInside(x,y) )
		{
			tracking_started = true;
		}
		else
		{
			grab_allowed = true;
			grabbed = false;
			tracking_started = false;
			selectedBB.setWH(0,0);
			selectedBB.valid = 0;
		}
		
		if( tracking_started ){
			printf("START TRACKING!\n");
			
			predator.reset();
			predator.selectObject(v.currentFrame, selectedBB);
			originalBB = selectedBB;
		}
		


        break;
    case CV_EVENT_MOUSEMOVE:
		

		if( grab_allowed && grabbed  )
		{
			selectedBB.setWH(x - selectedBB.x,y - selectedBB.y);
		}

        break;
    case CV_EVENT_LBUTTONDOWN:

        //printf("mouse left button down!\n");

		if( grab_allowed && !grabbed )
		{
			selectedBB.setXY(x,y);
			selectedBB.setWH(0,0);
			grabbed = true;

		}



        break;
    case CV_EVENT_LBUTTONUP:
        printf("mouse left button up!\n");

		if( grabbed ) {
			grabbed = false;
			grab_allowed = false;
		}


        break;
    default:
        printf("another event\n");
        break;
    }


}


void display(void)
{
	
	acquireFrameAndProcess(0);

	/* clear all pixels */
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	/* draw white polygon (rectangle) with corners at
	* (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	*/
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Sets the camera position and orientation
	

	gluLookAt(camPos[0],camPos[1],camPos[2],0,0,0,0,1,0);
	lightPos2[0] = camPos[0];
	lightPos2[1] = camPos[1];
	lightPos2[2] = camPos[2];

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
	surface1.draw();
	surface2.draw();
	surface3.draw();
	surface4.draw();

	
	/* don't wait!
	* start processing buffered OpenGL routines
	*/
	for(int i = 0; i < 256; i++) key[i] = false;
	

	glutSwapBuffers();
	
}
void init (void)
{
	

	/* select clearing (background) color */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, double(FRAMEW)/FRAMEH, 1, 800);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m.load("models/eagle.obj");

	m.translation[2] = 50;

	glEnable(GL_DEPTH_TEST);


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);


	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	namedWindow(tldwindow_name);

	grabbed = tracking_started = selected = false;
	grab_allowed = true;
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

	

	setMouseCallback(tldwindow_name, onMouseCB, &v.currentFrame);

	glutMainLoop();
	return 0; /* ISO C requires main to return int. */
}