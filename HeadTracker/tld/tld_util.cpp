#include "tld_util.h"



float randomFloat()
{

    return ((float)rand())/((float)RAND_MAX);
}

double randomDouble()
{
    return ((double)rand())/((double)RAND_MAX);
}

int randomInt(int from, int to)
{
    return rand()%(to - from + 1) + from;
}


Mat get_roi(Mat& src, BoundingBox box, int padding)
{
    Range rowRange = Range(box.y + padding,box.yf - padding);
    Range colRange = Range(box.x + padding,box.xf - padding);

    Mat roi, temp;

    if( rowRange.size() > 0 && colRange.size() > 0 )
    {
        //temp = Mat(src, rowRange, colRange );
        //temp.copyTo(roi);
        roi = src(rowRange, colRange).clone();
        //cvtColor(roi, roi, CV_BGR2GRAY);
    }

    return roi;
}

void draw_box(BoundingBox& b, Mat& src, Scalar color)
{
    Point2i p1(b.x, b.y);
    Point2i p2(b.xf, b.yf);
    rectangle(src, p1, p2, color, 5 );
}


bool gtCompf( float& a,  float& b)
{
    return a > b;
}


bool tldSortByOverlapDesc(pair<int,float> bb1 , pair<int,float> bb2) {
	return bb1.second > bb2.second;
}

Color3f::Color3f()
{
    this->color[0] = 0.0f;
    this->color[1] = 0.0f;
    this->color[2] = 0.0f;
}

Color3f::Color3f(GLfloat r, GLfloat g, GLfloat b)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
}

Color3f::operator GLfloat*()
{
    return this->color;
}


Cube::Cube(GLfloat edge)
{
    this->edge = edge;
    pos[0] = pos[1] = pos[2] = 0.0f;
    angle[0] = angle[1] = angle[2] = 0.0f;

    //Front-face
    this->color[0] = Color3f(1.0f,0.0f,0.0f);
    //Back-face
    this->color[1] = Color3f(1.0f,0.0f,0.0f);
    //Right-face
    this->color[2] = Color3f(0.0f,1.0f,0.0f);
    //Left-face
    this->color[3] = Color3f(0.0f,1.0f,0.0f);
    //Up-face
    this->color[4] = Color3f(0.5f,0.5f,0.5f);
    //Down-face
    this->color[5] = Color3f(0.0f,0.0f,1.0f);
}

GLfloat& Cube::operator[](int i)
{
    return this->pos[i];
}

void Cube::draw()
{
    glPushMatrix();
    //glLoadIdentity();

    glTranslatef(this->pos[0],this->pos[1], this->pos[2]);

    glRotatef(this->angle[0], 1.0f, 0.0f, 0.0f);
    glRotatef(this->angle[1], 0.0f, 1.0f, 0.0f);
    glRotatef(this->angle[2], 0.0f, 0.0f, 1.0f);




    glBegin(GL_QUADS);
    //Front-face
    glColor3fv(this->color[0]);

    glVertex3f(-edge/2, edge/2, 0);
    glVertex3f(edge/2, edge/2, 0);
    glVertex3f(edge/2, -edge/2, 0);
    glVertex3f(-edge/2, -edge/2, 0);

    //Back-face
    glColor3fv(this->color[1]);
    glVertex3f(-edge/2, edge/2, -edge);
    glVertex3f(edge/2, edge/2, -edge);
    glVertex3f(edge/2, -edge/2, -edge);
    glVertex3f(-edge/2, -edge/2, -edge);

    //Right-face
    glColor3fv(this->color[2]);
    glVertex3f(edge/2, edge/2, 0);
    glVertex3f(edge/2, edge/2, -edge);
    glVertex3f(edge/2, -edge/2, -edge);
    glVertex3f(edge/2, -edge/2, 0);

    //Right-face
    glColor3fv(this->color[3]);
    glVertex3f(-edge/2, edge/2, 0);
    glVertex3f(-edge/2, edge/2, -edge);
    glVertex3f(-edge/2, -edge/2, -edge);
    glVertex3f(-edge/2, -edge/2, 0);

    //Up-face
    glColor3fv(this->color[4]);
    glVertex3f(-edge/2, edge/2, 0);
    glVertex3f(-edge/2, edge/2, -edge);
    glVertex3f(edge/2, edge/2, -edge);
    glVertex3f(edge/2, edge/2, 0);

    //Down-face
    glColor3fv(this->color[5]);
    glVertex3f(-edge/2, -edge/2, 0);
    glVertex3f(-edge/2, -edge/2, -edge);
    glVertex3f(edge/2, -edge/2, -edge);
    glVertex3f(edge/2, -edge/2, 0);

    glEnd();

    glPopMatrix();




}