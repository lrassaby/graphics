#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include "particle_system.h"

class Fountain : public ParticleSystem {
	public:
		Fountain();
		~Fountain();
		void drawParticles();

	private:
		void createParticle(int i);
		void evolveParticles();
};

#endif /* fountain.h */