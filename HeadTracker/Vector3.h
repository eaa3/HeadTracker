#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

#define PI 3.14159265359f

float toRad(float angle);
float toDegree(float rad);

class Vector3
{
    public:

        static const Vector3 ZERO;

		//Rotation type
        static const int R_X;
        static const int R_Y;
        static const int R_Z;

        Vector3(float x, float y, float z);
        //Vector3(Vector3& other);
        Vector3();
        virtual ~Vector3();

        operator float*();

        float& operator[](int index);

        void operator=(Vector3 other);
        float operator*(Vector3 other);
        Vector3 operator*(float k);
        Vector3 operator/(float k);
        Vector3 operator+(Vector3 other);
        Vector3 operator-(Vector3 other);
        void operator*=(float k);
        void operator/=(float k) ;
        void operator+=(Vector3 other);
        void operator-=(Vector3 other);




        float norm();

        void normalize();

        float angleBetween(Vector3 other) ;

        Vector3 cross(Vector3 other);

        void rotate(float angle, int type);
        void rotate(float angle, Vector3 u);


        void transform(Vector3 vi, Vector3 vj, Vector3 vk);

        Vector3 proj(Vector3 other);



    protected:
    private:

        float coord[3];


};

#endif // VECTOR3_H
