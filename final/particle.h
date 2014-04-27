#ifndef PARTICLE_H
#define PARTICLE_H

#include "Algebra.h"
#define DECAY 0.001

struct Color {
	union {
		struct {
			float r, g, b, a;
		};
		float channels[4];
	};
};

class Particle {
	public:		
		Particle();
		Particle(float lifetime, Color color, Point pos, Vector speed, 
			bool active);
		~Particle();

		float lifetime; /* total lifetime of the particle */
		Color color;    /* color values of the particle */
		Point pos;		/* particle's position in 3-space */
		Vector speed;    /* particle's speed */
		float angle;
		float weight;
		float size;
		float cameradistance; /* *Squared* distance to the camera. if dead : -1.0f */
		bool operator<(const Particle& that) const {
			return this->cameradistance > that.cameradistance;
		}
};

#endif  /* particle.h */