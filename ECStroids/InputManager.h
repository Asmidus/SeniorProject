#pragma once
#include <unordered_map>
#include <list>
#include <glm/glm.hpp>

class InputManager {
public:
	InputManager();
	~InputManager();
		
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	//bool isKeyPressed(unsigned int keyID);
	void setMouseCoords(float x, float y);
	glm::vec2 getMouseCoords() const { return _mouseCoords; }
	const std::list<unsigned int>& getPressedKeys() { return _pressedKeys; }

private:
	//std::unordered_map<unsigned int, bool> _keyMap;
	std::list<unsigned int> _pressedKeys;
	glm::vec2 _mouseCoords;
};