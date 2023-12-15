#include "window.h"
#include <stdexcept>

namespace AGE
{
	int Window::getWidth()
	{
		return this->width;
	}
	int Window::getHeight()
	{
		return this->height;
	}
	GLFWwindow* Window::getWindow()
	{
		return this->window;
	}
	Window::Window(int width, int height, std::string name) :width(width), height(height), name(name)
	{

		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL); 
		if(window == NULL){
			glfwTerminate();
			throw std::runtime_error("Window failed to initialize");
		}


		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}


	}

}