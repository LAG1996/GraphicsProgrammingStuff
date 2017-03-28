/*
Author: Luis Angel Garcia
Description: A practice in shading and interactive viewing using GLUT API.
*/

#include <GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<time.h>
#include "Material.h"
#include "Light.h"
#include "Prism.h"

#include<fstream>
using namespace std;

/*
All the start stuff
*/
R_Prism * RectPrisms; //List of rectangles and their locations (immutable)
R_Prism * RectLoca; //Same list of rectangles, but the locations are variable to allow for interpolation from the origin to the generated point (mutable)

Vec4 CameraLoc; //Camera location
Vec4 CenterOfScene; //Focus of the scene
Vec4 cam_forward; //Camera's forward vector
Vec4 cam_right; //Camera's right vector
Vec4 cam_up; //Camera's up vector

Vec4 Move_Vec; //Vector to move along depending on key presses

//GLfloat Theta = 0; Useless, but keeping it for learning's sake
GLfloat camera_move_speed = 10.0f; //Speed at which the camera moves around the screen
GLfloat prism_move_speed = 0.0f; //Speed at which the prisms move from their spawn point (the origin) to their generated center points (stored in RectPrisms)
//GLfloat Time = time(NULL); Also useless, but I'll keep it.
//GLfloat dt = 0.0f; Aaaaaaand useless.

const int MAX_X = 100; //Maximum distance along the x-axis that the prisms will be allowed to be at
const int MAX_Z = 100; //Maximum distance along the z-axis that the prisms will be allowed to be at
const int MAX_HEIGHT = 30; //Maximum height each prism can have
const int MIN_HEIGHT = 5; //Mininimum height for each prisms
const int AMT_PRISMS = 200; //The amount of prisms I will spawn

int prism_count = 0; //Keeps track of the amount of prisms (ultimately useless since I have AMT_PRISMS, but it's here for debugging purposes
int prism_to_move = 0; //Keeps track of which prism to move when prisms are moving from the origin in mode 1
int MODE = 0; //Keeps track of which movement mode the prisms are in (altogether (0) or one-by-one (1))

//Utiliy functions
void DrawPrisms();
void MovePrisms();
void PrintControls();

void timer(int v)
{
	/*
	Useless, useless, useless

	GLfloat cur_time = time(NULL);
	dt = difftime(cur_time, Time);
	Time = cur_time;
	Theta += camera_move_speed;*/

	glLoadIdentity(); //Reset the model view for transformation
	
	/*Do some linear algebra to find the camera's orientation vectors*/
	cam_forward = CenterOfScene - CameraLoc;
	cam_forward = cam_forward.Normalized();

	cam_right = cam_forward * Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	cam_right = cam_right.Normalized();
	
	cam_up = cam_forward * cam_right;
	cam_up = cam_up.Normalized();

	CameraLoc = CameraLoc + Move_Vec * camera_move_speed; //Move camera

	//The punchline:
	gluLookAt(CameraLoc.x, CameraLoc.y, CameraLoc.z, CenterOfScene.x, CenterOfScene.y, CenterOfScene.z, 0, 1, 0);

	//Move prisms if there are any prisms to do
	//Remark: This could be optimized
	MovePrisms();
	//glRotatef(Theta, 0.0f, 1.0f, 0.0f); Here for debug reasons
	Move_Vec = Vec4::Zero(); //Reset the movement vector for user input
	glutPostRedisplay(); //Call display
	glutTimerFunc(1000.0/60.0, timer, v); //Set the timer for another 60th of a second
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glutSolidSphere(2, 100, 10);
	//glutSolidDodecahedron();
	DrawPrisms();
	//PrintControls();

	glFlush();
	glutSwapBuffers();
}

//Move prisms from origin according to which movement mode they are in
void MovePrisms()
{
	//MODE 0: Move prisms in a cluster
	if (MODE == 0)
	{
		for (int i = 0; i < prism_count; i++)
		{
			Vec4 Dir = RectPrisms[i].GetCenter() - RectLoca[i].GetCenter();

			if (Dir.Magnitude() > 1.5f)
				RectLoca[i].Translate(Dir.Normalized() * prism_move_speed);
			else
				RectLoca[i].SetCenter(RectPrisms[i].GetCenter());
		}
	}
	//MODE 1: Move prisms 1-by-1
	else if(MODE == 1)
	{
		if (prism_to_move < prism_count)
		{
			Vec4 Dir = RectPrisms[prism_to_move].GetCenter() - RectLoca[prism_to_move].GetCenter();

			if (Dir.Magnitude() > 1.5f)
				RectLoca[prism_to_move].Translate(Dir.Normalized() * prism_move_speed);
			else
			{
				RectLoca[prism_to_move].SetCenter(RectPrisms[prism_to_move].GetCenter());
				prism_to_move++;
			}
		}
	}
}

void PrintControls()
{
	glMatrixMode(GL_PROJECTION);
	string controls = "Q and W for prism movement modes\nE and R for prism movement speeds\n'x' and 'X' for lateral camera movement\n'z' and 'Z' for forward movement\n'y' and 'Y' for up-and-down movement";
	int len = controls.length();
	glRasterPos2f(1.0f, 0.0f);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, controls[i]);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//Draw each prism in prism array using polygons
void DrawPrisms()
{
	for(int i = 0; i < prism_count; i++)
	{
		Vec4 * points = new Vec4[8];
		points = RectLoca[i].GetPoints();
		Vec4 * normals = new Vec4[8];
		normals = RectLoca[i].GetNormals();
		
		glBegin(GL_POLYGON);
		glNormal3fv(normals[0].ToList());
		glVertex3fv(points[0].ToList());
		glNormal3fv(normals[1].ToList());
		glVertex3fv(points[1].ToList());
		glNormal3fv(normals[2].ToList());
		glVertex3fv(points[2].ToList());
		glNormal3fv(normals[3].ToList());
		glVertex3fv(points[3].ToList());
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3fv(normals[4].ToList());
		glVertex3fv(points[4].ToList());
		glNormal3fv(normals[5].ToList());
		glVertex3fv(points[5].ToList());
		glNormal3fv(normals[6].ToList());
		glVertex3fv(points[6].ToList());
		glNormal3fv(normals[7].ToList());
		glVertex3fv(points[7].ToList());
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3fv(normals[3].ToList());
		glVertex3fv(points[3].ToList());
		glNormal3fv(normals[2].ToList());
		glVertex3fv(points[2].ToList());
		glNormal3fv(normals[6].ToList());
		glVertex3fv(points[6].ToList());
		glNormal3fv(normals[7].ToList());
		glVertex3fv(points[7].ToList());
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3fv(normals[0].ToList());
		glVertex3fv(points[0].ToList());
		glNormal3fv(normals[1].ToList());
		glVertex3fv(points[1].ToList());
		glNormal3fv(normals[5].ToList());
		glVertex3fv(points[5].ToList());
		glNormal3fv(normals[4].ToList());
		glVertex3fv(points[4].ToList());
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3fv(normals[3].ToList());
		glVertex3fv(points[3].ToList());
		glNormal3fv(normals[0].ToList());
		glVertex3fv(points[0].ToList());
		glNormal3fv(normals[4].ToList());
		glVertex3fv(points[4].ToList());
		glNormal3fv(normals[7].ToList());
		glVertex3fv(points[7].ToList());
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3fv(normals[2].ToList());
		glVertex3fv(points[2].ToList());
		glNormal3fv(normals[1].ToList());
		glVertex3fv(points[1].ToList());
		glNormal3fv(normals[5].ToList());
		glVertex3fv(points[5].ToList());
		glNormal3fv(normals[6].ToList());
		glVertex3fv(points[6].ToList());
		glEnd();

	}
}

//Set up perspective camera
void reshapeWindow(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(60.0, 0.0, 100.0, 50.0);

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

void ReadKeys(unsigned char key, int x, int y)
{
	Move_Vec = Vec4::Zero();
	//Controls copied straight from Angel!
	if(key == 'x') 
		Move_Vec += cam_right;
	if(key == 'X') 
		Move_Vec += cam_right*-1.0f;
	if (key == 'y')
	{
		Move_Vec += cam_up;
	}
	if (key == 'Y')
	{
		Move_Vec += cam_up*-1.0f;
	}
		
	if (key == 'z')
	{
		Move_Vec += cam_forward;
		if (((CameraLoc + Move_Vec) - CenterOfScene).Magnitude() <= 1.0f)
			Move_Vec = Move_Vec - cam_forward;
	}
	if(key == 'Z')
		Move_Vec += cam_forward*-1.0f;

	//Except this stuff. I did this stuff myself.
	if (key == 'q')
		MODE = 0;//Switch to mode 0
	if (key == 'w')
		MODE = 1;//Switch to mode 1
	if (key == 'e')
		prism_move_speed += 0.01; //Speed up prism movement
	if (key == 'r')
	{
		//Slow down prism movement to 0
		if (prism_move_speed > 0.0f)
			prism_move_speed -= 0.01;
		else
			prism_move_speed == 0.0f;
	}
}

//Generate the rectangular prism array from either a file or from random values if there is no such file.
void initializePrisms()
{
	ifstream inFile;
	inFile.open("RectLocas.txt");
	RectPrisms = new R_Prism[AMT_PRISMS];
	RectLoca = new R_Prism[AMT_PRISMS];

	if(inFile.is_open())
	{
		GLfloat x;
		GLfloat z;
		GLfloat height;
		while (inFile >> x)
		{
			inFile >> z;
			inFile >> height;

			GLfloat width = 0.75;
			GLfloat depth = 0.75;

			RectPrisms[prism_count] = R_Prism(Vec4((GLfloat)x, (GLfloat)height / 2.0f, z, 0.0f), (GLfloat)height, (GLfloat)width, (GLfloat)depth);
			RectLoca[prism_count] = R_Prism(Vec4(CenterOfScene.x, height / 2.0f, CenterOfScene.z, 0.0f), height, width, depth);

			prism_count++;
		}
	}
	else
	{
		ofstream oFile;
		oFile.open("RectLocas.txt");

		for (int i = 0; i < AMT_PRISMS; i++)
		{
			GLfloat x = rand() % MAX_X;
			GLfloat z = rand() % MAX_Z;
			GLfloat height = (rand() % MAX_HEIGHT) + MIN_HEIGHT;

			GLfloat width = 0.75;
			GLfloat depth = 0.75;

			RectPrisms[i] = R_Prism(Vec4(x, height / 2.0f, z, 0.0f), height, width, depth);
			RectLoca[i] = R_Prism(Vec4(CenterOfScene.x, height / 2.0f, CenterOfScene.z, 0.0f), height, width, depth);

			oFile << x << endl;
			oFile << z << endl;
			oFile << height << endl;
		}

		prism_count = AMT_PRISMS;
		oFile.close();
	}
	inFile.close();
}

//Initialize lighting, materials, fog, the location of the focal point for the camera, the location of the camera itself and its orientation
void init()
{
	glClearColor(0.3, 0.3, 0.3, 0.0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	CenterOfScene = { MAX_X / 2, 5.0, MAX_Z / 2, 0.0f };
	CameraLoc = { MAX_X / 2, 40.0f, 40.0f, 0.0f };

	Material mat;
	mat.ambient = { 1.0f, 0.3f, 0.3f, 1.0f };
	mat.diffuse = { 1.0f, 0.3f, 0.6f, 1.0f };
	mat.specular = { 1.0f, 1.0f, 1.0f, 0.1f };
	GLfloat s = 50.0f;
	mat.shininess = &s;

	PointLight light;
	light.position = { -1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.ambient.ToList());
	glMaterialfv(GL_FRONT, GL_SHININESS, mat.shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular.ToList());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse.ToList());
	glLightfv(GL_LIGHT0, GL_POSITION, light.position.ToList());

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, Vec4(0.45f, 0.6f, 0.6f, 1.0f).ToList());
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 100.0f);

	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	//Generate the AMT_PRISMS rectangles at random centers within a square of side length 10
	
	initializePrisms();

	cam_up = { 0.0f, 1.0f, 0.0f, 1.0f };

	cam_forward = CenterOfScene - CameraLoc;
	cam_up = cam_forward * cam_up;

	cam_right = cam_up * cam_forward;

	glMatrixMode(GL_MODELVIEW);
	Move_Vec = Vec4::Zero();
	//glTranslatef(0.0f, 0.0f, -10.0f);
	
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Lighting Test");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);
	glutTimerFunc(1000.0/60.0f, timer, 0);
	glutKeyboardFunc(ReadKeys);

	init();
	glutMainLoop();
}