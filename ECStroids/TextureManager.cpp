#include "TextureManager.h"
#include "lodepng.h"
#include <iostream>



static std::unordered_map<const char*, std::tuple<unsigned int, unsigned int, unsigned int>> _textures;

std::tuple<unsigned int, unsigned int, unsigned int> TextureManager::LoadTexture(const char* fileName) {
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Generate the mip maps
		glGenerateMipmap(GL_TEXTURE_2D);

		//Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		_textures[fileName] = std::make_tuple(texture, width, height);
	}
	return _textures[fileName];
}

SDL_Texture* TextureManager::LoadText(const char* text, TTF_Font* font, SDL_Color color) {
	auto texSurface = TTF_RenderText_Blended(font, text, color);
	//auto tex = SDL_CreateTextureFromSurface(_renderer, texSurface);
	SDL_FreeSurface(texSurface);
	return nullptr;
}