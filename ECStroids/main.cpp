#include <iostream>
#include "MainGame.h"
#include <glm/glm.hpp>
#include <SDL.h>

int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();
	//std::cout << sizeof(glm::vec4) << " vs " << sizeof(SDL_FRect) << "\n";

	return 0;
}