#include "particle.h"

Particle::Particle()
{
	lifetime = 0.0;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 1;
	speed.xspeed = 0;
	speed.yspeed = 0;
	speed.zspeed = 0;
	pos = Point();
}

Particle::Particle(float lifetime, Color color, Point pos, Speed speed, 
	bool active)
{
	this->lifetime = lifetime;
	this->color = color;
	this->speed = speed;
	this->pos = pos;
}

Particle::~Particle() {}