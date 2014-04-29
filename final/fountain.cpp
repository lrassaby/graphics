#include "fountain.h"
#include <GL/glui.h>
#include <stdlib.h>


Fountain::Fountain()
{
    max_particles = 10000;
    vertex_shader = "shaders/particle.vert"; 
    fragment_shader = "shaders/particle.frag"; 
    texture_file = "shaders/particle.dds";
    spread = 1.0f; 
    particle_size = 10;
    radius = 1.5f;
    main_direction = Vector(0.0, 12.0, 0.0);
    gravity = Vector(0.0f, -9.81f, 0.0f);
}

Fountain::~Fountain(){}

void Fountain::createNewParticles()
{
    int newparticles = elapsed * 1000;
    if (newparticles > max_particles / 2.0) { 
        newparticles = max_particles / 2.0;
    }
    for (int i = 0; i < newparticles; i++) {
        int particle_index = findDeadParticle();
        particles[particle_index].lifetime = 5.0f;
        particles[particle_index].pos = Point(0, 0, 0);

        Vector rand_direction = getRandVector();

        particles[particle_index].speed = main_direction + rand_direction * spread;
        /* TODO: generate random colors */
        particles[particle_index].color.r = (rand() / (float)RAND_MAX);
        particles[particle_index].color.g = rand() / (float)RAND_MAX;
        particles[particle_index].color.b = rand() / (float)RAND_MAX;
        particles[particle_index].color.a = rand() / (float)RAND_MAX; 

        particles[particle_index].size = (rand() % particle_size) / 200.0f + 0.01f;
    }
}

void Fountain::computeParticles()
{
    active_particles = 0;

    createNewParticles();
    for (int i = 0; i < max_particles; i++) {
        Particle *p = &particles[i];

        if (p->lifetime > 0.0f) {
            p->lifetime -= elapsed;
            if (p->lifetime > 0.0f) {
                p->speed = p->speed + (gravity * elapsed);
                p->pos = p->pos + (p->speed * elapsed / 10);
                p->cameradistance = length((p->pos - camera_position));
                //fprintf(stderr, "cameradistance %f\n", p->cameradistance);
                setGPUBuffers(p, active_particles);
            } else {
                p->cameradistance = -1.0f; /* particle has just died */
            }
            active_particles++;
        }
    }
}
