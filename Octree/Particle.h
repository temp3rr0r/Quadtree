#pragma once

class Particle {
public:
	Particle(double x, double y, double velocity_x, double velocity_y, double mass, double acceleration_x, double acceleration_y) :
		x_(x), y_(y), mass_(mass), velocity_x_(velocity_x), velocity_y_(velocity_y), acceleration_x_(acceleration_x), acceleration_y_(acceleration_y) {};

	void add_acceleration_pairwise(Particle& interacting_particle);
	void add_acceleration(const Particle& interacting_particle);
	void advance(double time_stamp);
	double x_;
	double y_;
	double mass_;
	double velocity_x_, velocity_y_;
	double acceleration_x_, acceleration_y_;

	Particle operator+(const Particle& r) const {
		return Particle(x_ + r.x_, y_ + r.y_, velocity_x_ + r.velocity_x_, velocity_y_ + r.velocity_y_, mass_ + r.mass_, 
			acceleration_x_ + r.acceleration_x_, acceleration_y_ + r.acceleration_y_);
	}

	Particle operator-(const Particle& r) const {
		return Particle(x_ - r.x_, y_ - r.y_, velocity_x_ - r.velocity_x_, velocity_y_ - r.velocity_y_, mass_ - r.mass_,
			acceleration_x_ - r.acceleration_x_, acceleration_y_ - r.acceleration_y_);
	}
};
