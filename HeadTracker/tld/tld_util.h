#ifndef _UTIL_
#define _UTIL_

#include "includes.h"
#include  "boundingbox.h"
#include <Windows.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <glut.h>
#include "Vector3.h"

using tld::BoundingBox;

float randomFloat();
double randomDouble();
int randomInt();

bool gtCompf( float& a,  float& b);
bool tldSortByOverlapDesc(pair<int,float> bb1 , pair<int,float> bb2);

Mat get_roi(Mat& src, BoundingBox box, int padding = 0);
void draw_box(BoundingBox& b, Mat& src, Scalar color);

struct Color3f
{
    //0->R,1->G,2->B
    GLfloat color[3];

    Color3f();

    Color3f(GLfloat r, GLfloat g, GLfloat b);


    operator GLfloat*();

};

struct Cube
{
    GLfloat edge;
    Vector3 pos;
    GLfloat angle[3];
    Color3f color[6];

    Cube(GLfloat edge = 0.0f);

    GLfloat& operator[](int i);

    void draw();
};


#endif
