/*
Following the example from Edward Angel and Dave Schreiner's Sierpinski Gasket program in Appendix A.2 in the textbook
"Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL",
creates a Sierpinski gasket from a set of 5000 random points.
*/
#include <GL/glut.h>
#include<stdlib.h> //Contains random number generator function
#include "vec.h"

typedef vec2 point2; //Defines a point2 type identical to a vec2

const int NUM_POINTS = 5000;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(0, 0);
	glVertex2f(0, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glEnd();

	glFlush();
}

// WINDOW RESIZE -------------------------------------------------------------
void reshapeWindow(int width, int height)
{
	//Set the aspect ratio for orthogonal view
	float aspectRatio = ((float)width / height);
	float xSpan = 1; //The scaling on the x-axis
	float ySpan = 1; //The scaling on the y-axis

	glMatrixMode(GL_PROJECTION); //Switch to the projection matrix to mess with the camera
	glLoadIdentity(); //Load in the identity to avoid artefacts
	


	if (width > height)
	{
		xSpan *= aspectRatio;
	}
	else
		ySpan *= aspectRatio;

	gluOrtho2D(0, xSpan, 0, ySpan); //Change the aspect ratio of the orthographic camera accordingly
	glViewport(0, 0, width / 2, height / 2); //Have the viewport render for the whole window

	glMatrixMode(GL_MODELVIEW);//Switch to the modelview
	glLoadIdentity();

	glutPostRedisplay();
}

// GENERAL INITIALIZATION STUFF --------------------------------------
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	point2 points[NUM_POINTS]; //Generate an array that will contain NUM_POINTS amount of points

	//A triangle on the plane has a z-axis value of 0, so we define it using point2

	//Create a triangle
	point2 vertices[3] = { point2(-1.0, -1.0), point2(0.0, -1.0), point2(1.0, -1.0) };

	points[0] = point2(0.25, 0.5); //Choose an arbitrary point inside the triangle we just defined.

	for (int k = 1; k < NUM_POINTS; k++)
	{
		int j = rand() % 3; //Pick a vertex from the triangle at random

		//Compute the point halfway between the selected vertex and the previous point we constructed
		points[k] = (points[k - 1] + vertices[j]) / 2.0;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sierpinski Triangle");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);

	init();
	glutMainLoop();
}