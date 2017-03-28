#pragma once
//must import stdlib from tinyxml2 first otherwise 
//exit redefinition from glut breaks everything
#include "tinyxml2.h"
#ifdef __APPLE__
#include <GL/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#define ANG2RAD M_PI/180.0

using namespace std;
using namespace tinyxml2;

class SceneTree;
class Component;
class ModelComponent;
class GroupComponent;

enum Op : unsigned char 
{
	ID, TR, RT, SC
};

typedef struct vector_struct
{
	//initialize a vector3D as a null vector
	vector_struct() : x(0.0f), y(0.0f), z(0.0f) {}
	float x, y, z;
} Vector3D;

typedef struct camera_struct
{
	void postAlphaDecrease()
	{
		alpha -= 0.1f;
		if (alpha <= -M_PI)
		{
			alpha = -M_PI + 0.00001f;
		}
	}

	void postAlphaIncrease()
	{
		alpha += 0.1f;
		if (alpha >= M_PI)
		{
			alpha = M_PI - 0.000001f;
		}
	}

	void postBetaDecrease()
	{
		beta -= 0.1f;
		if (beta <= -M_PI_2)
		{
			beta = -M_PI_2 + 0.00001f;
		}
	}

	void postBetaIncrease()
	{
		beta += 0.1f;
		if (beta >= M_PI_2)
		{
			beta = M_PI_2 - 0.00001f;
		}
	}

	//spherical coordinates based on alpha and beta angles + radius
	float alpha = 0.0f;
	float beta = 0.0f;
	float radius = 5.0f;
} Camera;


class SceneTree 
{
	private:
		vector<Component *> elements;
			
	public:
		//Constructor
		SceneTree(const char *file);
		//Destructor
		~SceneTree();

		void renderTree();
};

class Component 
{
	public:
		//boolean
		bool bIsGroupingComponent;
		Component(bool bIsGroupingComponent)
		{
			this->bIsGroupingComponent = bIsGroupingComponent;
		}
		~Component() {}

		virtual void renderComponent()=0;
};

class ModelComponent : public Component 
{
	private:
		string model;
		int bound_buffer_index;
		//Use as a giant pile of vertices you go through as an array
		int v_size;
		Vector3D* vertices;
	
	public:
		ModelComponent(const char *model);
		ModelComponent(string model); 
		~ModelComponent();
		void renderComponent();
		void assignBuffer(int index);
};

class GroupComponent : public Component
{
private:
	//Each group component can only have one transform and one rotate
	Vector3D translate;
	Vector3D scale;
	Vector3D rotate;
	float rotate_angle;
	//vector that holds which operation to do first, second and third.
	Op order_vector[3];

	//Each group can have subgroups, necessitates recursive handling
	//Or stack based iteration and manual pushing into its component vector from
	//outside the group component.
	vector<Component *> elements;
	
public:
	//Stack based?
	/*
	GroupComponent();
	GroupComponent PushElement(Component& element);
	*/
	//Recursive?
	GroupComponent(XMLElement* &current);
	~GroupComponent();

	void renderComponent();
};