#include "Engine.h"
#include <stdexcept>
#include "Shader.h"
#include <iostream>
#include <math.h>
#include "mesh.h"
#include "Utils.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "time.h"
#include <cstdlib>
#include "Lights.h"

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

		// positions of the point lights
		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
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

		SpotLight flashLight{};
		flashLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		flashLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		flashLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		flashLight.constant = 1.0f; 
		flashLight.linear = 0.09f; 
		flashLight.quadratic = 0.032f; 
		flashLight.cutOff = glm::cos(glm::radians(12.5f)); 
		flashLight.outerCutOff = glm::cos(glm::radians(17.5f)); 

		DirectionalLight dirLight{}; 
		dirLight.direction = glm::vec3(0.f, -1.0f, 0.f);
		dirLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		dirLight.diffuse = glm::vec3( 1.0f, 0.4f, 0.4f);
		dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);



		// light properties
		cubeShader.use(); 
		cubeShader.setVec3("spotLight.ambient", flashLight.ambient);
		cubeShader.setVec3("spotLight.diffuse", flashLight.diffuse);
		cubeShader.setVec3("spotLight.specular", flashLight.specular);
		cubeShader.setFloat("spotLight.constant", flashLight.constant);
		cubeShader.setFloat("spotLight.linear", flashLight.linear);
		cubeShader.setFloat("spotLight.quadratic", flashLight.quadratic);
		cubeShader.setFloat("spotLight.cutOff", flashLight.cutOff);
		cubeShader.setFloat("spotLight.outerCutOff", flashLight.outerCutOff);

		cubeShader.setVec3("dirLight.ambient", dirLight.ambient);
		cubeShader.setVec3("dirLight.diffuse", dirLight.diffuse);
		cubeShader.setVec3("dirLight.specular", dirLight.specular);
		cubeShader.setVec3("dirLight.direction", dirLight.direction); 

		PointLight pointLight{}; 
		pointLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f); 
		pointLight.diffuse = glm::vec3(0.1f, 0.1f, 0.1f); 
		pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f); 
		pointLight.position = glm::vec3(0.f); 
		pointLight.constant = 1.0f; 
		pointLight.linear = 0.09f; 
		pointLight.quadratic = 0.032f; 

		cubeShader.use();

		glm::vec3 amb_vec = glm::vec3(0.05f, 0.05f, 0.05f);
		glm::vec3 diff_vec = glm::vec3(0.8f, 0.8f, 0.8f);
		glm::vec3 spec_vec = glm::vec3(1.f, 1.f, 1.f);


	while (!glfwWindowShouldClose(MainWindow.get()->getWindow()))
		{

			cubeShader.use();
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processInput(cam,deltaTime); 

			if (glfwGetKey(this->MainWindow.get()->getWindow(), GLFW_KEY_F) == GLFW_PRESS)
			{
				flashLight.ambient = glm::vec3(0.f); 
				flashLight.diffuse = glm::vec3(0.f);
				flashLight.specular = glm::vec3(0.f);

				cubeShader.setVec3("spotLight.ambient", flashLight.ambient);
				cubeShader.setVec3("spotLight.diffuse", flashLight.diffuse);
				cubeShader.setVec3("spotLight.specular", flashLight.specular);

			}
			else
			{
				flashLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				flashLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
				flashLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

				cubeShader.setVec3("spotLight.ambient", flashLight.ambient);
				cubeShader.setVec3("spotLight.diffuse", flashLight.diffuse);
				cubeShader.setVec3("spotLight.specular", flashLight.specular);
			}

			for (GLuint i = 0; i < 4; i++)
			{

				std::string number = std::to_string(i);
				std::string loc = "pointLights[" + number + "]"; 

				cubeShader.setVec3(loc + ".position", pointLightPositions[i]);
				cubeShader.setVec3(loc + ".ambient", amb_vec);
				cubeShader.setVec3(loc + ".diffuse", diff_vec);
				cubeShader.setVec3(loc + ".specular", spec_vec);

				cubeShader.setFloat(loc + ".constant", 1.0f);
				cubeShader.setFloat(loc + ".linear", 0.09f);
				cubeShader.setFloat(loc + ".quadratic", 0.032);

			}

			
			// render
			// ------
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = cam.GetViewMatrix();

			// bind textures on corresponding texture units
			cubeShader.setTexture(texture1, 0); 
			cubeShader.setTexture(texture2, 1); 
	

			// get matrix's uniform location and set matrix

			cubeShader.setVec3("spotLight.position", cam.Position);
			cubeShader.setVec3("spotLight.direction", cam.Front); 
			cubeShader.setVec3("viewPos", cam.Position);


			// material properties
			cubeShader.setFloat("material.shininess", 32.0f);

			// render container
			

			cubeShader.setMat4("projection", this->ProjectionMatrix); 
			cubeShader.setMat4("view", view); 


			glBindVertexArray(this->VAO);

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

			lightShader.use(); 
			view = cam.GetViewMatrix(); 
			lightShader.setMat4("view", view); 

			for (int i = 0; i < 4; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f));

				lightShader.setMat4("model", model);
				lightShader.setMat4("projection", this->ProjectionMatrix);
				lightCube.BindBuffers();
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glfwSwapBuffers(MainWindow.get()->getWindow());
			glfwPollEvents();
		}
	}

	

}
