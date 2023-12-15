#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(std::vector<float> vertices)
{
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	model = glm::mat4(1.0f); 

}

void Mesh::BindBuffers() const
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
}

glm::mat4 Mesh::getModelMatrix() const
{
	return model;
}

void Mesh::setModelMatrix(glm::mat4 newModel)
{
	this->model = newModel; 
}

void Mesh::rotateModelMatrix(float angle, glm::vec3 axis)
{
	this->model = glm::rotate(this->model, glm::radians(angle), axis);
}

void Mesh::scaleModelMatrix(float scale)
{
	this->model = glm::scale(this->model, glm::vec3(scale)); 
}

void Mesh::translateModelMatrix(glm::vec3 translation)
{
	this->model = glm::translate(this->model, translation); 
}
