#pragma once


class Bullet {
private:
	bool tracking = false;
	float angle;
	float rotatedX;
	float rotatedY;

public:
	Bullet(Bullet bullet, bool tracking) {
		static unsigned int bulletSize = 12.5;
		auto angle = Player.angle * (3.14159 / 180);
		//TODO
		//rotatedX = 
		//rotatedY =

	}
};