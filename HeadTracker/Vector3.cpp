#include "Vector3.h"


float toRad(float angle){
    return (angle*PI)/180;
}

float toDegree(float rad)
{
	return (rad*180.0f)/PI;
}


const int Vector3::R_X = 0;
const int Vector3::R_Y = 1;
const int Vector3::R_Z = 2;
const Vector3 Vector3::ZERO(0,0,0);

/*
Vector3::Vector3(Vector3& other)
{
    for(int i = 0; i < 3; i++) this->coord[i] = other[i];
}
*/

Vector3::Vector3(float x, float y, float z)
{
    this->coord[0] = x;
    this->coord[1] = y;
    this->coord[2] = z;
}

Vector3::Vector3()
{
    this->coord[0] = this->coord[1] = this->coord[2] = 0;
}

Vector3::~Vector3()
{
    //dtor
}


Vector3::operator float*()
{
    return this->coord;
}

float& Vector3::operator[](int index)
{
    return this->coord[index];
}

float Vector3::operator*(Vector3 other)
{
    float r = 0.0f;

    for(int i = 0; i < 3; i++) r += this->coord[i]*other[i];

    return r;
}
Vector3 Vector3::operator*(float k)
{
    Vector3 r;

    for(int i = 0; i < 3; i++) r[i] = this->coord[i]*k;

    return r;
}
Vector3 Vector3::operator/(float k)
{
    Vector3 r;

    for(int i = 0; i < 3; i++) r[i] = this->coord[i]/k;

    return r;
}
Vector3 Vector3::operator+(Vector3 other)
{
    Vector3 r;

    for(int i = 0; i < 3; i++) r[i] = this->coord[i] + other[i];

    return r;
}
Vector3 Vector3::operator-(Vector3 other)
{
    Vector3 r;
    for(int i = 0; i < 3; i++) r[i] = this->coord[i] - other[i];

    return r;
}
void Vector3::operator=(Vector3 other)
{
    for(int i = 0; i < 3; i++) this->coord[i] = other[i];

}
void Vector3::operator*=(float k)
{
    for(int i = 0; i < 3; i++) this->coord[i] = this->coord[i]*k;
}
void Vector3::operator/=(float k)
{
    for(int i = 0; i < 3; i++) this->coord[i] = this->coord[i]/k;
}
void Vector3::operator+=(Vector3 other)
{
    for(int i = 0; i < 3; i++) this->coord[i] = this->coord[i] + other[i];
}
void Vector3::operator-=(Vector3 other)
{
    for(int i = 0; i < 3; i++) this->coord[i] = this->coord[i] - other[i];

}

float Vector3::norm()
{
    return (float)sqrtf((*this)*(*this));
}

void Vector3::normalize()
{
    (*this) /= this->norm();
}

float Vector3::angleBetween(Vector3 other)
{
    return ((*this)*other)/(this->norm()*other.norm());
}

Vector3 Vector3::cross(Vector3 other)
{
    Vector3 r;

    r[0] = this->coord[1]*other[2] - this->coord[2]*other[1];
    r[1] = this->coord[2]*other[0] - this->coord[0]*other[2];
    r[2] = this->coord[0]*other[1] - this->coord[1]*other[0];

    return r;

}

void Vector3::rotate(float angle, int type)
{
    angle = toRad(angle);

    float x,y,z;

    x = this->coord[0];
    y = this->coord[1];
    z = this->coord[2];

    if( type == Vector3::R_X )
    {
        this->coord[1] = y*cosf(angle) - z*sinf(angle);
        this->coord[2] = y*sinf(angle) + z*cosf(angle);
    }
    else if( type == Vector3::R_Y )
    {
        this->coord[0] = x*cosf(angle) + z*sinf(angle);
        this->coord[2] = -x*sinf(angle) + z*cosf(angle);
    }
    else if( type == Vector3::R_Z )
    {
        this->coord[0] = x*cosf(angle) - y*sinf(angle);
        this->coord[1] = x*sinf(angle) + y*cosf(angle);
    }
}

void Vector3::rotate(float angle, Vector3 u)
{
    u.normalize();
    angle = toRad(angle);

    Vector3 r = (*this);

    float cosTeta = cosf(angle);
    float sinTeta = sinf(angle);
    float oneMinusCosTeta = 1.0f - cosTeta;

    this->coord[0] = r[0]*(cosTeta + u[0]*u[0]*oneMinusCosTeta) + r[1]*(u[0]*u[1]*oneMinusCosTeta - u[2]*sinTeta) + r[2]*(u[0]*u[2]*oneMinusCosTeta + u[1]*sinTeta);
    this->coord[1] = r[0]*(u[1]*u[0]*oneMinusCosTeta + u[2]*sinTeta) + r[1]*(cosTeta + u[1]*u[1]*oneMinusCosTeta) + r[2]*(u[1]*u[2]*oneMinusCosTeta - u[0]*sinTeta);
    this->coord[2] = r[0]*(u[2]*u[0]*oneMinusCosTeta - u[1]*sinTeta) + r[1]*(u[2]*u[1]*oneMinusCosTeta + u[0]*sinTeta) + r[2]*(cosTeta + u[2]*u[2]*oneMinusCosTeta);
}

void Vector3::transform(Vector3 vi, Vector3 vj, Vector3 vk)
{
    float x,y,z;

    x = this->coord[0];
    y = this->coord[1];
    z = this->coord[2];

    for(int i = 0; i < 3; i++)
    {
        this->coord[i] = x*vi[i] + y*vj[i] + z*vk[i];
    }
}

Vector3 Vector3::proj(Vector3 other)
{
    Vector3 r;

    r = other*(((*this)*other)/(other*other));

    return r;
}
