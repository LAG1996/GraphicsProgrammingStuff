/*
Author: Luis Angel Garcia
Description: Displays two types of interactivity with users in openGL using the GLUT
interface. Five shapes can be displayed when the user "triggers" them.
Keyboard controls: Q,W,E,A,S
Mouse controls: Right click to open menu
*/
#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<cmath>
#include<fstream>
#include<windows.h>
using namespace std;

const int AMT_SHAPES = 5;
enum SHAPES { TRAPEZOID, TRIANGLE, SQUARE, HEXAGON, OCTAGON};
int shape = -1;
bool ACTIVE_SHAPES[AMT_SHAPES];
int AMT_TIMES_CLICKED[AMT_SHAPES];
float ** RGB;

void getKey(unsigned char, int, int);
void top_menu(int);
void color_menu(int);
void DrawShape(int);
void drawTrapezoid(int);
void drawSquare(int);
void drawTriangle(int);
void drawHexagon(int);
void drawOctagon(int);

//Change color of shape based on the amount of times it was clicked
void ChangeColor(int shape)
{
	switch (AMT_TIMES_CLICKED[shape])
	{
	case 1: RGB[shape][0] = 1.0; RGB[shape][1] = 0.0; RGB[shape][2] = 0.0; break;
	case 2: RGB[shape][0] = 1.0; RGB[shape][1] = 1.0; RGB[shape][2] = 0.0; break;
	case 3: RGB[shape][0] = 0.0; RGB[shape][1] = 1.0; RGB[shape][2] = 0.0; break;
	case 4: RGB[shape][0] = 0.0; RGB[shape][1] = 0.0; RGB[shape][2] = 1.0; 
		AMT_TIMES_CLICKED[shape] = 0; break;
	}
}

/*Changes colors of shapes based on key input*/
void getKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case'Q': shape = 0; break;

	case 'w':
	case 'W': shape = 1; break;

	case 'e':
	case 'E': shape = 2; break;

	case 'a':
	case 'A': shape = 3; break;

	case 's':
	case 'S': shape = 4; break;

	case 32: shape = -1; break;
	}

	if (shape != -1)
	{
		ACTIVE_SHAPES[shape] = true;

		AMT_TIMES_CLICKED[shape]++;

		//Assign color based on the amount of times the shape was clicked
		ChangeColor(shape);

		//Save shape data as follows:
		//the index of the shape, the red value, the green value, the blue value
		ofstream outputFile("shapes.txt");
		for (int i = 0; i < AMT_SHAPES; i++)
		{

			if (ACTIVE_SHAPES[i])
			{
				outputFile << i << "\n";
				outputFile << RGB[i][0] << "\n";
				outputFile << RGB[i][1] << "\n";
				outputFile << RGB[i][2] << "\n";
			}
		}

		outputFile.close();
	}
	else
	{
		//Destroy shape if user pressed space
		for (int i = 0; i < AMT_SHAPES; i++)
		{
			ACTIVE_SHAPES[i] = false;
			RGB[i][0] = 1.0;
			RGB[i][1] = 1.0;
			RGB[i][2] = 1.0;
			AMT_TIMES_CLICKED[i] = 0;
			remove("shapes.txt");
		}
	}

	
}


/*Changes colors of shapes based on menu input*/
void color_menu(int choice)
{
	//Figure out which shape's submenu the user entered.
	if (choice >= 50)
		shape = 4;
	else if (choice >= 40)
		shape = 3;
	else if (choice >= 30)
		shape = 2;
	else if (choice >= 20)
		shape = 1;
	else if (choice >= 10)
		shape = 0;
	else
		shape = -1;

		//Change color of shape based on menu option
		switch (choice%10)
		{
		case 0: RGB[shape][0] = 1.0; RGB[shape][1] = 0.0; RGB[shape][2] = 0.0; break;
		case 1: RGB[shape][0] = 1.0; RGB[shape][1] = 1.0; RGB[shape][2] = 0.0; break;
		case 2: RGB[shape][0] = 0.0; RGB[shape][1] = 1.0; RGB[shape][2] = 0.0; break;
		case 3: RGB[shape][0] = 0.0; RGB[shape][1] = 0.0; RGB[shape][2] = 1.0; break;
		}

		if (shape > -1)
		{
			ACTIVE_SHAPES[shape] = true;

			//Save shape data as follows:
			//the index of the shape, the red value, the green value, the blue value
			ofstream outputFile("shapes.txt");
			for (int i = 0; i < AMT_SHAPES; i++)
			{
				if (ACTIVE_SHAPES[i])
				{
					outputFile << i << "\n";
					outputFile << RGB[i][0] << "\n";
					outputFile << RGB[i][1] << "\n";
					outputFile << RGB[i][2] << "\n";
				}
			}
			outputFile.close();
		}
		else
		{
			//Erase all shapes and reset their colors to white
			for (int i = 0; i < AMT_SHAPES; i++)
			{
				ACTIVE_SHAPES[i] = false;
				RGB[i][0] = 1.0;
				RGB[i][1] = 1.0;
				RGB[i][2] = 1.0;
				AMT_TIMES_CLICKED[i] = 0;
				remove("shapes.txt");
			}
		}
			
		glutPostRedisplay();
}

void display()
{
	/* clear window */

	glClear(GL_COLOR_BUFFER_BIT);

	//Draw active shapes
	for (int i = 0; i < AMT_SHAPES; i++)
	{
		if (ACTIVE_SHAPES[i])
			DrawShape(i);
	}
	
	/* flush GL buffers */
	glFlush();
}

//Pick shape to draw based on index
void DrawShape(int index)
{
	switch (index)
	{
	case TRAPEZOID: drawTrapezoid(index); break;
	case SQUARE: drawSquare(index); break;
	case TRIANGLE: drawTriangle(index);  break;
	case HEXAGON: drawHexagon(index);  break;
	case OCTAGON: drawOctagon(index);  break;
	}
}

void drawTrapezoid(int index)
{
	glBegin(GL_POLYGON);
	glColor3f(RGB[index][0], RGB[index][1], RGB[index][2]);
	glVertex2f(-1.0, 0.5);
	glVertex2f(-0.7, 0.8);
	glVertex2f(-0.4, 0.8);
	glVertex2f(-0.1, 0.5);
	glEnd();
}

void drawTriangle(int index)
{
	glBegin(GL_POLYGON);
	glColor3f(RGB[index][0], RGB[index][1], RGB[index][2]);
	glVertex2f(0.2, 0.5);
	glVertex2f(0.4, 0.5 + sqrt((.4*.4)-(0.2*0.2)));
	glVertex2f(0.6, 0.5);
	glEnd();
}

void drawSquare(int index)
{
	glBegin(GL_POLYGON);
	glColor3f(RGB[index][0], RGB[index][1], RGB[index][2]);
	glVertex2f(0.2, -0.2);
	glVertex2f(0.2, 0.15);
	glVertex2f(-0.2, 0.15);
	glVertex2f(-0.2, -0.2);
	glEnd();
}

void drawHexagon(int index)
{
	int s = 3;
	float begin_x = -0.9;
	float begin_y = -0.5;
	glBegin(GL_POLYGON);
	glColor3f(RGB[index][0], RGB[index][1], RGB[index][2]);
	glVertex2f(begin_x, begin_y);
	glVertex2f(begin_x + (0.07*s), begin_y + (0.07 * s));
	glVertex2f(begin_x + (0.07*s) + .1 * s, begin_y + (0.07 * s));
	glVertex2f(begin_x + (0.07*s)*2 + .1 *s, begin_y);
	glVertex2f(begin_x + (0.07*s) + .1*s, begin_y - (0.07 * s));
	glVertex2f(begin_x + (0.07*s), begin_y - (0.07 * s));
	glEnd();
}

void drawOctagon(int index)
{
	int s = 2;
	float begin_x = 0.2;
	float begin_y = -0.6;

	glBegin(GL_POLYGON);
	glColor3f(RGB[index][0], RGB[index][1], RGB[index][2]);
	glVertex2f(begin_x, begin_y);
	glVertex2f(begin_x, begin_y + (0.1 * s));
	glVertex2f(begin_x + (0.07*s), begin_y + (0.07 * s) + (0.1 * s));
	glVertex2f(begin_x + (0.07*s) + .1 * s, begin_y + (0.07 * s) + (0.1 * s));
	glVertex2f(begin_x + (0.07*s) * 2 + .1 *s, begin_y + (0.1 * s));
	glVertex2f(begin_x + (0.07*s) * 2 + .1 *s, begin_y);
	glVertex2f(begin_x + (0.07*s) + .1*s, begin_y - (0.07 * s));
	glVertex2f(begin_x + (0.07*s), begin_y - (0.07 * s));
	glEnd();
}


void init()
{
	RGB = new float*[AMT_SHAPES];

	

		for (int i = 0; i < AMT_SHAPES; i++)
		{
			ACTIVE_SHAPES[i] = false;
			RGB[i] = new float[3];
			RGB[i][0] = 1.0;
			RGB[i][1] = 1.0;
			RGB[i][2] = 1.0;
			AMT_TIMES_CLICKED[i] = 0;
		}
		//Open "shapes.txt"
		ifstream inputFile("shapes.txt");
	//if "shapes.txt", simply read data.
	//Data is split into blocks of 4: the shape index, its red value, its green value,
		//then its blue value
	if(inputFile.is_open())
	{
		while (!inputFile.eof())
		{
			int i;
			inputFile >> i;
			ACTIVE_SHAPES[i] = true;
			inputFile >> RGB[i][0];
			inputFile >> RGB[i][1];
			inputFile >> RGB[i][2];
		}
		
	}
	inputFile.close();

	/* set clear color to black */

	/* 	glClearColor (0.0, 0.0, 0.0, 0.0); */
	/* set fill  color to white */

	/* 	glColor3f(1.0, 1.0, 1.0); */

	/* set up standard orthogonal view with clipping */
	/* box as cube of side 2 centered at origin */
	/* This is default view and these statement could be removed */

	/* glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);  */
}

int main(int argc, char** argv)
{
	//glutInit(&argc, argv);
	glutInit(&argc, argv);
	init();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Homework 2");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(getKey);

	int clear = glutCreateMenu(color_menu);
	glutAddMenuEntry("Clear", 1);

	int trap = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 10);
	glutAddMenuEntry("Yellow", 11);
	glutAddMenuEntry("Green", 12);
	glutAddMenuEntry("Blue", 13);

	int tri = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 20);
	glutAddMenuEntry("Yellow", 21);
	glutAddMenuEntry("Green", 22);
	glutAddMenuEntry("Blue", 23);

	int sq = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 30);
	glutAddMenuEntry("Yellow", 31);
	glutAddMenuEntry("Green", 32);
	glutAddMenuEntry("Blue", 33);

	int hex = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 40);
	glutAddMenuEntry("Yellow", 41);
	glutAddMenuEntry("Green", 42);
	glutAddMenuEntry("Blue", 43);

	int oct = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 50);
	glutAddMenuEntry("Yellow", 51);
	glutAddMenuEntry("Green", 52);
	glutAddMenuEntry("Blue", 53);

	glutAddSubMenu("Trapezoid", trap);
	glutAddSubMenu("Triangle", tri);
	glutAddSubMenu("Square", sq);
	glutAddSubMenu("Hexagon", hex);
	glutAddSubMenu("Octogon", oct);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}