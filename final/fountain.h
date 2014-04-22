#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include "particle_system.h"

class Fountain : public ParticleSystem {
	public:
		Fountain();
		~Fountain();
		void drawParticles();

	private:
		Particle createParticle();
		void evolveParticles();
};

#endif /* fountain.h */