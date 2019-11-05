#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>



enum WindowFlags {
	INVISIBLE = 0x1,
	FULLSCREEN = 0x2,
	BORDERLESS = 0x4
};

class Window {
public:
	Window();
	~Window();

	SDL_Renderer* create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
	void swapBuffer();
	SDL_Window* get() { return _sdlWindow; }
	int getScreenWidth() { return _screenWidth; }
	int getScreenHeight() { return _screenHeight; }
private:
	SDL_Window* _sdlWindow;
	int _screenWidth, _screenHeight;
};