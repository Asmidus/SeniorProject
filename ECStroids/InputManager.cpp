#include "InputManager.h"
#include <algorithm>



InputManager::InputManager() : _mouseCoords(0.0f) {}

InputManager::~InputManager() {}

void InputManager::pressKey(unsigned int keyID) {
	if (std::find(_pressedKeys.begin(), _pressedKeys.end(), keyID) == _pressedKeys.end()) {
		_pressedKeys.push_back(keyID);
	}
}

void InputManager::pressButton(unsigned int buttonID) {
	if (std::find(_pressedMouseButtons.begin(), _pressedMouseButtons.end(), buttonID) == _pressedMouseButtons.end()) {
		_pressedMouseButtons.push_back(buttonID);
	}
}

void InputManager::releaseKey(unsigned int keyID) {
	_pressedKeys.remove(keyID);
	_releasedKeys.push_back(keyID);
}

void InputManager::releaseButton(unsigned int buttonID) {
	_pressedMouseButtons.remove(buttonID);
	_releasedMouseButtons.push_back(buttonID);
}

void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}