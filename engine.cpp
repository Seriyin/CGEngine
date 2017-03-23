#include "engine.h"


//Position of the camera
#define P_X camera_vals.radius*cos(camera_vals.beta)*sin(camera_vals.alpha)
#define P_Y camera_vals.radius*sin(camera_vals.beta)
#define P_Z camera_vals.radius*cos(camera_vals.beta)*cos(camera_vals.alpha)


static Camera camera_vals;
static SceneTree *scene;



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

	scene->renderTree();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void processKeys(unsigned char key, int x, int y) 
{
/*	switch (key) {
	case 'a':
		glutPostRedisplay();
		break;
	case 'd':
		glutPostRedisplay();
		break;
	case 's':
		glutPostRedisplay();
		break;
	case 'w':
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
*/
}

void processSpecialKeys(int key_code, int x, int y) 
{
	switch (key_code)
	{
	case GLUT_KEY_DOWN:
		camera_vals.postBetaDecrease();
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		camera_vals.postBetaIncrease();
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		camera_vals.postAlphaDecrease();
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		camera_vals.postAlphaIncrease();
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
		glEnable(GL_CULL_FACE);
		glutPostRedisplay();
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		glutPostRedisplay();
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glDisable(GL_CULL_FACE);
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



int main(int argc, char **argv) 
{
	if (argc == 2)
	{
		scene = new SceneTree(argv[1]);

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
	}
	else 
	{
		cerr << "Invalid number of arguments";
	}
	return 1;
}


//SceneTree Methods
vector<Component *> SceneTree::LoadXML(const char *file)
{
	XMLDocument x;
	if (x.LoadFile(file) == XML_SUCCESS)
	{
		XMLElement *current=x.FirstChildElement("scene");
		while(current) 
		{
			if (!strcmp(current->Value(),"group")) 
			{
				//on new group component recursively travel the group? or explicit stack based iteration?
				//as is recursive
				elements.push_back((Component *)new GroupComponent(current));
			}
			//Found a model: try and get the file path into a ModelComponent and build it into the vector at the vector's end
			else if (!strcmp(current->Value(), "model")) 
			{
				elements.push_back((Component *)new ModelComponent(current->Attribute("file")));
			}
			//No children continue in the same or upper? hierarchy level
			if (current->NoChildren())
			{
				current = current->NextSiblingElement();
			}
			else
			{
				current = current->FirstChildElement();
			}
		}
	}
	return elements;
}

//
SceneTree::SceneTree(const char *file)
{
	elements=LoadXML(file);
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
		else 
		{
			GroupComponent *gc = (GroupComponent *)var;
			gc->renderGroup();
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
	/*
	for (int i = 0; i < v_size; i++) 
	{
		cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << "\r\n";
	}
	all the vertices where loaded successfully
	*/
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
	//cout << "got to drawing";
	for (int i = 0; i < v_size;)
	{
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++, i++) 
		{
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
	}
	//cout << "got through drawing";
}



//right now recursive is probly simpler
GroupComponent::GroupComponent(XMLElement * current) : Component(true)
{
}

GroupComponent::~GroupComponent()
{
	for each(Component *var in elements) 
	{
		delete var;
	}
}

void GroupComponent::renderGroup()
{
}
