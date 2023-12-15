#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
namespace AGE
{
	class Window
	{

	private: 
		std::string name;
		int width, height;
		GLFWwindow* window; 

	public: 
		int getWidth(); 
		int getHeight();
		GLFWwindow* getWindow(); 
		Window(int width, int height, std::string name);
	};
}

