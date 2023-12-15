#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
class Mesh
{
public:
	Mesh(std::vector<float> vertices); //Pass in the vertices this mesh will utilize
	void BindBuffers() const; //Bind the buffers contained in this class to be accessible via the VAO 
	glm::mat4 getModelMatrix() const; //Returns the model matrix 
	void setModelMatrix(glm::mat4 newModel); //set a new model matrix 

	//Rotate and scale the model matrix 
	void rotateModelMatrix(float angle, glm::vec3 axis); 
	void scaleModelMatrix(float scale);

	//TODO this doesn't seem to work 
	void translateModelMatrix(glm::vec3 translation); 

private: 
	unsigned int VBO; //A VBO to contain the vertex information
	glm::mat4 model; //The model matrix

};

