#include <GL/glut.h>


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

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Template");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);

	init();
	glutMainLoop();
}