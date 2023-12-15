#pragma once
#include <glad/glad.h>
#include "window.h"
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Camera.h"
namespace AGE
{
	class Engine
	{
	private: 

		std::unique_ptr<Window> MainWindow; //A pointer to a wrapper for a GLFW window
		void Init();
		void processInput(Camera& cam, float deltaTime); //Process keyboard keypresses given time since last frame and the camera 

		glm::mat4 ProjectionMatrix; //Global projection matrix to be applied to every vertex 
		unsigned int VAO; //Single VAO 
		static void mouse_callback(GLFWwindow* win, double xpos, double ypos); //Mouse callback function 

	public: 
		Engine(int window_width, int window_height, std::string window_name);
		void Run(); 
	};
}

