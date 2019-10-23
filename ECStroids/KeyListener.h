#pragma once
#include <SDL.h>
#include <unordered_map>
#include "Event.h"

struct KeyListener {
	bool active = true;
	std::unordered_map<unsigned int, Event::Type> map;
};