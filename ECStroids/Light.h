#pragma once
#include <glm/glm.hpp>



struct Light {
	Light(glm::vec3 col, float rad) : color(col), radius(rad), pos(0.5f, 0.5f) {}
	Light(glm::vec2 p, glm::vec3 col, float rad) : color(col), radius(rad), pos(p) {}
	glm::vec2 pos;
	glm::vec3 color;
	float radius;
};