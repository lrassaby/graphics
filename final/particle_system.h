#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include <GL/glut.h>
#include "particle.h"

static const GLfloat g_vertex_buffer_data[] = { 
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
     -0.5f,  0.5f, 0.0f,
      0.5f,  0.5f, 0.0f,
};

class ParticleSystem {
	public:
		ParticleSystem();
		~ParticleSystem(){};

		virtual void drawParticles();
        void SortParticles();
		int num_particles;

    protected:
        std::vector<Particle> particles;
};

#endif /* particle_system.h */