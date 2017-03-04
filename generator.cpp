#include "generator.h"

void generatePlane(ofstream fp, float length, float width) 
{
	fp << 2
	fp << "\n"

	float pos_x = length / 2;
	float pos_z = width / 2;
	float neg_x = -(length / 2);
	float neg_z = -(width / 2);

	//glBegin(GL_TRIANGLES);
	//glVertex3f(pos_x, 0, neg_z);
	fp << pos_x << ":" << 0 << ":" << neg_z;
	fp << ":";
	//glVertex3f(neg_x, 0, neg_z);
	fp << neg_x << ":" << 0 << ":" << neg_z;
	fp << ":";
	//glVertex3f(pos_x, 0, pos_z);
	fp << pos_x << ":" << 0 << ":" << pos_z;
	fp << ":";
	//glEnd();

	//glBegin(GL_TRIANGLES);
	//glVertex3f(pos_x, 0, pos_z);
	fp << pos_x << ":" << 0 << ":" << pos_z;
	fp << ":";
	//glVertex3f(neg_x, 0, neg_z);
	fp << neg_x << ":" << 0 << ":" << neg_z;
	fp << ":";
	//glVertex3f(neg_x, 0, pos_z);
	fp << neg_x << ":" << 0 << ":" << pos_z;
	//glEnd();
}

void generateBox(ofstream fp, float length, float height, float width, int divisions)
{
	//implement here
}

void generateSphere(ofstream fp, float radius, int slices, int stacks) 
{
	//implement here
}

void generateCone(ofstream fp, float radius, float height, int slices, int stacks) 
{
	//implement here
}

void generateCylinder(ofstream fp, float radius, float height, int slices, int stacks)
{
	//optional
}
