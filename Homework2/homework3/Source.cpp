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
#include<stack>
#include<queue>
using namespace std;

struct ShapeInfo
{
	int shapeID;
	float R;
	float G;
	float B;
	float first_x;
	float first_y;
	float left_x;
	float right_x;
	float top_y;
	float bottom_y;
	float y_scale;
	float x_scale;
};

const int AMT_SHAPES = 5;
enum SHAPES { TRAPEZOID, TRIANGLE, SQUARE, HEXAGON, OCTAGON};

//An array that holds shape info
queue<ShapeInfo> ShapesToDraw;
stack<ShapeInfo> ShapesDrawn;

ShapeInfo NewShape;

int amt_shape;

float ** RGB;
int CONTROL_STATE = 0;
//char bitmap[] = "Hello";

void getKey(unsigned char, int, int);
void DrawShape(ShapeInfo);
void drawTrapezoid(ShapeInfo);
void drawSquare(ShapeInfo);
void drawTriangle(ShapeInfo);
void drawHexagon(ShapeInfo);
void drawOctagon(ShapeInfo);
void output(float, float, char*, void*);
void PrintShapeControls();
void PrintColorControls();
void PrintClickControls();
void CalculateOtherSide(int);
int * ChoiceOfRGB(int);

/*Changes colors of shapes based on key input*/
void getKey(unsigned char key, int x, int y)
{
	int choice;
	switch (key)
	{
	case 'q':
	case'Q': choice = 0; break;

	case 'w':
	case 'W': choice = 1; break;

	case 'e':
	case 'E': choice = 2; break;

	case 'a':
	case 'A': choice = 3; break;

	case 't':
	case 'T': choice = 4; break;

	default: choice = -1;
	}

	//If the "control state" is 0, then the user is picking a shape to draw.
	if (CONTROL_STATE == 0)
	{
		NewShape.shapeID = choice;
		NewShape.x_scale = 1.0f;
		NewShape.y_scale = 1.0f;

		CONTROL_STATE++;
	}
	//If the "control state" is 1, then the user is picking a color to draw the new shape in.
	//Add the RGB values to the NewShape
	else if(CONTROL_STATE == 1)
	{
		int * RGB = ChoiceOfRGB(choice);

		NewShape.R = RGB[0];
		NewShape.G = RGB[1];
		NewShape.B = RGB[2];

		CONTROL_STATE++;
	}
	//If the "control state" is 2 or beyond, keyboards clicks don't matter.
}

int * ChoiceOfRGB(int choice)
{
	int RGB[3];
	switch (choice)
	{
	case 0: RGB[0] = 1.0f; RGB[1] = 0.0f; RGB[2] = 0.0f; break;
	case 1: RGB[0] = 1.0f; RGB[1] = 1.0f; RGB[2] = 0.0f; break;
	case 2: RGB[0] = 0.0f; RGB[1] = 1.0f; RGB[2] = 0.0f; break;
	case 3: RGB[0] = 0.0f; RGB[1] = 0.0f; RGB[2] = 1.0f; break;
	default: RGB[0] = 1.0f; RGB[1] = 1.0f; RGB[2] = 1.0f;
	}

	return RGB;
}

void OnMouseClick(int button, int state, int x, int y)
{
	if (CONTROL_STATE == 2)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			NewShape.first_x = x;
			NewShape.first_y = y;

			//CalculateOtherSide(NewShape.shapeID);

			ShapesToDraw.push(NewShape);

			CONTROL_STATE = 0;
		}
	}

	glutPostRedisplay();
}

//Gets the "other side" of the shape's bounding box based on its id.
void CalculateOtherSide(int shapeId)
{
	switch (shapeId)
	{
	case 0:
		//This is a trapezoid
		NewShape.top_y = NewShape.first_y + .3*NewShape.y_scale;
		NewShape.bottom_y = NewShape.first_y;

		NewShape.left_x = NewShape.first_x;
		NewShape.right_x = NewShape.first_x + .9*NewShape.x_scale;
		break;

	case 1:
		//This is a triangle
		NewShape.top_y = NewShape.first_y + NewShape.first_y + sqrt(((NewShape.first_x + .2*NewShape.x_scale)*(NewShape.first_x + .2*NewShape.x_scale)) - (.2*NewShape.x_scale*.2*NewShape.x_scale))*NewShape.y_scale;
		NewShape.bottom_y = NewShape.first_y;

		NewShape.left_x = NewShape.first_x;
		NewShape.right_x = NewShape.first_x + .4*NewShape.x_scale;
		break;

	case 2:
		//This is a square
		NewShape.top_y = NewShape.first_y + .5*NewShape.y_scale;
		NewShape.bottom_y = NewShape.first_y;

		NewShape.left_x = NewShape.first_x - .25*NewShape.x_scale;
		NewShape.right_x = NewShape.first_x;
		break;

	case 3:
		//This is a hexagon
		NewShape.top_y = NewShape.first_y + 0.07 * NewShape.y_scale;
		NewShape.bottom_y = NewShape.first_y - 0.07 * NewShape.y_scale;

		NewShape.left_x = NewShape.first_x;
		NewShape.right_x = NewShape.first_x + 0.07 * 2 + .1*NewShape.x_scale;
		break;
	
	case 4:
		//This is an octagon
		NewShape.top_y = NewShape.first_y + (0.07*NewShape.y_scale) + (0.1*NewShape.y_scale);
		NewShape.bottom_y = NewShape.first_y - (0.07*NewShape.y_scale);

		NewShape.left_x = NewShape.first_x;
		NewShape.right_x = NewShape.first_x + (0.07*NewShape.x_scale) * 2 + .1 *NewShape.x_scale;
		break;
	}
}

void display()
{
	/* clear window */
	glClear(GL_COLOR_BUFFER_BIT);

	switch (CONTROL_STATE)
	{
	case 0: PrintShapeControls(); break;
	case 1: PrintColorControls(); break;
	case 2: PrintClickControls(); break;
	}
	
	
	while (ShapesToDraw.size() > 0)
	{
		ShapeInfo temp = ShapesToDraw.front();
		DrawShape(temp);
		ShapesToDraw.pop();
		ShapesDrawn.push(temp);
	}

	while (ShapesDrawn.size() > 0)
	{
		ShapeInfo temp = ShapesDrawn.top();
		ShapesDrawn.pop();
		ShapesToDraw.push(temp);
	}
		
	
	/* flush GL buffers */
	glutSwapBuffers();
}

void output(float x, float y, char *message, void * font)
{
	glColor3f(1, 1, 1);
	int length = 0;
	glRasterPos3f(x, y, 0.0);
	length = (int)strlen(message);
	for (int i = 0; i < length; i++)
		glutBitmapCharacter(font, message[i]);
}

void PrintShapeControls()
{
	output(-1, .9, "PICK A SHAPE!", GLUT_BITMAP_TIMES_ROMAN_24);
	output(-1, .8, "Q) TRAPEZOID ", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .7, "W) TRIANGLE ", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .6, "E) SQUARE ", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .5, "R) HEXAGON ", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .4, "T) OCTAGON ", GLUT_BITMAP_TIMES_ROMAN_10);
}

void PrintColorControls()
{
	output(-1, .9, "PICK A COLOR!", GLUT_BITMAP_TIMES_ROMAN_24);
	output(-1, .8, "Q) RED", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .7, "W) YELLOW", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .6, "E) GREEN", GLUT_BITMAP_TIMES_ROMAN_10);
	output(-1, .5, "R) BLUE", GLUT_BITMAP_TIMES_ROMAN_10);
}

void PrintClickControls()
{
	output(-1, .9, "LEFT CLICK TO PICK A SPOT TO DRAW", GLUT_BITMAP_TIMES_ROMAN_24);
}

//Pick shape to draw based on index
void DrawShape(ShapeInfo s)
{
	switch (s.shapeID)
	{
	case TRAPEZOID: drawTrapezoid(s); break;
	case SQUARE: drawSquare(s); break;
	case TRIANGLE: drawTriangle(s);  break;
	case HEXAGON: drawHexagon(s);  break;
	case OCTAGON: drawOctagon(s);  break;
	}
}

void drawTrapezoid(ShapeInfo s)
{
	s.x_scale = .1;
	s.y_scale = .4;

	s.first_x = 0.0f;
	s.first_y = 0.0f;

	glBegin(GL_POLYGON);
	glColor3f(s.R, s.G, s.B);
	glVertex2f(s.first_x, s.first_y);
	glVertex2f(s.first_x + .3*s.x_scale, s.first_y + .3*s.y_scale);
	glVertex2f(s.first_x + .6*s.x_scale, s.first_y + .3*s.y_scale);
	glVertex2f(s.first_x + .9*s.x_scale, s.first_y);
	glEnd();
}

void drawTriangle(ShapeInfo s)
{
	glBegin(GL_POLYGON);
	glColor3f(s.R, s.G, s.B);
	glVertex2f(s.first_x, s.first_y);
	glVertex2f(s.first_x + .2*s.x_scale, s.first_y + sqrt(((s.first_x + .2*s.first_x)*(s.first_x + .2*s.x_scale))-(.2*s.x_scale*.2*s.x_scale))*s.first_y);
	glVertex2f(s.first_x + .4*s.first_x, s.first_y);
	glEnd();
}

void drawSquare(ShapeInfo s)
{
	glBegin(GL_POLYGON);
	glColor3f(s.R, s.G, s.B);
	glVertex2f(s.first_x, s.first_y);
	glVertex2f(s.first_x, s.first_y + .5*s.x_scale);
	glVertex2f(s.first_x - .25*s.x_scale, s.first_y + .5*s.x_scale);
	glVertex2f(s.first_x - .25*s.x_scale, s.first_y);
	glEnd();
}

void drawHexagon(ShapeInfo s)
{
	glBegin(GL_POLYGON);
	glColor3f(s.R, s.G, s.B);
	glVertex2f(s.first_x, s.first_y);
	glVertex2f(s.first_x + (0.07*s.x_scale), s.first_y + (0.07 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale) + .1 * s.x_scale, s.first_y + (0.07 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale)*2 + .1 *s.x_scale, s.first_y);
	glVertex2f(s.first_x + (0.07*s.x_scale) + .1*s.x_scale, s.first_y - (0.07 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale), s.first_y - (0.07 * s.x_scale));
	glEnd();
}

void drawOctagon(ShapeInfo s)
{
	glBegin(GL_POLYGON);
	glColor3f(s.R, s.G, s.B);
	glVertex2f(s.first_x, s.first_y);
	glVertex2f(s.first_x, s.first_y + (0.1 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale), s.first_y + (0.07 * s.x_scale) + (0.1 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale) + .1 * s.x_scale, s.first_y + (0.07 * s.x_scale) + (0.1 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale) * 2 + .1 *s.x_scale, s.first_y + (0.1 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale) * 2 + .1 *s.x_scale, s.first_y);
	glVertex2f(s.first_x + (0.07*s.x_scale) + .1*s.x_scale, s.first_y - (0.07 * s.x_scale));
	glVertex2f(s.first_x + (0.07*s.x_scale), s.first_y - (0.07 * s.x_scale));
	glEnd();
}

void OnWindowReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D(-2.0, 2.0, -2.0*(GLfloat)h, 2.0*(GLfloat)h / (GLfloat)w);
	else
		gluOrtho2D(-2.0*(GLfloat)w / (GLfloat)h, 2.0*(GLfloat)w / (GLfloat)h, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);
}

void init()
{
		/*
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
	inputFile.close();*/

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
	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Homework 2");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(getKey);
	glutReshapeFunc(OnWindowReshape);
	glutMouseFunc(OnMouseClick);

	/*
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

	glutAttachMenu(GLUT_RIGHT_BUTTON);*/

	glutMainLoop();
}