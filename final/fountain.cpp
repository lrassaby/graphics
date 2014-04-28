#include "fountain.h"
#include <GL/glui.h>
#include <stdlib.h>

Fountain::Fountain()
{
    max_particles = 10000;
    vertex_shader = "particle.vert"; 
    fragment_shader = "particle.frag"; 
    spread = 1.0f; 
}

Fountain::~Fountain(){}

void Fountain::createNewParticles()
{
    int newparticles = elapsed * 100;
    if (newparticles > 1600) { 
        newparticles = 1600;
    }
    for (int i = 0; i < newparticles; i++) {
        int particle_index = findUnusedParticle();
        //fprintf(stdout, "index %d\n", particle_index);
        particles[particle_index].lifetime = 5.0f;
        particles[particle_index].pos = Point(0, 0, 0);

        Vector main_direction(0.0, 12.0, 0.0);
        Vector rand_direction = getRandVector();

        particles[particle_index].speed = main_direction + rand_direction * spread;
        /* TODO: generate random colors */
        particles[particle_index].color.r = rand() % 256;
        particles[particle_index].color.g = rand() % 256;
        particles[particle_index].color.b = rand() % 256;
        particles[particle_index].color.a = rand() % 80;

        particles[particle_index].size = (rand() % 1000) / 2000.0f + 0.1f;
    }
}

void Fountain::computeParticles()
{
    active_particles = 0;
    Vector gravity(0.0f, -9.81f, 0.0f);

    createNewParticles();
    glBegin(GL_POINTS); 
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < max_particles; i++) {
        Particle *p = &particles[i];
        //fprintf(stderr, "before %d pos: %f %f %f \n", i, p->pos[X], p->pos[Y], p->pos[Z]);

        if (p->lifetime > 0.0f) {
            //fprintf(stderr, "elapsed %d\n", elapsed);
            p->lifetime -= elapsed;
            //fprintf(stderr, "index %d lifetime %f\n", i, p->lifetime);
            if (p->lifetime > 0.0f) {
                p->speed = p->speed + (gravity * elapsed);
                //fprintf(stderr, "speed: %f\n", p->speed[Y]);
                p->pos = p->pos + (p->speed * elapsed / 10);
                p->cameradistance = length((p->pos - camera_position));
                //fprintf(stderr, " inside loop %d pos: %f %f %f \n", i, p->pos[X], p->pos[Y], p->pos[Z]);
                setGPUBuffers(p, active_particles);
            } else {
                p->cameradistance = -1.0f; /* particle has just died */
            }
            //fprintf(stderr, "%f %f %f \n", p->pos[X], p->pos[Y], p->pos[Z]);
            glVertex3dv(particles[i].pos.unpack());
            active_particles++;
        }
    }
    glEnd();
}
