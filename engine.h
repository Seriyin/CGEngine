#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <string>
using namespace std;

typedef struct vector_struct
{
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
		vector<Component *> LoadXML();
	
	public:
		//Constructor
		SceneTree();
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