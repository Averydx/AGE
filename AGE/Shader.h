#pragma once
#include "LoadShader.h"
#include <stdint.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace AGE
{
	class Shader
	{
	private: 
		//The ID of the program
		uint32_t shaderID; 

	public: 
		Shader(std::string vertexFilePath, std::string fragmentFilePath); 
		void use();
		void setVec3(std::string name,glm::vec3& value) const; 
		void setMat4(std::string name,glm::mat4& value) const; 
		void setInt(std::string name, int value) const; 
		void setFloat(std::string name, float value) const; 
		void setTexture(unsigned int name, int textureUnit) const;

	};

}

