#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow*, double, double);
void _HandleMove();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//Some utility variables
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
double last_mouse_x = WIDTH/2;
double last_mouse_y = HEIGHT/2;

//Initialize Camera
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat cam_speed = 0.1f;

//Initialize light position
glm::vec3 Light_Pos(1.2f, 1.0f, 2.0f);

//Array of keys
bool keys[1024];

//Simple Vertex Data for a cube
GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// The MAIN function, from here we start the application and run the game loop
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	//Generate Shaders
	Shader objectShader("ObjVert.glsl", "ObjFrag.glsl");
	Shader lampShader("ObjVert.glsl", "LampFrag.glsl");

	//Create Vertex Arrays and Element and Vertex Buffer Obects
	GLuint VAO, VBO, EBO, LIGHT_VAO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO); //Bind VAO for editing
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void * )0); //Point to positions of vertices
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); //Unbind VAO to end editing

	glGenVertexArrays(1, &LIGHT_VAO);
	glBindVertexArray(LIGHT_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	lastFrame = glfwGetTime();
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
		//Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions	
		glfwPollEvents();
		_HandleMove();

        // Render
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		/*------------------------------
		*	COLORING OBJECT
		*-----------------------------*/
		
		objectShader.Use();

		/*
		*	SET CAMERA
		*/
		view = cam.GetViewMatrix();
		projection = glm::perspective(cam.Zoom, (GLfloat)WIDTH / GLfloat(HEIGHT), 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(objectShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(objectShader.Program, "view");
		GLint projLoc = glGetUniformLocation(objectShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Color the object
		//GLuint objectColorLoc = glGetUniformLocation(objectShader.Program, "ObjectColor");
		GLint lightColorLoc = glGetUniformLocation(objectShader.Program, "LightColor");
		GLint matAmbientLoc = glGetUniformLocation(objectShader.Program, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(objectShader.Program, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(objectShader.Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(objectShader.Program, "material.shininess");

		glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform1f(matShineLoc, 32.0f);

		//Set the light source position in the fragment shader
		GLint lightPosLoc = glGetUniformLocation(objectShader.Program, "LightPos");
		glUniform3f(lightPosLoc, Light_Pos.x, Light_Pos.y, Light_Pos.z);

		//Set the viewer's position in the fragment shader
		GLint viewPosLoc = glGetUniformLocation(objectShader.Program, "ViewPos");
		glUniform3f(viewPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);

		//glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.3f); //Set object's color to be coral
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); //Set lamp's color to be white

		//Draw the object at the location specified by model matrix
		glBindVertexArray(VAO);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		/*
		*	COLORING LAMP
		*/
		lampShader.Use();

		//Get the locations of the matrix variables in the lampShader's vertex shader subprogram
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(); //Reset model matrix so we don't get any weirdness
		model = glm::translate(model, Light_Pos);
		model = glm::scale(model, glm::vec3(0.2f));

		//Draw the lamp at the location specified by the model matrix
		glBindVertexArray(LIGHT_VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (firstMouse)
	{
		last_mouse_x = x_pos;
		last_mouse_y = y_pos;
		firstMouse = false;
	}

	GLfloat x_offset = x_pos - last_mouse_x;
	GLfloat y_offset = last_mouse_y - y_pos;

	last_mouse_x = x_pos;
	last_mouse_y = y_pos;

	cam.ProcessMouseMovement(x_offset, y_offset);
}

void _HandleMove()
{
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