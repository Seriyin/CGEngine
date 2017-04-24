#include "generator.h"

void inline generateVerticesForPatch(float quad_result[][3],
	float cur_bezier_ctrls[][4][4],
	int bezier[][4],
	int tessalation,
	ofstream& fp);

void inline calculateQuad(float quad_result[][3],
	float cur_bezier_ctrls[][4][4],
	int bezier[][4],
	float u[][4],
	float v[][4],
	int tessalation);

void inline multBezier(float u[4],
	float vertex[3],
	float P[][4][4],
	int M[][4],
	float v[4]);

void generatePlane(ofstream& fp, float length, float width)
{
	fp << 6 << endl;

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

void generateBox(ofstream& fp, float length, float height, float width) {

		fp << 36 << endl; //6 sides * 3 vertices * 2 triangles
		
		//draw bottom side
		fp << length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << -length/2 << " " << 0 << " " << width/2 << " ";
		fp << -length / 2 << " " << 0 << " " << -width / 2 << " ";

		fp << length / 2 << " " << 0 << " " << width / 2 << " ";
		fp << -length/2 << " " << 0 << " " << width/2 << " ";
		fp << length/2 << " " << 0 << " " << -width/2 << " ";
		
		//draw upper side
		fp << length / 2 << " " << height << " " << -width / 2 << " ";
		fp << -length / 2 << " " << height << " " << width / 2 << " ";
		fp << length / 2 << " " << height << " " << width / 2 << " ";

		fp << -length / 2 << " " << height << " " << -width / 2 << " ";
		fp << -length / 2 << " " << height << " " << width / 2 << " ";
		fp << length / 2 << " " << height << " " << -width / 2 << " ";

		//draw left side
		fp << -length / 2 << " " << height << " " << -width / 2 << " ";
		fp << -length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << -length / 2 << " " << height << " " << width / 2 << " ";

		fp << -length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << -length / 2 << " " << 0 << " " << width / 2 << " ";
		fp << -length / 2 << " " << height << " " << width / 2 << " ";

		//draw right side
		fp << length / 2 << " " << 0 << " " << width / 2 << " ";
		fp << length / 2 << " " << height << " " << -width / 2 << " ";
		fp << length / 2 << " " << height << " " << width / 2 << " ";

		fp << length / 2 << " " << 0 << " " << width / 2 << " ";
		fp << length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << length / 2 << " " << height << " " << -width / 2 << " ";

		//draw front side
		fp << length / 2 << " " << height << " " << width / 2 << " ";
		fp << -length / 2 << " " << height << " " << width / 2 << " ";
		fp << -length / 2 << " " << 0 << " " << width / 2 << " ";

		fp << -length / 2 << " " << 0 << " " << width / 2 << " ";
		fp << length / 2 << " " << 0 << " " << width / 2 << " ";
		fp << length / 2 << " " << height << " " << width / 2 << " ";
		//draw back side
		fp << length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << -length / 2 << " " << height << " " << -width / 2 << " ";
		fp << length / 2 << " " << height << " " << -width / 2 << " ";

		fp << length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << -length / 2 << " " << 0 << " " << -width / 2 << " ";
		fp << -length / 2 << " " << height << " " << -width / 2 << " ";
}


void generateBox(ofstream& fp, float length, float height, float width, int divisions) {
	//1 division means 4 quads per face, 2 quads in one axis, 2 quads in the other non-fixed axis.
	divisions++;
	float x_step = length / divisions;
	float y_step = height / divisions;
	float z_step = width / divisions;
	int x_pos = 0, y_pos = 0, z_pos = 0;

	fp << 36 * divisions * divisions << endl; //6 sides * number of divisions+1^2 * 2 triangles per quad * 3 vertices

	//draw bottom side
	for (z_pos = 0; z_pos < divisions; z_pos++) {
		for (x_pos = 0; x_pos < divisions; x_pos++) {

			fp << ((x_pos + 1)*x_step) - length / 2 << " " << 0 << " " << (z_pos*z_step) - width / 2 << " ";
			fp << ((x_pos + 1)*x_step) - length / 2 << " " << 0 << " " << ((z_pos + 1)*z_step) - width / 2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << 0 << " " << (z_pos*z_step) - width / 2 << " ";

			fp << (x_pos*x_step) - length / 2 << " " << 0 << " " << (z_pos*z_step) - width / 2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << 0 << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << 0 << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
		}
	}

	//draw upper side
	for (z_pos = 0; z_pos < divisions; z_pos++) {
		for (x_pos = 0; x_pos < divisions; x_pos++) {

			fp << ((x_pos + 1) * x_step) - length / 2 << " " << height << " " << (z_pos*z_step) - width / 2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << height << " " << (z_pos*z_step) - width / 2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << height << " " << ((z_pos + 1) * z_step) - width / 2 << " ";

			fp << (x_pos*x_step) - length / 2 << " " << height << " " << (z_pos*z_step) - width / 2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << height << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << height << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
		}
	}

	//draw left side
	for (y_pos = 0; y_pos < divisions; y_pos++) {
		for (z_pos = 0; z_pos < divisions; z_pos++) {

			fp << -length/2 << " " << (y_pos + 1) * y_step << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
			fp << -length/2 << " " << (y_pos + 1) * y_step << " " << (z_pos*z_step) - width / 2 << " ";
			fp << -length/2 << " " << y_pos*y_step << " " << ((z_pos + 1) * z_step) - width / 2 << " ";

			fp << -length/2 << " " << (y_pos + 1) * y_step << " " << (z_pos*z_step) - width / 2 << " ";
			fp << -length/2 << " " << y_pos*y_step << " " << (z_pos*z_step) - width / 2 << " ";
			fp << -length/2 << " " << y_pos*y_step << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
		}
	}
	//draw right side
	for (y_pos = 0; y_pos < divisions; y_pos++) {
		for (z_pos = 0; z_pos < divisions; z_pos++) {

			fp << length/2 << " " << (y_pos + 1) * y_step << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
			fp << length/2 << " " << y_pos*y_step << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
			fp << length/2 << " " << (y_pos + 1) * y_step << " " << (z_pos*z_step) - width / 2 << " ";

			fp << length/2 << " " << (y_pos + 1) * y_step << " " << (z_pos*z_step) - width / 2 << " ";
			fp << length/2 << " " << y_pos*y_step << " " << ((z_pos + 1) * z_step) - width / 2 << " ";
			fp << length/2 << " " << y_pos*y_step << " " << (z_pos*z_step) - width / 2 << " ";
		}
	}
	//draw front side
	for (y_pos = 0; y_pos < divisions; y_pos++) {
		for (x_pos = 0; x_pos < divisions; x_pos++) {
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << (y_pos + 1) * y_step << " " << width/2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << (y_pos + 1) * y_step << " " << width/2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << y_pos*y_step << " " << width/2 << " ";

			fp << (x_pos*x_step) - length / 2 << " " << (y_pos + 1) * y_step << " " << width/2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << y_pos*y_step << " " << width/2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << y_pos*y_step << " " << width/2 << " ";
		}
	}
	//draw back side
	for (y_pos = 0; y_pos < divisions; y_pos++) {
		for (x_pos = 0; x_pos < divisions; x_pos++) {
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << (y_pos + 1) * y_step << " " << -width/2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << y_pos*y_step << " " << -width/2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << (y_pos + 1) * y_step << " " << -width/2 << " ";

			fp << (x_pos*x_step) - length / 2 << " " << (y_pos + 1) * y_step << " " << -width/2 << " ";
			fp << ((x_pos + 1) * x_step) - length / 2 << " " << y_pos*y_step << " " << -width/2 << " ";
			fp << (x_pos*x_step) - length / 2 << " " << y_pos*y_step << " " << -width/2 << " ";
		}
	}
}

void generateSphere(ofstream& fp, float radius, int slices, int stacks)
{
	float i, j, lu, l, ld, alpha, alpha2, alpha3, alpha4,vertNum;

	j = -(((float)stacks) / ((float)2)); //j iterates through vert loops
	i = 0; //i iterates through verts on each loop
	fp << stacks*slices*6 << endl; //number of verts used

	while (j != ((float)stacks) / ((float)2)) {
		l = radius*cos((j*(M_PI / (stacks)))); //distance of the vert to the axis
		lu = radius*cos(((j + 1)*(M_PI / (stacks)))); //distance of the next vert to the axis

		//cycle forming quads divided into triangles joining a vert loop to the one above
		while (i != slices) {
			alpha = i*(M_PI / (((float)slices) / 2)); //angle vert makes with z axis
			alpha2 = (i + 1)*(M_PI / (((float)slices) / 2)); //angle next vert on the loop makes with z axis
			alpha3 = j*(M_PI / (stacks)); //angle vert makes with y axis
			alpha4 = (j + 1)*(M_PI / (stacks)); // angle next loop's vert makes with y axis

			//quad's left triangle
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
	int count;

	step = ((float)height) / (float)stacks; //distance traveled on y axis through each loop
	j = 0; //j iterates through vert loops
	j2 = step; //y value of next loop's vert
	i = 0; //i iterates through verts on each loop
	count = 0;

	fp << (stacks*slices * 6)+(3*slices) << endl; //number of verts used
	baseAngle = atan((float)height / (float)radius); //base's angle

	while (count!=stacks) {
		l = (height - j) / (tan(baseAngle)); //distance of the vert to the axis
		lu = (height - j2) / (tan(baseAngle));
		//base triangles loop
		if (j == 0) {
			while (i != slices) {
				alpha = i*(M_PI / (((float)slices) / 2)); //angle vert makes with z axis
				alpha2 = (i + 1)*(M_PI / (((float)slices) / 2)); //angle next vert on the loop makes with z axis
				
				fp << l*sin(alpha) << " " << j << " " << l*cos(alpha) << " ";

				fp << 0 << " " << j << " " << 0 << " ";

				fp << l*sin(alpha2) << " " << j << " " << l*cos(alpha2) << " ";


				i++;
			}
			i = 0;
		}
		while (i != slices) {
			alpha = i*(M_PI / (((float)slices) / 2)); //angle vert makes with z axis
			alpha2 = (i + 1)*(M_PI / (((float)slices) / 2)); //angle next vert on the loop makes with z axis

			//quad's left triangle
			fp << l*sin(alpha) << " " << j << " " << l*cos(alpha) << " ";

			fp << l*sin(alpha2) << " " << j << " " << l*cos(alpha2) << " ";

			fp << lu*sin(alpha) << " " << j2 << " " << lu*cos(alpha) << " ";

			//quad's right triangle
			fp << l*sin(alpha2) << " " << j << " " << l*cos(alpha2) << " ";

			fp << lu*sin(alpha2) << " " << j2 << " " << lu*cos(alpha2) << " ";

			fp << lu*sin(alpha) << " " << j2 << " " << lu*cos(alpha) << " ";
			i++;
		}
		count++;
		j = step*count;
		j2 = step*(count+1);
		i = 0;
	}
}

void generateCylinder(ofstream& fp, float radius, float height, int slices, int stacks)
{

	float alpha = 0, beta = 0, stackheight = height / stacks;
	int vertices = (int)(slices*(stacks+1)* 6);
	fp << vertices << endl;
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

//Two discus in the same spot, oriented up and down to avoid culling
void generateFlatDiscus(ofstream& fp, float inner_radius, float outer_radius,
						int slices) 
{
	float alpha = 0, beta = 0;
	int vertices = (int)(slices * 6) * 2;
	fp << vertices << endl;
	for (int i = 1; i <= slices; i++) 
	{
		//Get alpha angle based on i
		alpha = 2 * M_PI * i / slices;
		//Get beta for next slice
		beta = 2 * M_PI / slices;
		//First base triangle facing one way
		fp << inner_radius*cos(alpha) << " " << 0 << " " << inner_radius*sin(alpha) << " ";
		fp << inner_radius*cos(alpha+beta) << " " << 0 << " " << inner_radius*sin(alpha+beta) << " ";
		fp << outer_radius*cos(alpha) << " " << 0 << " " << outer_radius*sin(alpha) << " ";
		//First base triangle facing other way
		fp << inner_radius*cos(alpha + beta) << " " << 0 << " " << inner_radius*sin(alpha + beta) << " ";
		fp << inner_radius*cos(alpha) << " " << 0 << " " << inner_radius*sin(alpha) << " ";
		fp << outer_radius*cos(alpha) << " " << 0 << " " << outer_radius*sin(alpha) << " ";

		//Second base triangle facing one way
		fp << outer_radius*cos(alpha) << " " << 0 << " " << outer_radius*sin(alpha) << " ";
		fp << inner_radius*cos(alpha + beta) << " " << 0 << " " << inner_radius*sin(alpha + beta) << " ";
		fp << outer_radius*cos(alpha + beta) << " " << 0 << " " << outer_radius*sin(alpha + beta) << " ";
		//Second base triangle facing other way
		fp << inner_radius*cos(alpha + beta) << " " << 0 << " " << inner_radius*sin(alpha + beta) << " ";
		fp << outer_radius*cos(alpha) << " " << 0 << " " << outer_radius*sin(alpha) << " ";
		fp << outer_radius*cos(alpha + beta) << " " << 0 << " " << outer_radius*sin(alpha + beta) << " ";
	}
}

void generateFromPatches(ofstream & fp, ifstream & patchfile, int tessalation)
{
	//Get number of patches and fill indexes in patch array
	int n_patches;
	patchfile >> n_patches;
	int *patches = new int[n_patches*16];
	char virg;
	for (int i = 0; i < n_patches; i++) 
	{
		for (int j = 0; j < 15; j++) 
		{
			patchfile >> patches[i * 16 + j] >> virg;
		}
		patchfile >> patches[i * 16 + 15];
	}
	//Get number of control points and fill cntrlpoints array with every coord
	//all in one continuous array
	int n_ctrlpnts;
	patchfile >> n_ctrlpnts;
	float *ctrlpnts = new float[n_ctrlpnts*3];
	for (int i = 0; i < n_ctrlpnts; i++) 
	{
		patchfile >> ctrlpnts[i * 3] >> virg;
		patchfile >> ctrlpnts[i * 3 + 1] >> virg;
		patchfile >> ctrlpnts[i * 3 + 2];
	}
	//Set number of verts at the head of the file
	int n_vert = n_patches * tessalation * tessalation * 6;
	fp << n_vert << endl;
	int bezier[4][4] = 
	{
		{-1, 3, -3, 1},
		{3, -6, 3, 0},
		{-3, 3, 0, 0},
		{1, 0, 0, 0}
	};
	//Compute each coord based on each 16 ctrls coords
	float cur_bezier_ctrls[3][4][4];
	//4 vertices to build a quad
	float quad_result[4][3];
	//For each patch reuse the same matrix
	for (int i = 0; i < n_patches; i++)
	{
		int c = 0;
		for (int k=0; k < 4; k++) 
		{
			for (int l = 0; l < 4; l++,c++) 
			{
				//Patches at c (In interval -> [0,16[) points to one set of 3 coords
				//c is incremented only after updating x,y and z acording to the j offset
				//for each coord.
				for (int j = 0; j < 3; j++)
				{
					cur_bezier_ctrls[j][k][l] = ctrlpnts[patches[i*16 + c]*3+j];
				}
			}
		}
		generateVerticesForPatch(quad_result, cur_bezier_ctrls, bezier, tessalation, fp);
	}

	delete patches;
	delete ctrlpnts;
}


//Use counter-clockwise for vertex order excluively.
//Generates the vertices straight to the file from one patch.
//Will use shitty no memory algorithm. Will compute quad by quad
//And so recompute half the vertexes every row.
void inline generateVerticesForPatch(float quad_result[][3],
									 float cur_bezier_ctrls[][4][4],
									 int bezier[][4],
									 int tessalation,
									 ofstream& fp) 
{
	for(int i=0;i<tessalation;i++) 
	{
		float u_cur_tess_step = i*1.0 / tessalation;
		float u_next_tess_step = (i + 1)*1.0 / tessalation;
		float u[2][4] =
		{
			{ u_cur_tess_step * u_cur_tess_step * u_cur_tess_step, u_cur_tess_step * u_cur_tess_step, u_cur_tess_step, 1 },
			{ u_next_tess_step * u_next_tess_step * u_next_tess_step, u_next_tess_step * u_next_tess_step, u_next_tess_step, 1 }
		};
		for (int j = 0; j < tessalation; j++) 
		{
			float v_cur_tess_step = j*1.0 / tessalation;
			float v_next_tess_step = (j + 1)*1.0 / tessalation;
			float v[2][4] =
			{
				{ v_cur_tess_step * v_cur_tess_step * v_cur_tess_step, v_cur_tess_step * v_cur_tess_step, v_cur_tess_step, 1 },
				{ v_next_tess_step * v_next_tess_step * v_next_tess_step, v_next_tess_step * v_next_tess_step, v_next_tess_step, 1 }
			};
			calculateQuad(quad_result, cur_bezier_ctrls, bezier, u , v, tessalation);
			//Triangle from lower left corner to upper right corner to upper left corner
			for (int k = 2; k >= 0; k--)
			{
				for (int l = 0; l<3; l++)
				{
					fp << quad_result[k][l] << " ";
				}
			}
			//Triangle from upper right corner to lower left corner to lower right corner
			for (int k = 1; k<4; k++)
			{
				for (int l = 0; l<3; l++)
				{
					fp << quad_result[k][l] << " ";
				}
			}

		}
	}
}

//Calculates all 4 vertices for a quad for one step in the tessalation.
//Stores them into quad_result.
void inline calculateQuad(float quad_result[][3],
	float cur_bezier_ctrls[][4][4],
	int bezier[][4],
	float u[][4],
	float v[][4],
	int tessalation) 
{
	multBezier(u[0], quad_result[0], cur_bezier_ctrls, bezier, v[0]);
	multBezier(u[1], quad_result[1], cur_bezier_ctrls, bezier, v[0]);
	multBezier(u[0], quad_result[2], cur_bezier_ctrls, bezier, v[1]);
	multBezier(u[1], quad_result[3], cur_bezier_ctrls, bezier, v[1]);
}


//Bezier Matrixes Multiplication
void inline multBezier(float u[4], 
					   float vertex[3], 
					   float P[][4][4], 
					   int M[][4], 
					   float v[4]) 
{
	float interim_v[4];
	float interim_m[3][4];
	//M transpose v
	for(int i=0;i<4;i++) 
	{
		interim_v[i] = M[i][0] * v[0] + M[i][1] * v[1] + M[i][2] * v[2] + M[i][3] * v[3];
	}
	//P[][] * Interim
	for (int j = 0; j<3; j++) 
	{
		for (int i = 0; i < 4; i++)
		{
			interim_m[j][i] = P[j][i][0] * v[0] + P[j][i][1] * v[1] + P[j][i][2] * v[2] + P[j][i][3] * v[3];
		}
	}
	//U * M
	for (int i = 0; i<4; i++)
	{
		interim_v[i] = M[i][0] * u[0] + M[i][1] * u[1] + M[i][2] * u[2] + M[i][3] * u[3];
	}
	//vertex = UM Interim_M
	for (int i = 0; i < 3; i++) 
	{
		vertex[i] = interim_v[0] * interim_m[i][0] + interim_v[1] * interim_m[i][1] + interim_v[2] * interim_m[i][2] + interim_v[3] * interim_m[i][3];
	}
}