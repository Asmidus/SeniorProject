#include "TextureManager.h"
#include "lodepng.h"
#include <iostream>
#include <SDL_image.h>
#include "Vertex.h"

static SDL_Renderer* _renderer = nullptr;
static std::unordered_map<const char*, GLuint> _textures;
static GLuint _vao = 0;
static GLuint _vbo = 0;
static GLuint texLoc = 0;

void TextureManager::init(GLuint tex) {
	texLoc = tex;
	// Generate the VAO if it isn't already generated
	if (_vao == 0) {
		glGenVertexArrays(1, &_vao);
	}

	// Bind the VAO. All subsequent opengl calls will modify it's state.
	glBindVertexArray(_vao);

	//G enerate the VBO if it isn't already generated
	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	//Tell opengl what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glBindVertexArray(0);
}

unsigned int TextureManager::LoadTexture(const char* fileName) {
	if (_textures.find(fileName) == _textures.end()) {
		std::vector<unsigned char> image;
		unsigned width, height;
		if (lodepng::decode(image, width, height, fileName)) {
			std::cout << "Texture " << fileName << " failed to load\n";
		}
		GLuint texture;
		//Generate the openGL texture object
		glGenTextures(1, &(texture));

		//Bind the texture object
		glBindTexture(GL_TEXTURE_2D, texture);
		//Upload the pixels to the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image[0]));

		//Set some texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//Generate the mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		//Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		_textures[fileName] = texture;
	}
	return _textures[fileName];
}

SDL_Texture* TextureManager::LoadText(const char* text, TTF_Font* font, SDL_Color color) {
	auto texSurface = TTF_RenderText_Blended(font, text, color);
	auto tex = SDL_CreateTextureFromSurface(_renderer, texSurface);
	SDL_FreeSurface(texSurface);
	return tex;
}

void TextureManager::Draw(unsigned int texture, SDL_Rect src, SDL_Rect dest) {
	//SDL_RenderCopy(_renderer, texture, &src, &dest);
}

void TextureManager::DrawText(SDL_Texture* texture, SDL_Rect dest) {
	//SDL_RenderCopy(_renderer, texture, nullptr, &dest);
}

void TextureManager::Draw(unsigned int texture, SDL_Rect src, SDL_FRect dest, SDL_FPoint* center, double angle, glm::vec3 rgb) {
	//SDL_SetTextureColorMod(_textures[texture], rgb.r, rgb.g, rgb.b);
	//SDL_RenderCopyExF(_renderer, _textures[texture], &src, &dest, angle, center, SDL_FLIP_NONE);

	// Draw code goes here
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Make sure the shader uses texture 0
	glUniform1i(texLoc, 0);

	// Grab the camera matrix
	//glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	//glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//glm::vec2 pos = m_camera.convertScreenToWorld(m_inputManager.getMouseCoords());
	std::vector<Vertex> vertices = {
		Vertex(glm::vec2(0, 0), glm::vec2(0, 0)),
		Vertex(glm::vec2(0, 1), glm::vec2(0, 1)),
		Vertex(glm::vec2(1, 1), glm::vec2(1, 1)),
		Vertex(glm::vec2(1, 1), glm::vec2(1, 1)),
		Vertex(glm::vec2(1, 0), glm::vec2(1, 0)),
		Vertex(glm::vec2(0, 0), glm::vec2(0, 0))
	};
	//glBindVertexArray(0);
	// Bind our VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Orphan the buffer (for speed)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
	// Upload the data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STREAM_DRAW);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_vao);


	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//vertices.clear();

	glBindVertexArray(0);
}