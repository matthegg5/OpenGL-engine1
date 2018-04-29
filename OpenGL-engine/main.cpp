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
// tutorial 6 - Cameras - done
// tutorial 7 - colours - done
// tutorial 8 - basic lighting - done
//tutorial fuck knows - materials - done
// tutorial 10 - lighting maps - textures nd shit - done
// tutorial 11 - directional lighting - done
// tutorial 12 - Point Light - done
// tutorial 13 - spot lighting - done

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

glm::vec3 lightPos(1.2f, 1.0f, -2.0f);

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
	
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};


	GLuint VertexBufferObject, boxVAO;

	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VertexBufferObject);



	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(boxVAO);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//texture attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat))); //last parameter is what position in the vertices array to start
	glEnableVertexAttribArray(2);


	glBindVertexArray(0); // Unbind boxVAO

	//GLuint  lightVAO;

	//glGenVertexArrays(1, &lightVAO);
	//glGenBuffers(1, &VertexBufferObject);


	//glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	////position
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0); //Unbind lightVAO

	

	GLuint diffuseMap, specularMap, emissionMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);

	int textureWidth, textureHeight;
	unsigned char *image;

	//diffuse map

	image = SOIL_load_image("res/container2.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	//specular map

	image = SOIL_load_image("res/container2_specular.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f); //perspective



	//projection = glm::ortho(0.0f, (GLfloat)SCREEN_WIDTH, 0.0f, (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f); //orthographic

	//game loop

	while (!glfwWindowShouldClose(window))
	{

		

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// make the lightPos change per frame (move the light source)

		//lightPos.x += 0.01f * cos((float)currentFrame);
		//lightPos.y += 0.01f * sin((float)currentFrame);

		//std::cout << currentFrame << std::endl;
		

		glfwPollEvents();

		DoMovement();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		//GLint objectColourLoc = glGetUniformLocation(lightingShader.Program, "objectColour");
		//GLint lightColourLoc = glGetUniformLocation(lightingShader.Program, "lightColour");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint lightSpotDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint lightSpotcutoffLoc = glGetUniformLocation(lightingShader.Program, "light.cutoff");
		GLint lightSpotouterCutoffLoc = glGetUniformLocation(lightingShader.Program, "light.outerCutoff");

		//GLint lightDirectionLocation = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);
		
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shine"), 32.0f);




		//glm::vec3 lightColour;
		//lightColour.r = sin(glfwGetTime() * 2.0f);
		//lightColour.g = sin(glfwGetTime() * 0.7f);
		//lightColour.b = sin(glfwGetTime() * 1.3f);

		//glm::vec3 diffuseColour = lightColour * glm::vec3(0.5f);
		//glm::vec3 ambientColour = diffuseColour * glm::vec3(0.2f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), ambientColour.r, ambientColour.g, ambientColour.b);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), diffuseColour.r, diffuseColour.g, diffuseColour.b);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shine"), 32.0f);

		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(lightPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(lightSpotDirLoc, camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform1f(lightSpotcutoffLoc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightSpotouterCutoffLoc, glm::cos(glm::radians(17.5f)));


		//glUniform3f(lightDirectionLocation, -0.2f, 1.0f, -0.3f);
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		

		glm::mat4 view;
		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projectionLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glBindVertexArray(0);

		//glBindVertexArray(boxVAO);
		//glm::mat4 model;
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		//lampShader.Use();


		// modelLoc = glGetUniformLocation(lampShader.Program, "model");
		// viewLoc = glGetUniformLocation(lampShader.Program, "view");
		// projectionLoc = glGetUniformLocation(lampShader.Program, "projection");

		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//model = glm::mat4();
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//
		//
		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);


		glfwSwapBuffers(window);

	}

	//glDeleteVertexArrays(1, &lightVAO);
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
