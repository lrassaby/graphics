#include "particle_system.h"
#include <algorithm>

ParticleSystem::ParticleSystem()
{
    num_particles = 1000;
}

void ParticleSystem::drawParticles() {}

void ParticleSystem::SortParticles(){
    std::sort(&particles[0], &particles[num_particles]);
}