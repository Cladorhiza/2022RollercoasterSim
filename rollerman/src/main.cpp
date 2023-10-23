//OpenGL
#include "glew.h"
#include "wglew.h"
#pragma comment(lib, "glew32.lib")
#include "freeglut.h"
//GLM
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"
#include "gtc\matrix_inverse.hpp"
#include "gtx/rotate_vector.hpp"
//Image
#include "FreeImage.h"
//Misc
#include <iostream>
#include "Shader.h"
#include "Sphere.h"
#include "InputManager.h"
#include "Camera.h"
#include "Line.h"
#include "LineLoader.h"
#include "Carriage.h"
#include <chrono>
#include <string>

//MODEL LOADING
#include "threeDModel.h"
#include "OBJLoader.h"

CShader* myShader;
CShader* myBasicShader;

//Light Properties
float Light_Ambient_And_Diffuse[4] = { 1.f, 1.f, 1.0f, 1.0f };
float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
float LightPos[4] = { -200.0f, 40.0f, 0.0f, 0.0f };

//Metal properties
float m_Amb[4] = { 1.f, 1.0f, 1.0f, 1.0f };
float m_Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float m_Spec[4] = { 1.0f,1.0f, 1.0f, 1.0f };
float m_Shine = 80;

CThreeDModel coaster(m_Amb, m_Diff, m_Spec, m_Shine);
Carriage carriage(m_Amb, m_Diff, m_Spec, m_Shine);

//skyBox properties
float m_Amb2[4] = { 1.f, 1.0f, 1.0f, 1.0f };
float m_Diff2[4] = { 0.f, 0.f, 0.f, 0.f };
float m_Spec2[4] = { 0.f,0.f, 0.f, 0.f };
float m_Shine2 = 1;

CThreeDModel skyBox(m_Amb2, m_Diff2, m_Spec2, m_Shine2);

//Landscape properties
float m_Amb3[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float m_Diff3[4] = { 0.8f, 0.8f, 0.6f, 1.0f };
float m_Spec3[4] = { 0.f,0.f, 0.f, 0.f };
float m_Shine3 = 1;

CThreeDModel landscape(m_Amb3, m_Diff3, m_Spec3, m_Shine3);

std::vector<CThreeDModel*> collidables{&landscape, &skyBox, &coaster};

COBJLoader objLoader;	//this object is used to load the 3d models.
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing

//Rollercoaster navigation
Line l;

//Timer for deltatime
std::chrono::time_point<std::chrono::system_clock> start, end;
std::chrono::duration<double> elapsed;



int screenWidth=600, screenHeight=600;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys(double timeStep);         //called in winmain to process keyboard input
void idle();		//idle function



/*************    START OF OPENGL FUNCTIONS   ****************/
void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(glm::radians(100.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.001f, 3000.f);
}

void init()
{
	glClearColor(0.6, 0.6, 0.8, 0.0);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	
	myShader = new CShader();
	if (!myShader->CreateShaderProgram("BasicView", "res/shaders/basicTransformations.vert", "res/shaders/basicTransformations.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	myBasicShader = new CShader();
	if (!myBasicShader->CreateShaderProgram("Basic", "res/shaders/basic.vert", "res/shaders/basic.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	glUseProgram(myShader->GetProgramObjID());  // use the shader

	glEnable(GL_TEXTURE_2D);

	if (objLoader.LoadModel("res/models/coaster.obj"))//returns true if the model is loaded
	{
		coaster.ConstructModelFromOBJLoader(objLoader);
		coaster.InitVBO(myShader);
	}
	if (objLoader.LoadModel("res/models/skybox.obj"))//returns true if the model is loaded
	{
		skyBox.ConstructModelFromOBJLoader(objLoader);
		skyBox.InitVBO(myShader);
	}
	if (objLoader.LoadModel("res/models/landscape.obj"))//returns true if the model is loaded
	{
		landscape.ConstructModelFromOBJLoader(objLoader);
		landscape.InitVBO(myShader);
	}

	LineLoader::loadLine(l, "res/models/curve.obj");
	carriage.init(objLoader, l.getPoints()[0], myShader, "res/models/carriage.obj");
}

void display()
{
	glm::mat4 viewingMatrix(1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(myShader->GetProgramObjID());  // use the shader

	//Set the projection matrix in the shader
	GLuint projMatLocation = glGetUniformLocation(myShader->GetProgramObjID(), "ProjectionMatrix");
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	
	viewingMatrix = Camera::getViewingMatrix();

	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->GetProgramObjID(), "light_specular"), 1, Light_Specular);
	
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	//Switch to basic shader to draw the lines for the bounding boxes
	glUseProgram(myBasicShader->GetProgramObjID());
	projMatLocation = glGetUniformLocation(myBasicShader->GetProgramObjID(), "ProjectionMatrix");
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	//switch back to the shader for textures and lighting on the objects.
	glUseProgram(myShader->GetProgramObjID());  // use the shader
	ModelViewMatrix = glm::mat4(1.f);

	normalMatrix = glm::inverseTranspose(glm::mat3(viewingMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->GetProgramObjID(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	ModelViewMatrix *= viewingMatrix;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	coaster.DrawElementsUsingVBO(myShader);
	skyBox.DrawElementsUsingVBO(myShader);
	landscape.DrawElementsUsingVBO(myShader);

	//skyBox.DrawOctreeLeaves(myShader);
	//coaster.DrawOctreeLeaves(myShader);
	//landscape.DrawOctreeLeaves(myShader);
	
	ModelViewMatrix = viewingMatrix * carriage.getModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	carriage.render();
	
	
	glFlush();
	glutSwapBuffers();
}

void processKeys(double timeStep)
{
	if (InputManager::getKey_F()) {
		timeStep *= 3;
	}
	carriage.update(l, timeStep);
	Camera::Update(timeStep, carriage.getCarriagePosition(), carriage.getModelMatrix(), collidables);
	
	
}

void idle()
{
	end = std::chrono::system_clock::now();
	elapsed = end - start;
	start = std::chrono::system_clock::now();
	processKeys(elapsed.count());
	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("RollerCoaster");

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << " GLEW ERROR" << std::endl;
	}
	
	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	std::cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << std::endl;


	//initialise the objects for rendering
	init();

	glutReshapeFunc(reshape);
	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(InputManager::special);
	glutSpecialUpFunc(InputManager::specialUp);
	glutKeyboardFunc(InputManager::keyboardInput);
	glutKeyboardUpFunc(InputManager::keyboardUpInput);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	start = std::chrono::system_clock::now();
	glutMainLoop();

	return 0;
}