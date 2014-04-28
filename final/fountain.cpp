#include "fountain.h"
#include <GL/glui.h>
#include <stdlib.h>

Fountain::Fountain()
{
    max_particles = 1000;    
    vertex_shader = "particle.vert"; 
    fragment_shader = "particle.frag"; 
    spread = 1.0f; 
}

Fountain::~Fountain(){}

void Fountain::createNewParticles()
{
    int newparticles = elapsed * 10;
    if (newparticles > 160) { 
        newparticles = 160;
    }
    for (int i = 0; i < newparticles; i++) {
        int particle_index = findUnusedParticle();
        particles[particle_index].lifetime = 5.0f;
        particles[particle_index].pos = Point(0, 0, -5.0);

        Vector main_direction(0.0, 10.0, 0.0);
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
            fprintf(stderr, "elapsed %d\n", elapsed);
            p->lifetime -= elapsed / 100;
            if (p->lifetime > 0.0f) {
                p->speed = p->speed + (gravity * (elapsed * 0.1f));
                fprintf(stderr, "speed: %f\n", p->speed[Y]);
                p->pos = p->pos + (p->speed * elapsed / 100);
                p->cameradistance = length((p->pos - camera_position));
                fprintf(stderr, " inside loop %d pos: %f %f %f \n", i, p->pos[X], p->pos[Y], p->pos[Z]);
                //setGPUBuffers(p, active_particles);
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
