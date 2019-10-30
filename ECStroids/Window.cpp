#include "Window.h"



Window::Window() {}


Window::~Window() {}

SDL_Renderer* Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
	Uint32 flags = SDL_WINDOW_OPENGL;
	if (currentFlags & INVISIBLE) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS) {
		flags |= SDL_WINDOW_BORDERLESS;
	}
	_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
	if (_sdlWindow == nullptr) {
		//fatalError("Window could not be created");
		return nullptr;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return renderer;
}

void Window::swapBuffer() {
	// SDL_GL_SwapWindow(_sdlWindow);
}
