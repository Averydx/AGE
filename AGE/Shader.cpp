#include "Shader.h"

AGE::Shader::Shader(std::string vertexFilePath, std::string fragmentFilePath)
{
	this->shaderID = LoadShaders(vertexFilePath.c_str(), fragmentFilePath.c_str()); 
}

void AGE::Shader::use()
{
	glUseProgram(shaderID);
}

void AGE::Shader::setMat4(std::string name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void AGE::Shader::setInt(std::string name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void AGE::Shader::setFloat(std::string name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void AGE::Shader::setTexture(unsigned int name, int textureUnit) const 
{
	switch (textureUnit)
	{
	case 0: 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, name);
		break; 
	case 1: 
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, name);
		break; 
	}
}

void AGE::Shader::setVec3(std::string name, glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(shaderID, name.c_str()),1, glm::value_ptr(value)); 
}




