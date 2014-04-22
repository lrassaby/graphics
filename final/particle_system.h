#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include "particle.h"

class ParticleSystem {
	public:
		ParticleSystem();
		~ParticleSystem(){};

		virtual void drawParticles();

		std::vector<Particle> particles;
		int num_particles;
};

#endif /* particle_system.h */