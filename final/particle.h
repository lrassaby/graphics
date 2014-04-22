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

struct Speed {
	union {
		struct {
			float xspeed, yspeed, zspeed;
		};
		float dimensions[3];
	};
};

class Particle {
	public:		
		Particle();
		Particle(float lifetime, Color color, Point pos, Speed speed, 
			bool active);
		~Particle();

		float lifetime; /* total lifetime of the particle */
		Color color;    /* color values of the particle */
		Point pos;		/* particle's position in 3-space */
		Speed speed;    /* particle's speed */
		//float angle;
		//float weight;
		bool active;    /* is particle active or not? */
};

#endif  /* particle.h */