#pragma once
//must import stdlib from tinyxml2 first otherwise 
//exit redefinition from glut breaks everything
#include "tinyxml2.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace tinyxml2;

class SceneTree;
class Component;
class ModelComponent;
class GroupComponent;

typedef struct vector_struct
{
	//initialize a vector as a null vector
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
		
		
		//Load Components from XML
		vector<Component *> LoadXML(const char *file);
	
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
};

class ModelComponent : public Component 
{
	private:
		string model;
		//Use as a giant pile of vertices you go through as an array
		int v_size;
		Vector3D* vertices;
	
	public:
		ModelComponent(const char *model);
		~ModelComponent();
		void renderModel();
};

class GroupComponent : public Component
{
private:
	//Each group component can only have one transform and one rotate
	Vector3D translate;
	Vector3D rotate;
	float rotateangle;
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
	GroupComponent(XMLElement *current);
	~GroupComponent();
	void renderGroup();
};