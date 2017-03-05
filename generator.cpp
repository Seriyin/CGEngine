#include "generator.h"

void generatePlane(ofstream& fp, float length, float width)
{
	fp << 6 <<"\n";

	float pos_x = length / 2;
	float pos_z = width / 2;
	float neg_x = -(length / 2);
	float neg_z = -(width / 2);

	//glBegin(GL_TRIANGLES);
	//fp <<(pos_x, 0, neg_z);
	fp << pos_x << " " << 0 << " " << neg_z;
	fp << " ";
	//fp <<(neg_x, 0, neg_z);
	fp << neg_x << " " << 0 << " " << neg_z;
	fp << " ";
	//fp <<(pos_x, 0, pos_z);
	fp << pos_x << " " << 0 << " " << pos_z;
	fp << " ";
	//glEnd();

	//glBegin(GL_TRIANGLES);
	//fp <<(pos_x, 0, pos_z);
	fp << pos_x << " " << 0 << " " << pos_z;
	fp << " ";
	//fp <<(neg_x, 0, neg_z);
	fp << neg_x << " " << 0 << " " << neg_z;
	fp << " ";
	//fp <<(neg_x, 0, pos_z);
	fp << neg_x << " " << 0 << " " << pos_z;
	//glEnd();
}

void generateBox(float length, float height, float width, int divisions) {
	int quads = pow(4, divisions);
	float x_step = length / quads;
	float y_step = height / quads;
	float z_step = width / quads;
	float x_pos = 0;
	float y_pos = 0;
	float z_pos = 0;
	
	//draw bottom side
	for (z_pos = 0; z_pos < width; z_pos += z_step) {
		for (x_pos = 0; x_pos < length; x_pos += x_step) {
			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos + x_step, 0, z_pos);
			glVertex3f(x_pos + x_step, 0, z_pos + z_step);
			glVertex3f(x_pos, 0, z_pos);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos, 0, z_pos);
			glVertex3f(x_pos + x_step, 0, z_pos + z_step);
			glVertex3f(x_pos, 0, z_pos + z_step);
			glEnd();
		}
	}

	//draw upper side
	for (z_pos = 0; z_pos < width; z_pos += z_step) {
		for (x_pos = 0; x_pos < length; x_pos += x_step) {
			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos + x_step, height, z_pos);
			glVertex3f(x_pos, height, z_pos);
			glVertex3f(x_pos + x_step, height, z_pos + z_step);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos, height, z_pos);
			glVertex3f(x_pos, height, z_pos + z_step);
			glVertex3f(x_pos + x_step, height, z_pos + z_step);
			glEnd();
		}
	}

	//draw left side
	for (y_pos = 0; y_pos < height; y_pos += y_step) {
		for (z_pos = 0; z_pos < width; z_pos += z_step) {
			glBegin(GL_TRIANGLES);
			glVertex3f(0, y_pos + y_step, z_pos + z_step);
			glVertex3f(0, y_pos + y_step, z_pos);
			glVertex3f(0, y_pos, z_pos + z_step);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(0, y_pos + y_step, z_pos);
			glVertex3f(0, y_pos, z_pos);
			glVertex3f(0, y_pos, z_pos + z_step);
			glEnd();
		}
	}
	//draw right side
	for (y_pos = 0; y_pos < height; y_pos += y_step) {
		for (z_pos = 0; z_pos < width; z_pos += z_step) {
			glBegin(GL_TRIANGLES);
			glVertex3f(length, y_pos + y_step, z_pos + z_step);
			glVertex3f(length, y_pos, z_pos + z_step);
			glVertex3f(length, y_pos + y_step, z_pos);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(length, y_pos + y_step, z_pos);
			glVertex3f(length, y_pos, z_pos + z_step);
			glVertex3f(length, y_pos, z_pos);
			glEnd();
		}
	}
	//draw front side
	for (y_pos = 0; y_pos < height; y_pos += y_step) {
		for (x_pos = 0; x_pos < length; x_pos += x_step) {
			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos + x_step, y_pos + y_step, width);
			glVertex3f(x_pos, y_pos + y_step, width);
			glVertex3f(x_pos + x_step, y_pos, width);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos, y_pos + y_step, width);
			glVertex3f(x_pos, y_pos, width);
			glVertex3f(x_pos + x_step, y_pos, width);
			glEnd();
		}
	}
	//draw back side
	for (y_pos = 0; y_pos < height; y_pos += y_step) {
		for (x_pos = 0; x_pos < length; x_pos += x_step) {
			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos + x_step, y_pos + y_step, 0);
			glVertex3f(x_pos + x_step, y_pos, 0);
			glVertex3f(x_pos, y_pos + y_step, 0);
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(x_pos, y_pos + y_step, 0);
			glVertex3f(x_pos + x_step, y_pos, 0);
			glVertex3f(x_pos, y_pos, 0);
			glEnd();
		}
	}
}

void generateSphere(ofstream& fp, float radius, int slices, int stacks)
{
	float i, j, lu, l, ld, alpha, alpha2, alpha3, alpha4,vertNum;

	j = -(((float)stacks) / ((float)2)); //j iterates through vert loops
	i = 0; //i iterates through verts on each loop
	fp << stacks*slices*6 << "\n"; //number of verts used

	while (j != ((float)stacks) / ((float)2)) {
		l = radius*cos((j*(M_PI / (stacks)))); //distance of the vert to the axis
		lu = radius*cos(((j + 1)*(M_PI / (stacks)))); //distance of the next vert to the axis

		//cycle forming quads divided into triangles joining a vert loop to the one above
		while (i != slices) {
			alpha = i*(M_PI / (((float)slices) / 2)); //angle vert makes with y axis
			alpha2 = (i + 1)*(M_PI / (((float)slices) / 2)); //angle next vert on the loop makes with y axis
			alpha3 = j*(M_PI / (stacks)); //angle vert makes with z axis
			alpha4 = (j + 1)*(M_PI / (stacks)); // angle next loop's vert makes with z axis

			//quads's left triangle
			fp << l*sin(alpha) << " " << radius*sin(alpha3) << " " << l*cos(alpha) << " ";

			fp << l*sin(alpha2) << " " << radius*sin(alpha3) << " " << l*cos(alpha2) << " ";

			fp << lu*sin(alpha) << " " << radius*sin(alpha4) << " " << lu*cos(alpha) << " ";

			//quad's right triangle
			fp << l*sin(alpha2) << " " << radius*sin(alpha3) << " " << l*cos(alpha2) << " ";

			fp << lu*sin(alpha2) << " " << radius*sin(alpha4) << " " << lu*cos(alpha2) << " ";

			fp << lu*sin(alpha) << " " << radius*sin(alpha4) << " " << lu*cos(alpha) << " ";
			i++;
		}
		j++;
		i = 0;
	}
}

void generateCone(ofstream& fp, float radius, float height, int slices, int stacks) 
{
	float i, j, lu, l, alpha, alpha2, step, j2, baseAngle;

	step = ((float)height) / (float)stacks; //distance traveled on y axis through each loop
	j = 0; //j iterates through vert loops
	j2 = step; //y value of next loop's vert
	i = 0; //i iterates through verts on each loop

	fp << (stacks*slices * 6)+(3*slices) << "\n"; //number of verts used
	baseAngle = atan((float)height / (float)radius); //base's angle

	while (fabs(height - j) > TOL) {
		l = (height - j) / (tan(baseAngle)); //distance of the vert to the axis
		lu = (height - j2) / (tan(baseAngle));
		//base triangles loop
		if (j == 0) {
			while (i != slices) {
				alpha = i*(M_PI / (((float)slices) / 2)); //angle vert makes with y axis
				alpha2 = (i + 1)*(M_PI / (((float)slices) / 2)); //angle next vert on the loop makes with y axis
				
				fp << l*sin(alpha) << " " << j << " " << l*cos(alpha) << " ";

				fp << 0 << " " << j << " " << 0 << " ";

				fp << l*sin(alpha2) << " " << j << " " << l*cos(alpha2) << " ";


				i++;
			}
			i = 0;
		}
		while (i != slices) {
			alpha = i*(M_PI / (((float)slices) / 2)); //angle vert makes with y axis
			alpha2 = (i + 1)*(M_PI / (((float)slices) / 2)); //angle next vert on the loop makes with y axis

			//quads's left triangle
			fp << l*sin(alpha) << " " << j << " " << l*cos(alpha) << " ";

			fp << l*sin(alpha2) << " " << j << " " << l*cos(alpha2) << " ";

			fp << lu*sin(alpha) << " " << j2 << " " << lu*cos(alpha) << " ";

			//quad's right triangle
			fp << l*sin(alpha2) << " " << j << " " << l*cos(alpha2) << " ";

			fp << lu*sin(alpha2) << " " << j2 << " " << lu*cos(alpha2) << " ";

			fp << lu*sin(alpha) << " " << j2 << " " << lu*cos(alpha) << " ";
			i++;
		}
		j += step;
		j2 += step;
		i = 0;
	}
}

void generateCylinder(ofstream& fp, float radius, float height, int slices, int stacks)
{

	float alpha = 0, beta = 0, stackheight = height / stacks;
	int vertices = (int)(slices*(stacks+1)* 6);
	fp << vertices << "\n";
	for (int i = 1; i <= slices; i++)
	{
		//Get alpha angle based on i
		alpha = 2 * M_PI * i / slices;
		//Get beta for next slice
		beta = 2 * M_PI / slices;
		//Lower base Tri is facing down
		fp << 0 << " " << 0 << " " << 0 << " ";
		fp << radius*cos(alpha + beta) << " " << 0 << " " << radius*sin(alpha + beta) << " ";
		fp << radius*cos(alpha) << " " << 0 << " " << radius*sin(alpha) << " ";
		//Upper base Tri is facing up
		fp << " " << 0 << " " << height << " " << 0 << " ";
		fp << radius*cos(alpha) << " " << height << " " << radius*sin(alpha) << " ";
		fp << radius*cos(alpha + beta) << " " << height << " " << radius*sin(alpha + beta) << " ";
		for (int j = 1; j <= stacks; j++)
		{
			//Lateral left bottom to left upper
			fp << radius*cos(alpha) << " " << (j - 1)*stackheight << " " << radius*sin(alpha) << " ";
			fp << radius*cos(alpha + beta) << " " << j*stackheight << " " << radius*sin(alpha + beta) << " ";
			fp << radius*cos(alpha) << " " << j*stackheight << " " << radius*sin(alpha) << " ";
			//Lateral right upper to right bottom
			fp << radius*cos(alpha + beta) << " " << j*stackheight << " " << radius*sin(alpha + beta) << " ";
			fp << radius*cos(alpha) << " " << (j - 1)*stackheight << " " << radius*sin(alpha) << " ";
			fp << radius*cos(alpha + beta) << " " << (j - 1)*stackheight << " " << radius*sin(alpha + beta) << " ";
		}
	}
}
