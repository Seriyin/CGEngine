#include "engine.h"
#include <fstream>

#define P_X camera_vals.radius*cos(camera_vals.beta)*sin(camera_vals.alpha)
#define P_Y camera_vals.radius*sin(camera_vals.beta)
#define P_Z camera_vals.radius*cos(camera_vals.beta)*cos(camera_vals.alpha)

void drawCylinder(float radius, float height, int stacks, int slices);
void drawCylinder();

static struct transform_struct
{
	float r_y = 0.0f;
	float r_x = 0.0f;
	float t_x = 0.0f;
	float t_y = 0.0f;
	float t_z = 0.0f;
} transf_vals;

static Camera camera_vals;

static struct cilinder_struct
{
	int radius = 1;
	int height = 2;
	int stacks = 1;
	int slices = 6;
} cilinder_vals;


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawCylinder()
{
	drawCylinder(cilinder_vals.radius, cilinder_vals.height, cilinder_vals.stacks, cilinder_vals.slices);
}

void drawCylinder(float radius, float height, int stacks, int slices)
{

	float alpha = 0, beta = 0, stackheight = height / stacks,
		lowerbound = -height / 2;
	for (int i = 1; i <= slices; i++)
	{
		//Get alpha angle based on i
		alpha = 2 * M_PI * i / slices;
		//Get beta for next slice
		beta = 2 * M_PI / slices;
		//Lower base Tri is facing down
		glBegin(GL_TRIANGLES);
		glVertex3f(0, lowerbound, 0);
		glVertex3f(radius*cos(alpha + beta), lowerbound, radius*sin(alpha + beta));
		glVertex3f(radius*cos(alpha), lowerbound, radius*sin(alpha));
		glEnd();
		//Upper base Tri is facing up
		glBegin(GL_TRIANGLES);
		glVertex3f(0, -lowerbound, 0);
		glVertex3f(radius*cos(alpha), -lowerbound, radius*sin(alpha));
		glVertex3f(radius*cos(alpha + beta), -lowerbound, radius*sin(alpha + beta));
		glEnd();
		for (int j = 1; j <= stacks; j++)
		{

			//Lateral left bottom to left upper
			glBegin(GL_TRIANGLES);
			glVertex3f(radius*cos(alpha), (j - 1)*stackheight + lowerbound, radius*sin(alpha));
			glVertex3f(radius*cos(alpha + beta), j*stackheight + lowerbound, radius*sin(alpha + beta));
			glVertex3f(radius*cos(alpha), j*stackheight + lowerbound, radius*sin(alpha));
			glEnd();
			//Lateral right upper to right bottom
			glBegin(GL_TRIANGLES);
			glVertex3f(radius*cos(alpha + beta), j*stackheight + lowerbound, radius*sin(alpha + beta));
			glVertex3f(radius*cos(alpha), (j - 1)*stackheight + lowerbound, radius*sin(alpha));
			glVertex3f(radius*cos(alpha + beta), (j - 1)*stackheight + lowerbound, radius*sin(alpha + beta));
			glEnd();
		}
	}
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(P_X, P_Y, P_Z,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put the geometric transformations here
	//Translate last otherwise you will rotate off center
	glTranslatef(transf_vals.t_x, transf_vals.t_y, transf_vals.t_z);
	glRotatef(transf_vals.r_x, 1, 0, 0);
	glRotatef(transf_vals.r_y, 0, 1, 0);

	drawCylinder();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void processKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		camera_vals.postAlphaDecrease();
		glutPostRedisplay();
		break;
	case 'd':
		camera_vals.postAlphaIncrease();
		glutPostRedisplay();
		break;
	case 's':
		camera_vals.postBetaDecrease();
		glutPostRedisplay();
		break;
	case 'w':
		camera_vals.postBetaIncrease();
		glutPostRedisplay();
		break;
	case '+':
		cilinder_vals.stacks++;
		glutPostRedisplay();
		break;
	case '-':
		cilinder_vals.stacks--;
		glutPostRedisplay();
		break;
	case 'l':
		cilinder_vals.slices--;
		glutPostRedisplay();
		break;
	case 'u':
		cilinder_vals.slices++;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void processSpecialKeys(int key_code, int x, int y) {
	switch (key_code)
	{
	case GLUT_KEY_DOWN:
		transf_vals.t_y -= 0.1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		transf_vals.t_y += 0.1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		transf_vals.t_x -= 0.1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		transf_vals.t_x += 0.1;
		glutPostRedisplay();
		break;
	default:break;
	}

}

// write function to process menu events
void mouse_primary_handler(int button, int state, int x, int y)
{
	static int mode;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		mode = (mode + 1) % 3;
	switch (mode)
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glutPostRedisplay();
		break;
	default:break;
	}
}

void mouse_mov_handler(int x, int y)
{
	/*
	p_z = 1 - x ^ 2 - y ^ 2;
	if p_z
	*/
}



int main(int argc, char **argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard and menu callbacks
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(mouse_primary_handler);
	glutPassiveMotionFunc(mouse_mov_handler);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}


//SceneTree Methods
vector<Component *>* SceneTree::LoadXML()
{
	//Implement Loading every component from XML to a component vector. Pain in the ass
	return nullptr;
}

//
SceneTree::SceneTree()
{
	elements=LoadXML();
}

SceneTree::~SceneTree()
{
	if (elements) 
	{
		for each (Component* var in *elements)
		{
			delete var;
		}
	}
}

//Render every component through Tree
void SceneTree::renderTree()
{
	if (elements) 
	{
		for each (Component* var in *elements)
		{
			if (!var->bIsGroupingComponent) 
			{
				ModelComponent *mc = (ModelComponent *)var; 
				mc->renderModel();
			}
		}
	}
}



//ModelComponent Methods
//Constructor for models takes file reads a bunch of vertices, component is not grouping
ModelComponent::ModelComponent(const char* model) : Component(false)
{
	this->model = model;
	//open file and populate vertices
}

//Destructor for models
ModelComponent::~ModelComponent()
{
	delete vertices;
}

/*
	Draw actual model from vector array
*/
void ModelComponent::renderModel()
{
}
