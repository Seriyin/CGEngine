#include "VectorMath.h"

#define SMALL_NUMBER 1e-18

inline bool isEqual(float x, float y)
{
	return std::abs(x - y) <= SMALL_NUMBER * std::abs(x);
	// see Knuth section 4.2.2 pages 217-218
}

Vector3D normalize(Vector3D&& vec)
{
	float norm = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (isEqual(norm,0))
	{
		if (isEqual(vec.x,0)) 
		{
			vec.x=0;
		}
		else 
		{
			vec.x /= norm;
		}
		if (isEqual(vec.y, 0))
		{
			vec.y = 0;
		}
		else
		{
			vec.y /= norm;
		}
		if (isEqual(vec.z, 0))
		{
			vec.z = 0;
		}
		else
		{
			vec.z /= norm;
		}
	}
	else
	{
		vec.x /= norm; vec.y /= norm; vec.z /= norm;
	}
	return vec;
}

Vector3D normalize(Vector3D& vec)
{
	float norm = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	//check for sqrt of negative
	if (isEqual(norm, 0))
	{
		if (isEqual(vec.x, 0))
		{
			vec.x = 0;
		}
		else
		{
			vec.x /= norm;
		}
		if (isEqual(vec.y, 0))
		{
			vec.y = 0;
		}
		else
		{
			vec.y /= norm;
		}
		if (isEqual(vec.z, 0))
		{
			vec.z = 0;
		}
		else
		{
			vec.z /= norm;
		}
	}
	else
	{
		vec.x /= norm; vec.y /= norm; vec.z /= norm;
	}
	return vec;
}


Vector3D crossProduct(Vector3D&& left, Vector3D&& right)
{
	return Vector3D(left.y * right.z - left.z * right.y,
					left.z * right.x - left.x * right.z,
					left.x * right.y - left.y * right.x);
}

Vector3D crossProduct(Vector3D& left, Vector3D& right)
{
	return Vector3D(left.y * right.z - left.z * right.y,
					left.z * right.x - left.x * right.z,
					left.x * right.y - left.y * right.x);
}

