#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

#define ANG2RAD M_PI/180.0

typedef struct vector_struct 
{
	//initialize a vector3D as a null vector
	vector_struct() : x(0.0f), y(0.0f), z(0.0f) {}
	vector_struct(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	//do not abuse - use with explicitly float arrays of size > 3
	vector_struct(const float x[3]) : x(x[0]), y(x[1]), z(x[2]) {}

	void printVector(std::ofstream& fp)
	{
		fp << x << " " << y << " " << z << " ";
	}


	float x, y, z;
} Vector3D;


Vector3D crossProduct(Vector3D&& left, Vector3D&& right);
Vector3D crossProduct(Vector3D& left, Vector3D& right);
Vector3D normalize(Vector3D&& vec);
Vector3D normalize(Vector3D& vec);

