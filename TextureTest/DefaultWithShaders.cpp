#include <GL/glew.h>
#include <GL/glut.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <SOIL.h>

#include <iostream>

#include "Shader.h"

GLint width, height;
GLfloat screenWidth, screenHeight;

GLfloat vertices[] = {
	//Positions(x,y), Colors(R,G,B), Texture Coords(s,t)
	-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Bottom Right
	0.5, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	//Bottom left
	0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,	//Top right
	-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f	//Top left
};

GLuint indices[] = {
	0, 1, 3, //Triangle 1
	1, 2, 3 //Triangle 2
};

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glFlush();
	glutSwapBuffers();
}

void reshapeWindow(int width, int height)
{
	//glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//gluPerspective(60.0, 0.0, 100.0, 50.0);

	/*
	if (width <= height)
	glOrtho(-1.5, 1.5, -1.5*(GLfloat)height / (GLfloat)width,
	1.5*(GLfloat)height / (GLfloat)width, -10.0, 10.0);
	else
	glOrtho(-1.5*(GLfloat)width / (GLfloat)height,
	1.5*(GLfloat)width / (GLfloat)height, -1.5, 1.5, -10.0, 10.0);
	*/

	glutPostRedisplay();
}

void init()
{
	std::cout << "Initialize" << std::endl;
	screenWidth = 800;
	screenHeight = 600;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluPerspective(0, 0, screenWidth, screenHeight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	//-----------------------------
	//	OPENGL OBJECTS
	//-----------------------------
	std::cout << "Vertex Array Object..." << std::endl;
	//Generate Vertex Array Object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	std::cout << "Generating Vertex Buffer Object..." << std::endl;
	//Generate Vertex Buffer Object
	GLuint VBO;
	glGenBuffers(1, &VBO); //Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind the buffer to the current context
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //The buffer will read the data from the vertices array
																			  //specified globally.
	std::cout << "Generating Element Array Object";
	//Generate Element Array Object
	GLuint EBO;
	glGenBuffers(1, &EBO); //Generate 1 buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind the buffer to the current context
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //The buffer will read data from the vertex buffer in
																					 //the order specified by the indices array

	//---------------------------
	//	TEXTURES
	//---------------------------
	std::cout << "Initializing Shader" << std::endl;
	//Initialize the shader
	Shader ourShader("vsource.glsl", "fsource.glsl");
	ourShader.Use();

	//Get input for the vertex shader. The actual location to input in is specified in the vetex shader
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	//------------------------
	//	TEXTURES
	//------------------------

	//Set-up the texture
	GLuint texture;

	glGenTextures(1, &texture); //Get the texture id
	glBindTexture(GL_TEXTURE_2D, texture); //Bind the texture to the current context

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Specify texture wrapping type for S coordinate
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Specify texture wrapping type for T coordinate

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *image = SOIL_load_image("LastSupper.jpg", &width, &height, 0, SOIL_LOAD_RGBA); //Set an image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); //Generate a 2D texture image
	glGenerateMipmap(GL_TEXTURE_2D); //Generate mipmaps for the image
	SOIL_free_image_data(image); //Free the image data (a little bit of clean-up to help out our GPU
	glBindTexture(GL_TEXTURE_2D, 0); //Unbind our texture

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

	//----------------------------------------
	//	TRANSFORMATIONS
	//----------------------------------------

	//Create transformations for the container
	glm::mat4 transMatrix; //The transformation matrix
	transMatrix = glm::rotate(transMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); //Rotate by 90 degrees around the z-axis
	transMatrix = glm::scale(transMatrix, glm::vec3(0.5, 0.5, 0.5)); //Scale on all axes by 0.5

	//Inject transformation matrix to vertex shader
	GLuint transformMatLoc = glGetUniformLocation(ourShader.Program, "transMat"); //Get the location of the transformation matrix uniform value in GLSL
	glUniformMatrix4fv(transformMatLoc, 1, GL_FALSE, glm::value_ptr(transMatrix)); //Set the value of transMat in the vertex shader to be the transformation matrix we defined.

	/*
	glm::mat4 modelMat;
	modelMat = glm::rotate(modelMat, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 viewMat;
	viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projectMat;
	projectMat = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projectMat"), 1, GL_FALSE, glm::value_ptr(projectMat));*/
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Template");
	
	glewInit();

	init();

	glutDisplayFunc(display);
	//glutReshapeFunc(reshapeWindow);

	glutMainLoop();
}