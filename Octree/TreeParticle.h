#pragma once
#include "Particle.h"


class TreeParticle {
	Particle particle;
public:
	TreeParticle(const Particle& input_particle) : particle(input_particle) { }
	const Particle& getPosition() const {
		return particle;
	}
	void setPosition(const Particle& input_particle) {
		particle = input_particle;
	}
};
