#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <GLEW/GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL/SOIL.h>
// GLM Mathematics
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// Other includes
#include "Util/Shader.h"
#include "Util/Model.h"
#include "Util/Camera.h"

#include <vector>
#include <time.h>


const float CAM_SPEED = 5.0f;

Camera cam(glm::vec3(80.0f, 1.0f, 3.0f));

//Some utility variables
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

double lastX = 0.0;
double lastY = 0.0;

bool keys[1024];
bool first_mouse = true;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

// Utility functions
void move_cam();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//A list of trees
std::vector<Model> Tree_List;
//A list of tree positions
std::vector<glm::mat4> Tree_ModelMats;

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

	// Options
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

	//Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	//Initialize a shader
	Shader shader("vertShader.glsl", "toonShaderFrag.glsl");
	Shader simpleColorShade("vertShader.glsl", "singleColorFrag.glsl");
	Model new_model("Tree_1/untitled.obj");
	Model building("Nice_Building/Budynek 5.obj");
	Model ground("Ground/untitled.obj");

	//Spawn a bunch of trees
	for (int i = 0; i < 150; i++)
	{
		glm::mat4 model;
		float x_pos = (float)(rand() % 100);
		float z_pos = (float)(rand() % 100);

		float rot = (float)(rand() % 180);

		model = glm::translate(model, glm::vec3(x_pos, 0.0, z_pos));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0, 1.0, 0.0));

		Tree_List.push_back(new_model);
		Tree_ModelMats.push_back(model);
	}

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		move_cam();

		// Render
		// Clear the color buffer
		glClearColor(0.8f, 0.7f, 0.0f, 0.65f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		shader.Use();

		view = cam.GetViewMatrix();
		projection = glm::perspective(cam.Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 500.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), cam.Position.x, cam.Position.y, cam.Position.z);

		glUniform3f(glGetUniformLocation(shader.Program, "directional_light.direction"), 1.0f, 1.0f, 0.3f);
		glUniform3f(glGetUniformLocation(shader.Program, "directional_light.ambient"), 0.2f, 0.2f, 0.5f);
		glUniform3f(glGetUniformLocation(shader.Program, "directional_light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(shader.Program, "directional_light.specular"), 1.0f, 1.0f, 1.0f);

		//========================
		// DRAW THE BUILDING AND GROUND FIRST
		//========================
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		model = glm::translate(model, glm::vec3(-40.0f, 5.0f, 50.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		building.Draw(shader, glm::vec3(.4f, 0.0f, .7f), .65, deltaTime, false);


			//=========================================================
			// DRAWING TREES
			//=========================================================
			
			//Draw objects as normal, filling the stencil buffer
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			for (int i = 0; i < Tree_List.size(); i++)
			{
				glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(Tree_ModelMats.at(i)));

				Tree_List.at(i).Draw(shader, glm::vec3(1.0f, 1.0f, 1.0f), .65, deltaTime, false, Tree_List.size());
			}

			//Now, draw a slightly scaled version of the tree, but disable stencil writing.
			
			float scale = 1.015;
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			simpleColorShade.Use();

			glUniformMatrix4fv(glGetUniformLocation(simpleColorShade.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(simpleColorShade.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			for (int i = 0; i < Tree_List.size(); i++)
			{
				glUniformMatrix4fv(glGetUniformLocation(simpleColorShade.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::scale(Tree_ModelMats.at(i), glm::vec3(scale, scale, scale))));
				glUniform3f(glGetUniformLocation(simpleColorShade.Program, "color"), 0.0f, 0.0f, 0.0f);
				Tree_List.at(i).Draw(simpleColorShade, glm::vec3(6.0f, 0.0f, 5.0f), .65, deltaTime, false, Tree_List.size());
			}
			

			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);

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

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

}

void mouse_callback(GLFWwindow * window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		lastX = x_pos;
		lastY = y_pos;
		first_mouse = false;
	}

	GLfloat x_offset = x_pos - lastX;
	GLfloat y_offset = lastY - y_pos;

	lastX = x_pos;
	lastY = y_pos;

	cam.ProcessMouseMovement(x_offset, y_offset);
}

void move_cam()
{
	if (keys[GLFW_KEY_W])
	{
		cam.ProcessKeyboard(FORWARD, deltaTime * CAM_SPEED);
	}
	if (keys[GLFW_KEY_A])
	{
		cam.ProcessKeyboard(LEFT, deltaTime * CAM_SPEED);
	}
	if (keys[GLFW_KEY_S])
	{
		cam.ProcessKeyboard(BACKWARD, deltaTime * CAM_SPEED);
	}
	if (keys[GLFW_KEY_D])
	{
		cam.ProcessKeyboard(RIGHT, deltaTime * CAM_SPEED );
	}
	if (keys[GLFW_KEY_E])
	{
		cam.ProcessKeyboard(UP, deltaTime * CAM_SPEED);
	}
	if (keys[GLFW_KEY_Q])
	{
		cam.ProcessKeyboard(DOWN, deltaTime * CAM_SPEED);
	}
}