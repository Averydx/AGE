#include "Engine.h"
#include <stdexcept>
#include "Shader.h"
#include <iostream>
#include <math.h>
#include "mesh.h"
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "time.h"
#include <cstdlib>

namespace AGE
{
	bool firstMouse = true; 
	float lastX = 800 / 2.0f;
	float lastY = 600 / 2.0f;
	Camera cam{};


	//callback function on mouse movement 
	void Engine::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		cam.ProcessMouseMovement(xoffset, yoffset);

	}


	//Initializes the OpenGL context and windowing information
	void Engine::Init()
	{
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("GLFW failed to initialize");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	}

	//process the input from key presses 
	void Engine::processInput(Camera& cam,float deltaTime)
	{
		if (glfwGetKey(this->MainWindow.get()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(this->MainWindow.get()->getWindow(), true);
		if (glfwGetKey(this->MainWindow.get()->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			cam.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(this->MainWindow.get()->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			cam.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(this->MainWindow.get()->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			cam.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(this->MainWindow.get()->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			cam.ProcessKeyboard(RIGHT, deltaTime);

	}


	//constructor for the engine
	Engine::Engine(int window_width, int window_height, std::string window_name)
	{
		Init(); 
		MainWindow = std::make_unique<Window>(Window(window_width,window_height,window_name)); 
		glfwSetInputMode(this->MainWindow.get()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glViewport(0, 0, MainWindow.get()->getWidth(), MainWindow.get()->getHeight());
		ProjectionMatrix = glm::perspective(glm::radians(60.f), float(MainWindow.get()->getWidth()) / float(MainWindow.get()->getHeight()), 0.1f, 100.0f);

		
		
		glEnable(GL_DEPTH_TEST);
		
	}

	//run function, infinite loop
	void Engine::Run()
	{
		
		
		Shader cubeShader("MeshVertex.glsl", "MeshFrag.glsl"); 
		Shader lightShader("LightVertex.glsl", "LightFrag.glsl"); 
		// timing
		float deltaTime = 0.0f;	// time between current frame and last frame
		float lastFrame = 0.0f;
		glfwSetCursorPosCallback(MainWindow.get()->getWindow(), mouse_callback);

		std::vector<float> vertices= {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};


		glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		std::vector<Mesh> cubes; 

		for (int i = 0; i < 10; i++)
		{
			cubes.push_back(Mesh(vertices)); 
		}

		Mesh lightCube(vertices); 

		glm::vec3 lightPos(5.2f, 5.0f, 5.0f);

		
		unsigned int texture1 = loadTexture("C:\\Users\\avery\\Downloads\\new_container.png"); 
		unsigned int texture2 = loadTexture("C:\\Users\\avery\\Downloads\\new_container_specular.png"); 

		cubeShader.use();
		cubeShader.setInt("material.diffuse", 0);
		cubeShader.setInt("material.specular", 1);

		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);



	while (!glfwWindowShouldClose(MainWindow.get()->getWindow()))
		{

			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processInput(cam,deltaTime); 
			
			// render
			// ------
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = cam.GetViewMatrix();

			// bind textures on corresponding texture units
			cubeShader.setTexture(texture1, 0); 
			cubeShader.setTexture(texture2, 1); 
	

			// get matrix's uniform location and set matrix
			cubeShader.use();
			cubeShader.setVec3("light.position", cam.Position);
			cubeShader.setVec3("light.direction", cam.Front); 
			cubeShader.setVec3("viewPos", cam.Position);

			glm::vec3 lightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
			glm::vec3 lightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f); 
			glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

			
			

			// light properties
			cubeShader.setVec3("light.ambient", lightAmbient);
			cubeShader.setVec3("light.diffuse", lightDiffuse);
			cubeShader.setVec3("light.specular", lightSpecular);
			cubeShader.setFloat("light.constant", 1.0f);
			cubeShader.setFloat("light.linear", 0.09f);
			cubeShader.setFloat("light.quadratic", 0.032f);
			cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
			cubeShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));


			// material properties
			cubeShader.setFloat("material.shininess", 32.0f);

			// render container
			

			cubeShader.setMat4("projection", this->ProjectionMatrix); 
			cubeShader.setMat4("view", view); 


			glBindVertexArray(this->VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 36);


			for (unsigned int i = 0; i < cubes.size(); i++)
			{

				glm::mat4 model = cubes[i].getModelMatrix();
				model = glm::translate(model, cubePositions[i]); 

				float angle = 20.0f * i;
				// calculate the model matrix for each object and pass it to shader before drawing
				model = glm::rotate(model,angle, glm::vec3(1.0f, 0.3f, 0.5f));

				cubeShader.setMat4("model", model);
				cubes[i].BindBuffers(); 

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//lightShader.use(); 
			//view = cam.GetViewMatrix(); 
			//lightShader.setMat4("view", view); 
			//glm::mat4 model = glm::mat4(1.0f); 
			//model = glm::scale(model,glm::vec3(0.2f));
			//model = glm::translate(model, lightPos); 

			//lightShader.setMat4("model", model);
			//lightShader.setMat4("projection", this->ProjectionMatrix); 
			//lightCube.BindBuffers(); 
			//glDrawArrays(GL_TRIANGLES, 0, 36);


			glfwSwapBuffers(MainWindow.get()->getWindow());
			glfwPollEvents();
		}
	}

	

}
