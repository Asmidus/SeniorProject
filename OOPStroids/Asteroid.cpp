#include "GameObject.h"
#include "Asteroid.h"
#include <vector>
#include <iostream>

SDL_Texture* asteroidSprite;



void Asteroid::drawAsteroid() {

	/*for (auto& asteroid : a) {
		asteroid.x += asteroid.dx * timePassed;
		asteroid.y += asteroid.dy * timePassed;

		SDL_Surface* temp = IMG_Load("media/Projectile.png");
		
	}*/
}

void Asteroid::transformAsteroid(std::vector<GameObject> a) { // ???

}

// ***not called in onuser update
void Asteroid::createAsteroid(std::vector<GameObject> a) {
	// {x, y, dx, dy, SIZE}
	float timePassed = 5;
	a.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16});

	Asteroid::drawAsteroid(a, timePassed);

}
