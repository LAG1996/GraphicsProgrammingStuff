#include<iostream>

//#define GLEW_STATIC
#include<GLEW\GL\glew.h>
#include<glfw3.h>

#include<SOIL\SOIL.h>
#include<GLM\glm.hpp>

#include<GLM\gtc\matrix_transform.hpp>
#include<GLM\gtc\type_ptr.hpp>

#include"Shader.h"
#include"Camera.h"


//Initialize some variables
int WIDTH = 800, HEIGHT = 600; //Width and height of the screen

bool keys[1024];

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f)); //Initialize the camera with a position
glm::vec3 lightSourcePos(1.2f, 1.0f, 2.0f); //The light source's position

glm::vec3 lightColor(1.0f, 0.3f, 0.4f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

double lastX = 0.0;
double lastY = 0.0;

//A texture
const char * boxDiffusePNG = "container2.png";
const char * boxSpecularPNG = "container2_specular.png";

//Event callbacks
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);

//Utility functions
void do_movement();


//Specification for our cube
GLfloat vertices[] = {
	// Positions           // Normals           // Texture Coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//Set input mode
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide mouse

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	//Build and compile shader program
	Shader objectShader("vertShader.glsl", "fragShader.glsl");
	Shader lampShader("lampVertShader.glsl", "lampFragShader.glsl");

	/*
		Set vertex array object and buffer object
	*/
	GLuint VBO, containerVAO;
	//Set the container's VAO
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(containerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Tell the array which elements are the coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Tell the array which elements are the normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Tell the array which elements are the texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//Set the lamp's VAO. VBO stays the same since the two objects are the same shape
	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	
	glBindVertexArray(lampVAO);
	//Bind this array to the VBO. No need to fill it with new data, since the container and lamp are using the same data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Tell the array which elements are the coordinates. This is all we need for the lamp since it is not going to change colors (we set it to white in
	//the fragment shader)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Load textures
	GLuint diffuseMap;
	glGenTextures(1, &diffuseMap);
	GLuint specularMap;
	glGenTextures(1, &specularMap);

	int width, height;
	unsigned char* image;
	//Diffuse map
	image = SOIL_load_image(boxDiffusePNG, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Specular map
	image = SOIL_load_image(boxSpecularPNG, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Set texture units
	objectShader.Use();
	glUniform1i(glGetUniformLocation(objectShader.Program, "material.diffuse"), 0);

	glUniform1i(glGetUniformLocation(objectShader.Program, "material.specular"), 1);

	while (!glfwWindowShouldClose(window))
	{
		//Get delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		glfwPollEvents(); //Handle window events
		do_movement(); //Move according to mouse movement

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 projection;
		glm::mat4 view;

		objectShader.Use();
		
		GLint lightPosLoc = glGetUniformLocation(objectShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(objectShader.Program, "viewPos");
		
		/*
			OBJECT 1's PROPERTIES
		*/
		glUniform3f(lightPosLoc, lightSourcePos.x, lightSourcePos.y, lightSourcePos.z);
		glUniform3f(viewPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);

		//Set light struct's properties
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		//Set material struct's properties
		glUniform3f(glGetUniformLocation(objectShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(objectShader.Program, "material.shininess"), 64.0f);

		//Create camera transformations
		view = cam.GetViewMatrix();
		projection = glm::perspective(cam.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		//Get the uniform locations for each matrix that we are going to manipulate
		GLint modelLoc = glGetUniformLocation(objectShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(objectShader.Program, "view");
		GLint projectionLoc = glGetUniformLocation(objectShader.Program, "projection");

		//Pas the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Bind the diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//Bind the specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//Draw the container (using the container's vertex attributes)
		glBindVertexArray(containerVAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		/*
		OBJECT 2's PROPERTIES
		*/
		glUniform3f(lightPosLoc, lightSourcePos.x, lightSourcePos.y, lightSourcePos.z);
		glUniform3f(viewPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);

		//Set light struct's properties
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		//Set material struct's properties
		glUniform3f(glGetUniformLocation(objectShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(objectShader.Program, "material.shininess"), 64.0f);

		//Create camera transformations
		view = cam.GetViewMatrix();
	    projection = glm::perspective(cam.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		//Get the uniform locations for each matrix that we are going to manipulate
		modelLoc = glGetUniformLocation(objectShader.Program, "model");
		viewLoc = glGetUniformLocation(objectShader.Program, "view");
		projectionLoc = glGetUniformLocation(objectShader.Program, "projection");

		//Pas the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Bind the diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//Bind the specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//Draw the container (using the container's vertex attributes)
		glBindVertexArray(containerVAO);
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		/*
		OBJECT 3's PROPERTIES
		*/
		glUniform3f(lightPosLoc, lightSourcePos.x, lightSourcePos.y, lightSourcePos.z);
		glUniform3f(viewPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);

		//Set light struct's properties
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.diffuse"), 0.6f, 0.2f, 0.4f);
		glUniform3f(glGetUniformLocation(objectShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		//Set material struct's properties
		glUniform3f(glGetUniformLocation(objectShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(objectShader.Program, "material.shininess"), 64.0f);

		//Create camera transformations
		view = cam.GetViewMatrix();
		projection = glm::perspective(cam.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		//Get the uniform locations for each matrix that we are going to manipulate
		modelLoc = glGetUniformLocation(objectShader.Program, "model");
		viewLoc = glGetUniformLocation(objectShader.Program, "view");
		projectionLoc = glGetUniformLocation(objectShader.Program, "projection");

		//Pas the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Bind the diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//Bind the specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//Draw the container (using the container's vertex attributes)
		glBindVertexArray(containerVAO);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		/*
			LAMP'S PROPERTIES
		*/

		lampShader.Use(); //The lamp is shaded differently from the container

		glUniform3f(glGetUniformLocation(lampShader.Program, "color"), lightColor.x, lightColor.y, lightColor.z);

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projectionLoc = glGetUniformLocation(lampShader.Program, "projection");
		//Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightSourcePos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		//Now draw the lamp using the lamp's vertex attributes
		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

//Handle keyboard press
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

//Move camera
void do_movement()
{
	//Camera controls
	if (keys[GLFW_KEY_W])
	{
		cam.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		cam.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		cam.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_E])
	{
		cam.ProcessKeyboard(UP, deltaTime);
	}
	if (keys[GLFW_KEY_Q])
	{
		cam.ProcessKeyboard(DOWN, deltaTime);
	}

}

bool firstMouse = true;
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;

		firstMouse = false;
	}

	GLfloat deltaX = xpos - lastX;
	GLfloat deltaY = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(deltaX, deltaY);
}

void scroll_callback(GLFWwindow * window, double deltaX, double deltaY)
{
	cam.ProcessMouseScroll(deltaY);
}