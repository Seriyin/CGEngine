#include "engine.h"
#include <fstream>

//Position of the camera
#define P_X camera_vals.radius*cos(camera_vals.beta)*sin(camera_vals.alpha)
#define P_Y camera_vals.radius*sin(camera_vals.beta)
#define P_Z camera_vals.radius*cos(camera_vals.beta)*cos(camera_vals.alpha)


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
vector<Component *> SceneTree::LoadXML()
{
	//Implement Loading every component from XML to a component vector. Pain in the ass
	return elements;
}

//
SceneTree::SceneTree()
{
	elements=LoadXML();
}

SceneTree::~SceneTree()
{
	for each (Component* var in elements)
	{
		delete var;
	}
}

//Render every component through Tree
void SceneTree::renderTree()
{
	for each (Component* var in elements)
	{
		if (!var->bIsGroupingComponent) 
		{
			ModelComponent *mc = (ModelComponent *)var; 
			mc->renderModel();
		}
	}
}



//ModelComponent Methods
//Constructor for models takes file reads a bunch of vertices, component is not grouping
ModelComponent::ModelComponent(const char* model) : Component(false)
{
	this->model.assign(model);
	//open file and populate vertices
	ifstream fp;
	fp.open(model);
	string input;
	getline(fp, input);
	v_size = stoi(input);
	vertices = new Vector3D[v_size];
	//really unsafe code
	for(int i=0;i<v_size;i++) 
	{
		fp >> vertices[i].x >> vertices[i].y >> vertices[i].z;
	}
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
	//Really unsafe code yet again
	for (int i = 0; i < v_size; i++)
	{
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++, i++) 
		{
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
	}
}
