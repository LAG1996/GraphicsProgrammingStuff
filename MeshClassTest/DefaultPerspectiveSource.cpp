/*
Author: Luis Angel Garcia
Description: A practice in shading and interactive viewing using GLUT API.
*/

#include <GL/glut.h>
#include <iostream>

#include<stdlib.h>
#include<time.h>
#include<math.h>
#include "Material.h"
#include "Light.h"
#include "Prism.h"
#include "Vec4.h"

/*
All the start stuff
*/
Vec4 CameraLoc; //Camera location
Vec4 CenterOfScene; //Focus of the scene
Vec4 cam_forward; //Camera's forward vector
Vec4 cam_right; //Camera's right vector
Vec4 cam_up; //Camera's up vector

R_Prism * RectPrisms; //List of rectangles and their locations (immutable)



Vec4 Move_Vec; //Vector to move along depending on key presses

			   //GLfloat Theta = 0; Useless, but keeping it for learning's sake
GLfloat camera_move_speed = 1.0f; //Speed at which the camera moves around the screen

GLfloat phi = 0.0f;
GLfloat theta = 0.0f;

void timer(int v)
{

	glLoadIdentity(); //Reset the model view for transformation

	/*Do some linear algebra to find the camera's orientation vectors*/
	/*
	cam_forward = CenterOfScene - CameraLoc; //The camera would be facing the center of the scene
	cam_forward = cam_forward.Normalized();

	cam_right = cam_forward * Vec4(0.0f, 1.0f, 0.0f, 1.0f); //Get the right vector that would be orthogonal to the forward vector and the world up vector
	cam_right = cam_right.Normalized();

	cam_up = cam_forward * cam_right; //Get the up vector from the other two vectors we've derived
	cam_up = cam_up.Normalized();*/

	//The punchline:
	gluLookAt(CameraLoc.x, CameraLoc.y, CameraLoc.z, CenterOfScene.x, CenterOfScene.y, CenterOfScene.z, 0, 1, 0);

	Move_Vec = Vec4::Zero(); //Reset the movement vector for user input
	glutPostRedisplay(); //Call display
	glutTimerFunc(1000.0 / 60.0, timer, v); //Set the timer for another 60th of a second
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSolidIcosahedron();

	glFlush();
	glutSwapBuffers();
}


//Set up perspective camera
void reshapeWindow(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	if (width <= height)
	glOrtho(-1.5, 1.5, -1.5*(GLfloat)height / (GLfloat)width,
	1.5*(GLfloat)height / (GLfloat)width, -10.0, 10.0);
	else
	glOrtho(-1.5*(GLfloat)width / (GLfloat)height,
	1.5*(GLfloat)width / (GLfloat)height, -1.5, 1.5, -10.0, 10.0);
	*/

	/*Set up perspective camera according to the height and width*/
	if (width <= height)
		gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 200.0);
	else
		gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);//Switch to the modelview
}

//Initialize lighting, materials, fog, the location of the focal point for the camera, the location of the camera itself and its orientation
void init()
{
	glClearColor(0.3, 0.3, 0.3, 0.0); //Let the background be grey
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	//Set up our camera and scene points.
	CenterOfScene = { 0.0f, 0.0f, 0.0f, 0.0f };
	CameraLoc = { 0.0f, 0.0f, 3.0f, 0.0f };

	Material mat;
	mat.ambient = { 1.0f, 0.3f, 0.3f, 1.0f };
	mat.diffuse = { 1.0f, 0.3f, 0.6f, 1.0f };
	mat.specular = { 1.0f, 1.0f, 1.0f, 0.1f };
	GLfloat s = 50.0f;
	mat.shininess = &s;

	PointLight light;
	light.position = { -1.0f, 1.0f, 1.0f, 1.0f };

	//Set up materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.ambient.ToList());
	glMaterialfv(GL_FRONT, GL_SHININESS, mat.shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular.ToList());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse.ToList());
	glLightfv(GL_LIGHT0, GL_POSITION, light.position.ToList());

	//Set up some linear fog
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, Vec4(0.45f, 0.6f, 0.6f, 1.0f).ToList());
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 100.0f);

	//Enable everything
	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);



	//Set up our camera's orientation with the power of linear algebra and assuming that the camera's up vector is the world's
	//up vector
	cam_up = { 0.0f, 1.0f, 0.0f, 1.0f };

	cam_forward = CenterOfScene - CameraLoc;
	cam_up = cam_forward * cam_up;

	cam_right = cam_up * cam_forward;

	glMatrixMode(GL_MODELVIEW);
	Move_Vec = Vec4::Zero();

	std::cout << "Hello, world" << std::endl;
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Hello, World!");
	
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);
	glutTimerFunc(1000.0 / 60.0f, timer, 0);

	glutMainLoop();
}