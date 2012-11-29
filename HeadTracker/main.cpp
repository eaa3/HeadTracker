
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
#include "tld/tld_util.h"

const int FRAMEW = 640;
const int FRAMEH = 480;
const string tldwindow_name = "TLD Frame View";
const string tldroi_name = "TLD ROI View";

float RADIUS = 70.0f;

GLfloat realWindowW = 47.6f;
GLfloat realWindowH = 26.77f;

GLfloat fx1 = -realWindowW/2, fx2 = realWindowW/2, fy1 = -realWindowH/2, fy2 = realWindowH/2;
GLfloat dn = 70, f = 5*realWindowW;

using namespace tld;


Model3D m;
Params p(TLD_CONFIG_FILE);
Predator predator(&p);
VideoHandler v(p.frame_w, p.frame_h);
Vector3 color = Vector3(0.3f,0.5f,0.3f);

glSurface surface0(realWindowW, f, 25, Vector3(0, fy2, 0), Vector3(0,0,0), color, Vector3(0,1,0));
glSurface surface1(realWindowW, f, 25, Vector3(0, fy1, 0), Vector3(0,0,0), color, Vector3(0,1,0));
glSurface surface2(realWindowH, f, 25, Vector3(fx1, 0, 0), Vector3(0,0,90), color, Vector3(1,0,0));
glSurface surface3(realWindowH, f, 25, Vector3(fx2, 0, 0), Vector3(0,0,90), color, Vector3(1,0,0));
glSurface surface4(realWindowW, realWindowH, 30, Vector3(0, 0, f/2), Vector3(90,0,0), color, Vector3(0,0,-1));
Cube cube(10);



int* key = new int[256];


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

float toCm(float a, float xi, float yi, float xf, float yf)
{
	float yp = yf + ((a - xf)/(xf-xi))*(yf - yi);

	return yp;
}



void keyDown(unsigned char keyCode, int x, int y)
{
	key[keyCode]++;
	printf("down!\n");
}

void keyUp(unsigned char keyCode, int x, int y)
{
	key[keyCode] = 0;
	printf("up!\n");
}

void keyOp()
{
	float d = 10;
	if( key['a'] == 1 ) camPos += Vector3(-d, 0, 0);
	if( key['d'] == 1) camPos += Vector3(d, 0, 0);
	if( key['w'] == 1) camPos += Vector3(0, -d, 0);
	if( key['s'] == 1) camPos += Vector3(0, d, 0);
	if( key['q'] == 1) camPos += Vector3(0, 0, d);
	if( key['e'] == 1) camPos += Vector3(0, 0, -d);

	if( key['i'] == 1) RADIUS-=d;
	if( key['o'] == 1) RADIUS+=d;

	int r = 5;

	if( key['4'] == 1 ){
		cube.angle[1]+=r;
		m.angle[1] += r;
	}
	if( key['6'] == 1 ){
		cube.angle[1]-=r;
		m.angle[1] -= r;
	}
	if( key['8'] == 1 ){
		cube.angle[0]+=r;
		m.angle[0]+=r;
	}
	if( key['2'] == 1 ){
		cube.angle[0]-=r;
		m.angle[0]-=r;
	}

	if( key['+'] == 1 ){
		cube.pos[2] +=r;
		m.translation[2] +=r;
	}
	if( key['-'] == 1 ){
		cube.pos[2] -=r;
		m.translation[2] -=r;

	}


	glutPostRedisplay();
	
}

void acquireFrameAndProcess(int value)
{

	//printf("BASE %.2f OTHER %.2f\n", predator.detector->baseScale, p.base_window_scale);

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
			Vector3 c1(predator.prevBB.x + float(predator.prevBB.w)/2, predator.prevBB.y + float(predator.prevBB.h)/2, 0);
			Vector3 c2(predator.currBB.x + float(predator.currBB.w)/2, predator.currBB.y + float(predator.currBB.h)/2, 0);
			
			Vector3 motion = c2 - c1;

			motion[0] = -motion[0]/2;
			motion[1] = -motion[1]/2;
			camPos += motion;
			
			float zoomFactor = sqrtf(originalBB.getArea())/sqrtf(predator.currBB.getArea()) ;
			
			camPos *= RADIUS*zoomFactor/camPos.norm();
			
			Vector3 head(c2[0] - FRAMEW/2, -(c2[1] - FRAMEH/2), -RADIUS*zoomFactor );

			head[0] = toCm(head[0], -FRAMEW/2, -realWindowW, FRAMEW/2, realWindowW);
			head[1] = toCm(head[1] - 20, -FRAMEH/2, -realWindowH, FRAMEH/2, realWindowH);

			camPos = head;

			
			//printf("motion %.2f %.2f %.2f\n", motion[0], motion[1], zoomFactor);
		}
		else
		{
			camPos = originalCamPos;
		}
		

	}

	if( !tracking_started || (predator.currBB.valid && tracking_started)  ) draw_box(selectedBB, v.currentFrame, Scalar(255,0,0));
	imshow(tldwindow_name,v.currentFrame);
	
	//printf("\rCamPos(%.2f,%.2f,%.2f)\r", camPos[0],camPos[1],camPos[2]);
	
	//glutSwapBuffers();

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
	//printf("OLHA %f\n", predator.detector->baseScale);

	acquireFrameAndProcess(0);
	/* clear all pixels */
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	/* draw white polygon (rectangle) with corners at
	* (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Sets the camera position and orientation
	
	camPos[0] /= 2;
	camPos[1] /= 2;
	//gluLookAt(camPos[0],camPos[1],camPos[2],0,0,0,0,1,0);
	gluLookAt(camPos[0],camPos[1],camPos[2], camPos[0],camPos[1],0,0,1,0);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double df = f - camPos[2];
	//double n = -camPos[2] + 1;

	//double x1 = (fx1 - camPos[0]/2)/n;
	//double x2 = (fx2 - camPos[0]/2)/n;
	//double y1 = (fy1 - camPos[1]/2)/n;
	//double y2 = (fy2 - camPos[1]/2)/n;

	double x1 = (fx1 + camPos[0]);///(fabs(camPos[2])/15);
	double x2 = (fx2 + camPos[0]);///(fabs(camPos[2])/15);
	double y1 = (fy1 - camPos[1]);////(fabs(camPos[2])/15);
	double y2 = (fy2 - camPos[1]);///(fabs(camPos[2])/15);
	
	dn = fabs(camPos[2]);
	df = 300 - fabs(camPos[2]);

	//printf("\rFrustum(%.2f,%.2f,%.2f,%.2f,%.2f,%.2f) CamPos(%.2f,%.2f,%.2f)", x1,x2,y1,y2,dn,df, camPos[0],camPos[1],camPos[2]);
	
	//dn = (1/dn)*1000;

	glFrustum(x1,x2,y1,y2,dn,f);





	glMatrixMode(GL_MODELVIEW);

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

	//cube.draw();
	m.draw();

	surface0.draw();
	surface1.draw();
	surface2.draw();
	surface3.draw();
	surface4.draw();

	
	/* don't wait!
	* start processing buffered OpenGL routines
	*/
	//for(int i = 0; i < 256; i++) key[i] = false;
	

	glutSwapBuffers();
	
}
void init (void)
{
	
	cube.pos[2] = f/2 - 2*cube.edge;

	/* select clearing (background) color */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	
	
	glFrustum(fx1,fx2,fy1,fy2,dn,f);
	//gluPerspective(45.0, double(FRAMEW)/FRAMEH, 1, 800);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m.load("models/eagle.obj");

	m.translation[2] = 50;
	m.scaleFactor = 0.15f;

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
	glutFullScreen();
	glutMainLoop();
	return 0; /* ISO C requires main to return int. */
}