#include "InputManager.h"

InputManager::InputManager() : _mouseCoords(0.0f) {}


InputManager::~InputManager() {}

void InputManager::pressKey(unsigned int keyID) {
	//_keyMap[keyID] = true;
	_pressedKeys.push_back(keyID);
}

void InputManager::releaseKey(unsigned int keyID) {
	//_keyMap[keyID] = false;
	_pressedKeys.remove(keyID);
}

//bool InputManager::isKeyPressed(unsigned int keyID) {
//	auto it = _keyMap.find(keyID);
//	if (it != _keyMap.end()) {
//		return it->second;
//	}
//	return false;
//	//return _keyMap[keyID];
//}
void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}