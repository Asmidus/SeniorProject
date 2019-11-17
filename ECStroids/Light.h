#pragma once
#include <glm/glm.hpp>



struct Light {
	Light(glm::vec3 col, float rad) : color(col, 255), radius(rad), pos(0.5f, 0.5f) {
		auto [fbo, tex] = TextureManager::CreateRenderTexture(radius * 2, radius * 2);
		shadowFBO = fbo;
		shadowTex = tex;
	}
	//Light(glm::vec2 p, glm::vec3 col, float rad) : color(col, 255), radius(rad), pos(p) {}
	glm::vec2 pos;
	glm::vec4 color;
	float radius;
	GLuint shadowFBO;
	GLuint shadowTex;
};