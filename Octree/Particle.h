#pragma once

class Particle {
public:
	Particle() {
		x_ = 0.0;
		y_ = 0.0;
		velocity_x_ = 0.0;
		velocity_y_ = 0.0;
		mass_ = 0.0;
		acceleration_x_ = 0.0;
		acceleration_y_ = 0.0;
	}
	Particle(float x, float y, float velocity_x, float velocity_y, float mass, float acceleration_x, float acceleration_y) :
		x_(x), y_(y), mass_(mass), velocity_x_(velocity_x), velocity_y_(velocity_y), acceleration_x_(acceleration_x), acceleration_y_(acceleration_y) {};

	inline void add_acceleration_pairwise(Particle& interacting_particle);
	inline void add_acceleration(const Particle& interacting_particle);
	inline void advance(float time_stamp);
	
	float x_;
	float y_;
	float mass_;
	float velocity_x_, velocity_y_;
	float acceleration_x_, acceleration_y_;

	Particle operator+(const Particle& r) const {
		return Particle(x_ + r.x_, y_ + r.y_, velocity_x_ + r.velocity_x_, velocity_y_ + r.velocity_y_, mass_ + r.mass_, 
			acceleration_x_ + r.acceleration_x_, acceleration_y_ + r.acceleration_y_);
	}

	Particle operator-(const Particle& r) const {
		return Particle(x_ - r.x_, y_ - r.y_, velocity_x_ - r.velocity_x_, velocity_y_ - r.velocity_y_, mass_ - r.mass_,
			acceleration_x_ - r.acceleration_x_, acceleration_y_ - r.acceleration_y_);
	}

	Particle operator*(float r) const {
		return Particle(x_ * r, y_ * r, velocity_x_ * r, velocity_y_ * r, mass_ * r, acceleration_x_ * r, acceleration_y_ * r);
	}
};
