/*
*  simple.c
*  This program draws a white rectangle on a black background.
*/


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/
#endif
#include<math.h>
#include<time.h>

float rotation = 0.0;
float offset = 0.0;
int dir = 1;
float rotspeed = .0001f;
float talkspeed = .005f;
int t = 0;

void DrawSquares();
void DrawWeirdShapes(bool t);


void display(void)
{
	/* clear window */

	glClear(GL_COLOR_BUFFER_BIT);

	DrawSquares();

	t++;
	if (t >= CLOCKS_PER_SEC*5 || t <= 0)
	{
		if (t >= CLOCKS_PER_SEC)
			t = -CLOCKS_PER_SEC*1.5;
		DrawWeirdShapes(true);
	}
	else
		DrawWeirdShapes(false);

	

	offset += 0.01*dir*talkspeed;

	if (fabs(offset) > 0.05)
		dir = dir*-1;

	glRotatef(45.0*rotspeed, 0.0, 0.0, 1.0);

	/* flush GL buffers */

	glFlush();

}

void DrawWeirdShapes(bool blink)
{
	//Draw the weird shape to the right
	glBegin(GL_POLYGON);
	if(blink)
		glColor3f(0.0, 0.0, 0.0);
	else
		glColor3f(0.0, 0.5, 1.0);
	
	glVertex2f(0.38, 0.0);
	glVertex2f(0.53, 0.15);
	glVertex2f(0.68, 0.0);
	glVertex2f(0.53, -0.15);
	glEnd();

	//Draw the weird shape on bottom
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-0.3, -0.5);
	glVertex2f(-0.15, -0.4);
	glVertex2f(0.15, -0.4);
	glVertex2f(0.3, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-0.3, -0.65 + offset);
	glVertex2f(0.0, -0.85 + offset);
	glVertex2f(0.3, -0.65 + offset);
	glEnd();
}

void DrawSquares()
{
	glColor3f(1.0, 1.0, 1.0);
	//Draw the central square
	glBegin(GL_POLYGON);
	glVertex2f(-0.1, -0.1);
	glVertex2f(-0.1, 0.1);
	glVertex2f(0.1, 0.1);
	glVertex2f(0.1, -0.1);
	glEnd();

	//Draw the rectangles in the corners
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.1f, 0.01f);
	glVertex2f(-1.5, -0.2);
	glVertex2f(-1.0, 2.5);
	glVertex2f(1.5, 0.5);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.5, -1.5);
	glVertex2f(-1.5, -0.5);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, -1.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.5, -1.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(1.5, -0.5);
	glVertex2f(1.5, -1.5);
	glEnd();
}

void init()
{

	/* set clear color to black */

	/* 	glClearColor (0.0, 0.0, 0.0, 0.0); */
	/* set fill  color to white */

	/* 	glColor3f(1.0, 1.0, 1.0); */
	//glColor3f(1.0, 1.0, 1.0);

	/* set up standard orthogonal view with clipping */
	/* box as cube of side 2 centered at origin */
	/* This is default view and these statement could be removed */

	/* glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);  */
}

int main(int argc, char** argv)
{

	/* Initialize mode and open a window in upper left corner of screen */
	/* Window title is name of program (arg[0]) */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Stuff");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	init();
	glutMainLoop();

}