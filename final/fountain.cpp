#include "fountain.h"
#include <GL/glui.h>
#include <stdlib.h>

Fountain::Fountain()
{
    max_particles = 10000;
    vertex_shader = "particle.vert"; 
    fragment_shader = "particle.frag"; 
    spread = 1.0f; 
    radius = 1.5f;
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

        Vector main_direction(0.0, 12.0, 0.0);
        Vector rand_direction = getRandVector();

        particles[particle_index].speed = main_direction + rand_direction * spread;
        /* TODO: generate random colors */
        particles[particle_index].color.r = (rand() / (float)RAND_MAX);
        particles[particle_index].color.g = rand() / (float)RAND_MAX;
        particles[particle_index].color.b = rand() / (float)RAND_MAX;
        particles[particle_index].color.a = rand() / (float)RAND_MAX; 

        particles[particle_index].size = (rand() % 1000) / 2000.0f + 0.1f;
    }
}

void Fountain::computeParticles()
{
    active_particles = 0;
    Vector gravity(0.0f, -9.81f, 0.0f);

    createNewParticles();
    //glBegin(GL_POINTS); 
    for (int i = 0; i < max_particles; i++) {
        Particle *p = &particles[i];

        if (p->lifetime > 0.0f) {
        //glColor3f(p->color.r, p->color.g, p->color.b);
            p->lifetime -= elapsed;
            if (p->lifetime > 0.0f) {
                p->speed = p->speed + (gravity * elapsed);
                p->pos = p->pos + (p->speed * elapsed / 10);
                p->cameradistance = length((p->pos - camera_position));
                setGPUBuffers(p, active_particles);
            } else {
                p->cameradistance = -1.0f; /* particle has just died */
            }
            //glVertex3dv(particles[i].pos.unpack());
            active_particles++;
        }
    }
    //glEnd();
}
