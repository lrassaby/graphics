#include "fountain.h"
#include <GL/glui.h>
#include <stdlib.h>

enum dims {X, Y, Z};

Fountain::Fountain(){}
Fountain::~Fountain(){}

void Fountain::createParticle(int i)
{
    particles[i].lifetime = (float)(rand() % 500000) / 500000.0;
    particles[i].color.r = 0.7;
    particles[i].color.g = 0.7;
    particles[i].color.b = 1.0;
    particles[i].pos[X] = 0.0;
    particles[i].pos[Y] = 0.0;
    particles[i].pos[Z] = 0.0;
    particles[i].speed.xspeed = 0.0005 - (float)(rand() % 100) / 100000.0;
    particles[i].speed.yspeed = 0.01 - (float)(rand() % 100) / 100000.0;
    particles[i].speed.zspeed = 0.0005 - (float)(rand() % 100) / 100000.0;
    particles[i].active = true;
}

void Fountain::drawParticles()
{
    glLoadIdentity();
    glRotatef(50.0, 1.0, 0.0, 0.0);

    //glBindTexture(GL_TEXTURE_2D,ParticleTexture);
    for (int i = 0; i <= this->num_particles; i++) {
        if (particles[i].pos[Y] < 0.0) {
            particles[i].lifetime = 0.0;
        }
        if ((particles[i].active == true) && (particles[i].lifetime > 0.0)) {
            glColor3f(particles[i].color.r, particles[i].color.g, particles[i].color.b);
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0, 1.0); glVertex3f(particles[i].pos[X] + 0.002, particles[i].pos[Y] + 0.002, particles[i].pos[Z] + 0.0);    // top    right
            glTexCoord2f(0.0, 0.0); glVertex3f(particles[i].pos[X] - 0.002, particles[i].pos[Y] + 0.002, particles[i].pos[Z] + 0.0);    // top    left
            glTexCoord2f(1.0, 1.0); glVertex3f(particles[i].pos[X] + 0.002, particles[i].pos[Y] - 0.002, particles[i].pos[Z] + 0.0);    // bottom right
            glTexCoord2f(1.0, 0.0); glVertex3f(particles[i].pos[X] - 0.002, particles[i].pos[Y] - 0.002, particles[i].pos[Z] + 0.0);    // bottom left
            glEnd();
        }
        else createParticle(i);
    }
    evolveParticles();
}

void Fountain::evolveParticles()
{
    for (int i = 0; i <= this->num_particles; i++) {
        particles[i].lifetime -= DECAY;
        particles[i].pos[X] += particles[i].speed.xspeed;
        particles[i].pos[Y] += particles[i].speed.yspeed;
        particles[i].pos[Z] += particles[i].speed.zspeed;
        particles[i].speed.yspeed -= 0.00007;
    }

}