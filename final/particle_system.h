#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include <string>
#include <GL/glut.h>
#include "particle.h"

static const GLfloat g_vertex_buffer_data[] ={
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    0.5f,  0.5f, 0.0f,
};

class ParticleSystem
{
public:
    int max_particles;        /* total number of particles */

    ParticleSystem();
    ~ParticleSystem() {};
    virtual void drawParticles();

protected:
    std::vector<Particle> particles;
    std::string vertex_shader;   /* shader set by child class */
    std::string fragment_shader; /* shader set by child class */
    int active_particles;        /* subset of particles that are currently active */

    int findUnusedParticle();
    void SortParticles();
    void bindShaders();

private:
    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    GLfloat *position_size_data;
    GLubyte *color_data;
    GLuint programID;
    GLuint CameraRight_worldspace_ID;
    GLuint CameraUp_worldspace_ID;
    GLuint ViewProjMatrixID;

    int last_used_particle;
    int last_time;
};

#endif /* particle_system.h */