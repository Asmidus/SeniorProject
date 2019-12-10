#include "GameObject.h"
//#include <glm/gtx/rotate_vector.hpp>
#include <vector>

class Asteroid : public GameObject {
public:
	Asteroid(float x, float y, float dx, float dy, int SIZE) : GameObject(x, y, dx, dy, SIZE) { };
	void drawAsteroid(std::vector<GameObject> a, float timePassed);
	void transformAsteroid(std::vector<GameObject> a); // ???
	void createAsteroid(std::vector<GameObject> a); // sprite?


private:
	std::vector<GameObject> asteroid;

};