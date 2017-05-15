#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

#define ANG2RAD M_PI/180.0

typedef struct vector_struct3D 
{
	//initialize a vector3D as a null vector
	vector_struct3D() : x(0.0f), y(0.0f), z(0.0f) {}
	vector_struct3D(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	//do not abuse - use with explicitly float arrays of size > 3
	vector_struct3D(const float x[3]) : x(x[0]), y(x[1]), z(x[2]) {}

	void printVector(std::ofstream& fp)
	{
		fp << x << " " << y << " " << z << " ";
	}

	void fillVector(std::ifstream& fp) 
	{
		fp >> x >> y >> z;
	}

	float x, y, z;
} Vector3D;

typedef struct vector_struct2D
{
	//initialize a vector3D as a null vector
	vector_struct2D() : x(0.0f), y(0.0f) {}
	vector_struct2D(const float x, const float y) : x(x), y(y) {}
	//do not abuse - use with explicitly float arrays of size > 2
	vector_struct2D(const float x[2]) : x(x[0]), y(x[1]) {}

	void printVector(std::ofstream& fp)
	{
		fp << x << " " << y << " ";
	}

	void fillVector(std::ifstream& fp)
	{
		fp >> x >> y;
	}


	float x, y;
} Vector2D;



Vector3D crossProduct(Vector3D&& left, Vector3D&& right);
Vector3D crossProduct(Vector3D& left, Vector3D& right);
Vector3D normalize(Vector3D&& vec);
Vector3D normalize(Vector3D& vec);

