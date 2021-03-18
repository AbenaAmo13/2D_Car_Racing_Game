#include <iostream>
using namespace std;

//--- OpenGL ---
#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "shaders\Shader.h"
#include "GL\freeglut.h"
#include "FreeImage.h"

#include "Sprite.h"
#include <ctime>
#include <iostream>


/*Variables for timing*/
float fDeltaTime = 0.0f;
float currentTicks = 0.0f;
float PreviousTicks = 0.0f;
float Angle = 0.0f;
/************************/

using namespace std;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 480, screenHeight = 480;

//booleans to handle when the arrow keys are pressed or released.
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
#define PI 3.1415926535897932384626433832795
double orthoYmax = 25;
double orthoYmin = -25;
double orthoXmin = -25;
double oorthoXmax = 25;


Shader shader;
Sprite mySquare;
Sprite myTrack;


//OPENGL FUNCTION PROTOTYPES
void display();				//used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
void init();				//called in main when the program starts.

/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	glViewport(0, 0, width, height);						// set Viewport dimensions

	ProjectionMatrix = glm::ortho(-25.0, 25.0, -25.0, 25.0); 
}


void display()									
{

	//obtain the ticks from the clock and find difference with previous time.
	currentTicks = std::clock();
	float deltaTicks = (float)(currentTicks - PreviousTicks);
	PreviousTicks = currentTicks;

	//now find the actual time passed between frames
	fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	myTrack.Render(shader, ViewMatrix, ProjectionMatrix);
	
	glEnable(GL_BLEND);
	glm::mat4 ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 0.0, 0.0));
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(mySquare.GetXPos(), mySquare.GetYPos(), 0.0));

	if (Up == true) {
		orthoYmin += 0.011;
		orthoYmax += 0.011;
		ProjectionMatrix = glm::ortho(-25.0, 25.0, orthoYmin, orthoYmax);

	}

	if (Down == true) {
		orthoYmin -= 0.011;
		orthoYmax -= 0.011;
		ProjectionMatrix = glm::ortho(-25.0, 25.0, orthoYmin, orthoYmax);
		float roteAngle = 1.57*2;
		ModelViewMatrix = glm::rotate(ModelViewMatrix, roteAngle, glm::vec3(0.0, 0.0, 1.0));


	}


	if (Left) {
		float roteAngle = 1.57;
		ModelViewMatrix = glm::rotate(ModelViewMatrix,roteAngle,glm::vec3(0.0, 0.0, 1.0));
	}


	if (Right) {
		float roteAngle = -1.57;
		ModelViewMatrix = glm::rotate(ModelViewMatrix, roteAngle, glm::vec3(0.0, 0.0, 1.0));
	}


	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	double trackPosition = 0;
	//Whole track at the beggining:
	for (int i = 0; i < 20; i++) {
		glm::mat4 ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, trackPosition, 0.0));
		myTrack.Render(shader, ModelViewMatrix, ProjectionMatrix);
		trackPosition += 5;


	}


	mySquare.Render(shader, ModelViewMatrix, ProjectionMatrix);
	glDisable(GL_BLEND);


	//ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	
	glutSwapBuffers();

	Angle += 0.0005f;
	if (Angle >= 360)
		Angle = 0;

}

void init()
{
	FreeImage_Initialise();

	glClearColor(0.0,0.0,1.0,0.0);						//sets the clear colour to black
	 
	//Load the GLSL program 
	if (!shader.load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	///This part commented is to scale the width of the sprite to match the dimensions of the car.png image.
	mySquare.SetWidth(5.0f);
	mySquare.SetHeight(10.0f);

	myTrack.SetWidth(10.0f);
	myTrack.SetHeight(10.0f);
	
	float red[3] = { 1,0,0 };

	mySquare.Init(shader, red, "textures/car_yellow_5.png");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myTrack.Init(shader, red, "textures/roadTile27.png");
	

	
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = true;
		break;
	case GLUT_KEY_RIGHT:
		Right = true;
		break;
	case GLUT_KEY_UP:
		Up = true;
		break;
	case GLUT_KEY_DOWN:
		Down = true;
		break;
	}
}

void specialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = false;
		break;
	case GLUT_KEY_RIGHT:
		Right = false;
		break;
	case GLUT_KEY_UP:
		Up = false;
		break;
	case GLUT_KEY_DOWN:
		Down = false;
		break;
	}
}

void processKeys()
{
	if (Left)
	{
		mySquare.IncPos(-10.0f * fDeltaTime, 0.0f);
	}
	if (Right)
	{
		mySquare.IncPos(10.0f * fDeltaTime, 0.0f);
	}
	if (Up)
	{
		mySquare.IncPos(0.0f, 10.0f * fDeltaTime);
	}
	if (Down)
	{
		mySquare.IncPos(0.0f, -10.0f * fDeltaTime);
	}
}

void idle()
{
	processKeys();

	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

// FREEGLUT WINDOW SET UP
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Image loading");

	glutReshapeFunc(reshape);

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

