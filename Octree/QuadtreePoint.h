#pragma once

#include "Vec2.h"

class QuadtreePoint {
	Vec2 position;
public:
	QuadtreePoint() { }
	QuadtreePoint(const Vec2& position) : position(position) { }
	const Vec2& getPosition() const {
		return position;
	}
	void setPosition(const Vec2& p) {
		position = p;
	}
};