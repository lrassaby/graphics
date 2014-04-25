#include "fountain.h"
#include <GL/glui.h>
#include <stdlib.h>

enum dims {X, Y, Z};

Fountain::Fountain()
{
    num_particles = 1000;    
}
Fountain::~Fountain(){}

Particle Fountain::createParticle()
{
    Particle new_particle;
    new_particle.lifetime = (float)(rand() % 500000) / 500000.0;
    new_particle.color.r = 0.7;
    new_particle.color.g = 0.7;
    new_particle.color.b = 1.0;
    new_particle.pos[X] = 0.0;
    new_particle.pos[Y] = 0.0;
    new_particle.pos[Z] = 0.0;
    new_particle.speed.xspeed = 0.0005 - (float)(rand() % 100) / 100000.0;
    new_particle.speed.yspeed = 0.01 - (float)(rand() % 100) / 100000.0;
    new_particle.speed.zspeed = 0.0005 - (float)(rand() % 100) / 100000.0;
    return new_particle;
}

void Fountain::drawParticles()
{

    GLuint billboard_vertex_buffer;
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        // The VBO containing the positions and sizes of the particles
    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, num_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, num_particles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


    int lastTime = glutGet(GLUT_ELAPSED_TIME);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    






    glBegin(GL_POINTS);
    //glBindTexture(GL_TEXTURE_2D,ParticleTexture);
    for (int i = 0; i <= this->num_particles; i++) {
        if (i >= particles.size()) {
            particles.push_back(createParticle());
        }
        if (particles[i].pos[Y] < 0.0) {
            particles[i].lifetime = 0.0;
        }
        if ((particles[i].active == true) && (particles[i].lifetime > 0.0)) {
            glColor3f(particles[i].color.r, particles[i].color.g, particles[i].color.b);
            glVertex3dv(particles[i].pos.unpack());

            /*
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.0, 1.0); glVertex3f(particles[i].pos[X] + 0.002, particles[i].pos[Y] + 0.002, particles[i].pos[Z] + 0.0);    // top    right
            glTexCoord2f(0.0, 0.0); glVertex3f(particles[i].pos[X] - 0.002, particles[i].pos[Y] + 0.002, particles[i].pos[Z] + 0.0);    // top    left
            glTexCoord2f(1.0, 1.0); glVertex3f(particles[i].pos[X] + 0.002, particles[i].pos[Y] - 0.002, particles[i].pos[Z] + 0.0);    // bottom right
            glTexCoord2f(1.0, 0.0); glVertex3f(particles[i].pos[X] - 0.002, particles[i].pos[Y] - 0.002, particles[i].pos[Z] + 0.0);    // bottom left
            glEnd();
            */
        }
        else {
            particles[i] = createParticle();
        }
    }
    evolveParticles();
    glEnd(); 
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
