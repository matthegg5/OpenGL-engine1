// main.cpp : Defines the entry point for the console application.
//

#include <iostream>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL2.h"
#include "shader.h"

//GLM

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// SETUP - GLEW, GLFW, SOIL2 - COMPLETE
//tutorial 2 - shader files - COMPLETE
//tutorial 3 - textures - COMPLETE
// tutorial 4 - transformations - next

const GLint WIDTH = 1366, HEIGHT = 768;


int mainTutorial() //rename to main to get to work
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL gameEngine", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{

		std::cout << "Failed to initialise GLEW" << std::endl;

		return EXIT_FAILURE;


	}

	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	GLfloat vertices[] =
	{
		//Positions				//Colours						//textures
		0.5f, 0.5f, 0.0f,			1.0f, 0.0f, 0.0f,		1.0f, 1.0f,						//top right
		0.5f, -0.5f, 0.0f,			0.0f, 1.0f, 0.0f,		1.0f, 0.0f,						//bottom right
		-0.5f, -0.5f, 0.0f,			0.0f, 0.0f, 1.0f,		0.0f, 0.0f,						//bottom left
		-0.5f, 0.5f, 0.0f,			1.0f, 0.0f, 1.0f,		0.0f, 1.0f	
	};

	GLuint indices[] =
	{

		0, 1, 3, // First Triangle
		1, 2, 3 //seond triangle

	};

	GLuint VertexBufferObject, VAO, ElementBufferObject;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VertexBufferObject);
	glGenBuffers(1, &ElementBufferObject);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0 );
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 *sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	GLuint texture;

	int width, height;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *image = SOIL_load_image("bitmap.bmp", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); 
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//game loop
	
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
		glClearColor(0.6f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();

		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		GLint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//draw stuff here

		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VertexBufferObject);
	glDeleteBuffers(1, &ElementBufferObject);

	glfwTerminate();

	return EXIT_SUCCESS;


}