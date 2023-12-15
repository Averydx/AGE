#pragma once
#include <glm/glm.hpp>
struct DirectionalLight
{
	glm::vec3 direction; 
	glm::vec3 ambient; 
	glm::vec3 diffuse; 
	glm::vec3 specular; 
};


struct PointLight {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};