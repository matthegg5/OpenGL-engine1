// main.cpp : Defines the entry point for the console application.
//

#include <iostream>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL2.h"
#include "shader.h"
#include "Camera.h"

//GLM

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// SETUP - GLEW, GLFW, SOIL2 - COMPLETE
//tutorial 2 - shader files - COMPLETE
//tutorial 3 - textures - COMPLETE
// tutorial 4 - transformations - COMPLETE (tweaked to work with relative linking and 
// tutorial 5 - Projections and Coordinate Systems - COMPLETE (OpenGL uses right-handed coordinate system)
// tutorial 6 - Cameras - long one - next

const GLint WIDTH = 1366, HEIGHT = 768;

int SCREEN_WIDTH, SCREEN_HEIGHT;
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//making changes
int main() //rename to main to get to work
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL gameEngine", nullptr, nullptr);

	
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (nullptr == window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //fix the mouse to the window



	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{

		std::cout << "Failed to initialise GLEW" << std::endl;

		return EXIT_FAILURE;


	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);


	Shader lampShader("lamp.vs", "lamp.fs");
	Shader lightingShader("lighting.vs", "lighting.fs");

	// Set up vertex data (and buffer(s)) and attribute pointers

	// use with Perspective Projection
	
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  

		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f  
	};



	GLuint VertexBufferObject, boxVAO;

	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VertexBufferObject);



	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(boxVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);


	GLuint  lightVAO;

	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VertexBufferObject);


	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f); //perspective



	//projection = glm::ortho(0.0f, (GLfloat)SCREEN_WIDTH, 0.0f, (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f); //orthographic

	//game loop

	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		DoMovement();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint objectColourLoc = glGetUniformLocation(lightingShader.Program, "objectColour");
		GLint lightColourLoc = glGetUniformLocation(lightingShader.Program, "lightColour");
		glUniform3f(objectColourLoc, 1.0f, 0.9f, 0.36f);
		glUniform3f(lightColourLoc, 1.0f, 0.5f, 1.0f);

		glm::mat4 view;
		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projectionLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(boxVAO);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lampShader.Use();


		 modelLoc = glGetUniformLocation(lampShader.Program, "model");
		 viewLoc = glGetUniformLocation(lampShader.Program, "view");
		 projectionLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteBuffers(1, &VertexBufferObject);

	glfwTerminate();

	return EXIT_SUCCESS;

}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
	{

		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{

		camera.ProcessKeyboard(BACKWARD, deltaTime);

	}


	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{

		camera.ProcessKeyboard(LEFT, deltaTime);

	}


	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{

		camera.ProcessKeyboard(RIGHT, deltaTime);

	}


}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_ESCAPE &&  GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);

	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;

		}
		else if (action == GLFW_RELEASE)
		{

			keys[key] = false;
		}

	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{

		lastX = xPos;
		lastY = yPos;
		firstMouse = false;

	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;
	
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);

}
