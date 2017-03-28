#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

#include "Shader.h"


GLfloat vertices[] = {
	//Positions(x,y), Colors(R,G,B)
	-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //Left-Bottom
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //Right-Bottom
	0.0f, 0.5f,	0.0f, 0.0f, 1.0f //Top
};

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glFlush();
	glutSwapBuffers();
}

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
	glLoadIdentity();
	glutPostRedisplay();
}

void init()
{
	std::cout << "Initialize" << std::endl;
	glClearColor(0.0, 0.0, 0.0, 0.0);

	std::cout << "Vertex Array Object..." << std::endl;
	//Generate Vertex Array Object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	std::cout << "Generating Vertex Buffer Object..." << std::endl;
	//Generate Vertex Buffer Object
	GLuint VBO;
	glGenBuffers(1, &VBO); //Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	std::cout << "Initializing Shader" << std::endl;
	//Initialize the shader
	Shader ourShader("vsource.glsl", "fsource.glsl");
	ourShader.Use();

	GLuint posAttrib = glGetAttribLocation(ourShader.Program, "vPosition");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);

	GLuint colorAttrib = glGetAttribLocation(ourShader.Program, "vColor");
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(colorAttrib);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Template");
	
	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshapeWindow);

	glutMainLoop();
}