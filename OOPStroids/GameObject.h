#include <vector>
#include "SDL_image.h"

class GameObject {
public:
	GameObject(float X, float Y, float DX, float DY, int SIZEof) {
		x = X;
		y = Y;
		dx = DX;
		dy = DY;
		SIZE = SIZEof;
	}
private:
	float x; // coordinates for position
	float y;
	float dx; // coordinates for velocity
	float dy;
	int SIZE;

	//std::vector<GameObject> obj;
	// sprite

};