
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
#include "FaceDetector.h"

const int FRAMEW = 640;
const int FRAMEH = 360;//360;
const string tldwindow_name = "TLD Frame View";
const string tldroi_name = "TLD ROI View";

float RADIUS = 90.0f;

GLfloat realWindowW = 47.61f; //29.0f;
GLfloat realWindowH = 27.5f; //15.4f;

GLfloat fx1 = -realWindowW/2, fx2 = realWindowW/2, fy1 = -realWindowH/2, fy2 = realWindowH/2;
GLfloat dn = RADIUS, f = 4*realWindowW;

char* fname = "110.txt";
FILE* file = NULL;

using namespace tld;

bool l0On = false, l1On = false, l2On = false;

Model3D m;
Params p(TLD_CONFIG_FILE);
Predator predator(&p);
FaceDetector faceDetector("c:/opencv/data/haarcascades/haarcascade_frontalface_alt.xml");
VideoHandler v(p.frame_w, p.frame_h);
Vector3 color = Vector3(0.3f,0.5f,0.3f);

glSurface surface0(realWindowW, f, 25, Vector3(0, fy2, 0), Vector3(0,0,0), color, Vector3(0,1,0));
glSurface surface1(realWindowW, f, 25, Vector3(0, fy1, 0), Vector3(0,0,0), color, Vector3(0,1,0));
glSurface surface2(realWindowH, f, 25, Vector3(fx1, 0, 0), Vector3(0,0,90), color, Vector3(1,0,0));
glSurface surface3(realWindowH, f, 25, Vector3(fx2, 0, 0), Vector3(0,0,90), color, Vector3(1,0,0));



int key[256];


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
	float d = 5;
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
	
		m.angle[1] += r; 
	}
	if( key['6'] == 1 ){
	
		m.angle[1] -= r;
	}
	if( key['8'] == 1 ){
	
		m.angle[0]+=r;
	}
	if( key['2'] == 1 ){
	
		m.angle[0]-=r;
	}

	if( key['+'] == 1 ){

		m.translation[2] +=r;
	}
	if( key['-'] == 1 ){

		m.translation[2] -=r;

	}

	if( key[27] == 1 )
	{
		exit(0);
	}


	if ( key['b'] == 1 ) 
	{
			l0On? glDisable(GL_LIGHT0) : glEnable(GL_LIGHT0);

			l0On = !l0On;
	}
    if ( key['n'] == 1 )
	{
		glEnable(GL_LIGHT1);

		l1On? glDisable(GL_LIGHT1) : glEnable(GL_LIGHT1);

		l1On = !l1On;
	}
    if( key['m'] == 1 ) 
	{
		glEnable(GL_LIGHT2);

		l2On? glDisable(GL_LIGHT2) : glEnable(GL_LIGHT2);

		l2On = !l2On;
	}


	//glutPostRedisplay();
	
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




		if( !predator.currBB.valid )
		{
			faceDetector.detect(v.currentFrame);

			if( faceDetector.detectionResult.size() )
			{
				predator.currBB = BoundingBox(faceDetector.detectionResult[0]);
				predator.currBB.valid = true;
			}
		}

		if( predator.currBB.valid )
		{
			

			Vector3 c2;
			
			c2 = Vector3(predator.currBB.x + float(predator.currBB.w)/2, predator.currBB.y + float(predator.currBB.h)/2, 0);
			
			
	
			float zoomFactor = sqrtf(originalBB.getArea())/sqrtf(predator.currBB.getArea());
			

			float z = -0.3756*predator.currBB.w + 126.73;

			Vector3 head(c2[0] - FRAMEW/2, -(c2[1] - FRAMEH/2), -z );

			//fprintf(file,"%d %d %f %f \n",predator.currBB.w, predator.currBB.h, RADIUS, -head[2]);

			head[0] = toCm(head[0], -FRAMEW/2, -realWindowW/2, FRAMEW/2, realWindowW/2);
			head[1] = toCm(head[1], -FRAMEH/2, -realWindowH/2, FRAMEH/2, realWindowH/2);

			head[0] *= 1.0f;

			head[1] *= 0.7f;

			head[1] += realWindowH/2 - realWindowH*0.15;
			//head[1] -=  realWindowW/2;

			camPos = head;

		}
		else
		{
			//camPos = originalCamPos;
		}
		

	}

	

	if( !tracking_started || (predator.currBB.valid && tracking_started)  ) draw_box(selectedBB, v.currentFrame, Scalar(255,0,0));
	
	if(  !tracking_started )
	{
		faceDetector.detect(v.currentFrame);

		if( faceDetector.detectionResult.size() > 0 )
		{

			selectedBB = BoundingBox(faceDetector.detectionResult[0]);
			originalBB = selectedBB;

			predator.reset();
			predator.selectObject(v.currentFrame, selectedBB);
			grab_allowed = false;
			grabbed = false;
			tracking_started = true;
		}

		
		
		
	}

	imshow(tldwindow_name,v.currentFrame);
	
	printf("\rCamPos(%.2f,%.2f,%.2f)\r", camPos[0],camPos[1],camPos[2]);
	
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

	acquireFrameAndProcess(0);
	/* clear all pixels */
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	/* draw white polygon (rectangle) with corners at
	* (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Sets the camera position and orientation
	
	gluLookAt(camPos[0],camPos[1],camPos[2], camPos[0],camPos[1],0,0,1,0);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	dn = 1.0f;
	double x1 = dn*(fx1 + camPos[0])/fabs(camPos[2]);
	double x2 = dn*(fx2 + camPos[0])/fabs(camPos[2]);
	double y1 = dn*(fy1 - camPos[1])/fabs(camPos[2]);
	double y2 = dn*(fy2 - camPos[1])/fabs(camPos[2]);
	
	double df = f;
	glFrustum(x1,x2,y1,y2,dn,df);

	//printf("\rFrustum(%.2f,%.2f,%.2f,%.2f,%.2f,%.2f) CamPos(%.2f,%.2f,%.2f)", x1,x2,y1,y2,dn,df, camPos[0],camPos[1],camPos[2]);	
	
	
	
	


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
	

	glPushMatrix();


	//glRotatef(-30, 1, 0, 0);
	//glRotatef(30, 0, 1, 0);
	


	glutSolidCube(5);

	glPushMatrix();

	glutSolidCube(5);

	glPopMatrix();



	glPushMatrix();
	glTranslatef(-10,-10, 20);

	glutWireTeapot(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50,-5, 50);

	glRotatef(30, 1, 1, 0);

	m.draw();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(20,-10, 30);

	glColor3f(1,0,0);

	glutSolidCube(5);

	glTranslatef(-30,20, -40);

	glColor3f(0,0,1);
	glutSolidCube(5);

	glTranslatef(15,-5, 0);

	glColor3f(0,1,0);
	glutSolidCube(5);

	glPopMatrix();

	


	
	
	
	

	glPopMatrix();
	
	surface0.draw();
	surface1.draw();
	surface2.draw();
	surface3.draw();
	


	
	//surface4.draw();

	
	/* don't wait!
	* start processing buffered OpenGL routines
	*/
	//for(int i = 0; i < 256; i++) key[i] = false;	

	glutSwapBuffers();
	glutPostRedisplay();
	
}
void init (void)
{
	/* select clearing (background) color */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	
	glFrustum(fx1,fx2,fy1,fy2,dn,f);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m.load("models/eagle.obj");

	//m.translation[2] = 15;
	m.scaleFactor = 0.2f;

	glEnable(GL_DEPTH_TEST);


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	
	


	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	namedWindow(tldwindow_name);

	grabbed = tracking_started = selected = false;
	grab_allowed = true;


	file = fopen(fname,"w");
}


void deinit()
{

	fclose(file);
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
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (FRAMEW, FRAMEH);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Voxar - HeadTracker");
	init ();	
	atexit(deinit);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(keyOp);	

	setMouseCallback(tldwindow_name, onMouseCB, &v.currentFrame);
	glutFullScreen();
	glutMainLoop();

	return 0; /* ISO C requires main to return int. */
}