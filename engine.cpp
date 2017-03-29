#include "engine.h"


//Position of the camera
#define P_X camera_vals.radius*cos(camera_vals.beta)*sin(camera_vals.alpha)
#define P_Y camera_vals.radius*sin(camera_vals.beta)
#define P_Z camera_vals.radius*cos(camera_vals.beta)*cos(camera_vals.alpha)

//#define IMMEDIATE_MODE


static Camera camera_vals;
static SceneTree *scene;
static unordered_map<string, ModelComponent*> *modelmap;
static GLuint* buffers;

/*
	Assigns buffers to every model component in model map.

	Number of buffers is calculated and alocated after building the 
	SceneTree from modelmap size and before calling this function
	to assign the buffers to the models.
*/
void assignBuffers() 
{
	int i=0;
	for each(const auto var in (*modelmap)) 
	{
		var.second->assignBuffer(i++);
	}
}

/*
	Pushes identified models into elements vector if already in modelmap
	otherwise pushes a new model into the modelmap.
	
	Function has side-effects. Works over the XMLElement pointer.

	Will exit pointing at last model tag and print an error in case
	there's a problem with duplicate keys or unrecognized tags.

*/
void processModelsIntoVector(vector<Component*> &elements,XMLElement *current) 
{
	//ignora modelos vazios
	if (!current->NoChildren())
	{
		current = current->FirstChildElement("model");
		while (current && !strcmp(current->Value(), "model"))
		{
			string path;
			path.assign(current->Attribute("file"));
			if (modelmap->count(path))
			{
				//Found a model tag: try and get the file path into a ModelComponent and push it into the vector
				elements.push_back((Component *)(*modelmap)[path]);
			}
			else
			{
				//slightly unsafe, no guarantee key is unique
				//might not insert, and memory leak ensue
				//if so, error report.
				auto inserted_pair = modelmap->insert(make_pair(path, new ModelComponent(path)));
				if (!inserted_pair.second)
				{
					cerr << "Duplicate key in model map" << endl;
				}
				else
				{
					elements.push_back((Component *)inserted_pair.first->second);
				}
			}
			current = current->NextSiblingElement();
		}
	}
}


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
		modelmap = new unordered_map<string, ModelComponent*>(10);
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

		glewInit();
		glEnableClientState(GL_VERTEX_ARRAY);

		//initialize enough buffers for models in modelmap
		buffers = new GLuint[modelmap->size()];
		glGenBuffers(modelmap->size(), buffers);

		//assign said buffers to every model in modelmap
		assignBuffers();

		// enter GLUT's main cycle
		glutMainLoop();
	}
	else 
	{
		cerr << "Invalid number of arguments";
	}
	return 1;
}







//Load from XML file and construct SceneTree
//It also initializes the vector to 10 of capacity(groups are usually tiny)
//TODO: Turns out next sibling does not go back to parent.
//	As such need to keep the current as a copy, not a reference to be used.
SceneTree::SceneTree(const char *file)
{
	elements.reserve(10);
	bool bFoundModels = false;
	XMLDocument x;
	if (x.LoadFile(file) == XML_SUCCESS)
	{
		XMLElement *current = x.FirstChildElement("scene");
		//There must be a scene tag.
		if (current) 
		{
			current = current->FirstChildElement();
			for(;current;current= current->NextSiblingElement())
			{
				if (!strcmp(current->Value(), "group"))
				{
					//on new group component recursively travel the group? or explicit stack based iteration?
					//as is recursive
					elements.push_back((Component *)new GroupComponent(current));
				}
				//Found a models tag, everything after should be model tags.
				else if (!bFoundModels && !strcmp(current->Value(), "models"))
				{
					processModelsIntoVector(elements, current);
					//Only one models tag processed per group/scene.
					bFoundModels = true;
				}
			}
		}
	}
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
		var->renderComponent();
	}
}






//Constructor for models takes file reads a bunch of vertices, ModelComponent is not grouping
ModelComponent::ModelComponent(const char* model) : Component(false), model(model)
{
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
	/* debug
	for (int i = 0; i < v_size; i++) 
	{
		cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << "\r\n";
	}
	all the vertices where loaded successfully
	*/
}



//Constructor for models takes file path in string form, comes from stack so has to be moved to avoid unnecessary second copy.
//Reads a bunch of vertices, ModelComponent is not grouping
ModelComponent::ModelComponent(string model) : Component(false), model(move(model))
{
	//open file and populate vertices
	ifstream fp;
	fp.open(this->model);
	string input;
	getline(fp, input);
	v_size = stoi(input);
	vertices = new Vector3D[v_size];
	//really unsafe code
	for (int i = 0; i<v_size; i++)
	{
		fp >> vertices[i].x >> vertices[i].y >> vertices[i].z;
	}
	/* debug
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
	Draw actual model from vector array.
	if IMMEDIATE_MODE is defined draws in immediate mode.
*/
void ModelComponent::renderComponent()
{

#ifdef IMMEDIATE_MODE
	//Really unsafe code yet again
	for (int i = 0; i < v_size;)
	{
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++, i++) 
		{
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
	}
#else
	glBindBuffer(GL_ARRAY_BUFFER, buffers[bound_buffer_index]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, v_size);
#endif
}


/*
	Assigns a buffer to this ModelComponent.
	Keep the index to later bind during the render pass.
*/
void ModelComponent::assignBuffer(int index)
{
	bound_buffer_index = index;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[index]);
	glBufferData(GL_ARRAY_BUFFER, v_size * sizeof(Vector3D),vertices, GL_STATIC_DRAW);
}






/*
	This constructor takes the XMLElement of the group tag.
	
	This constructor produces side-effects.
	
	When it exits current should be pointing at the last element of the group,
	having processed said element.

	This last element of the group might be the last element in the XML, if so
	the caller will have to deal with it. If that is the case it will recurse
	out into the scene and exit since on the last elements pointer never moves
	forward.
	
	It also initializes the vector to 10 of capacity(groups are usually tiny)
	
	Right now is recursive -> probly simpler

	TODO: Turns out next sibling does not go back to parent.
	As such need to keep the current as a copy, not a reference to be used.

*/
GroupComponent::GroupComponent(XMLElement *current) : Component(true), order_vector {ID,ID,ID}
{
	elements.reserve(10);
	//ignore empty groups
	if (!current->NoChildren()) 
	{
		int count=0;
		//Only process one translate, one rotate and one models each group
		bool bFoundModels = false;
		bool bFoundTranslate = false;
		bool bFoundScale = false;
		bool bFoundRotate = false;
		//current comes in at the group tag.
		current = current->FirstChildElement();

		for(;current;current = current->NextSiblingElement())
		{
			if (!bFoundTranslate && !strcmp(current->Value(), "translate"))
			{
				translate.x = current->FloatAttribute("X", 0.0f);
				translate.y = current->FloatAttribute("Y", 0.0f);
				translate.z = current->FloatAttribute("Z", 0.0f);
				bFoundTranslate = true;
				order_vector[count++] = TR;
			}
			else if (!bFoundRotate && !strcmp(current->Value(), "rotate"))
			{
				rotate.x = current->FloatAttribute("axisX", 0.0f);
				rotate.y = current->FloatAttribute("axisY", 0.0f);
				rotate.z = current->FloatAttribute("axisZ", 0.0f);
				rotate_angle = current->FloatAttribute("angle", 0.0f);
				bFoundRotate = true;
				order_vector[count++] = RT;
			}
			else if (!bFoundScale && !strcmp(current->Value(), "scale"))
			{
				scale.x = current->FloatAttribute("X", 0.0f);
				scale.y = current->FloatAttribute("Y", 0.0f);
				scale.z = current->FloatAttribute("Z", 0.0f);
				bFoundScale = true;
				order_vector[count++] = SC;
			}
			else if (!bFoundModels && !strcmp(current->Value(), "models"))
			{
				processModelsIntoVector(elements, current);
				//Only one models tag processed per group/scene.
				bFoundModels = true;
			}
			else if (!strcmp(current->Value(), "group"))
			{
				//on new group component recursively travel the group? or explicit stack based iteration?
				//as is recursive
				elements.push_back((Component *)new GroupComponent(current));
			}
		}
	}
}


GroupComponent::~GroupComponent()
{
	for each(Component *var in elements) 
	{
		delete var;
	}
}


/*
* This method involves following the order of operations present in the original XML.
* An ID operation means do nothing. If there is nothing to do for a given position
* than there is nothing to do in the following, otherwise there would have been one
* of the other three operations to do (either Translate, Rotate and Scale).
*/
void GroupComponent::renderComponent()
{
	glPushMatrix();
	for (int i=0; order_vector[i] != ID && i < 3; i++) 
	{
		switch (order_vector[i]) 
		{
			case TR: glTranslatef(translate.x, translate.y, translate.z);
					 break;
			case RT: glRotatef(rotate_angle, rotate.x, rotate.y, rotate.z);
					 break;
			case SC: glScalef(scale.x, scale.y, scale.z);
					 break;
			default: break;
		}
	}
	for each (Component *var in elements)
	{
		var->renderComponent();
	}
	glPopMatrix();
}
