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
    float spread;             /* controls randomness spread */
    int m_max_particles;        /* total number of particles */
    int particle_size;
    float gravity_y;
    float particle_direction[3];

    ParticleSystem();
    void initialize(); 
    ~ParticleSystem();
    void drawParticles();

protected:
    Vector gravity;
    Vector main_direction;
    
    std::vector<Particle> particles;
    std::string vertex_shader;   /* shader set by child class */
    std::string fragment_shader; /* shader set by child class */
    std::string texture_file;
    
    int max_particles;
    int active_particles;        /* subset of particles that are currently active */
    int last_time;
    float elapsed;
    float radius;
    Point camera_position;

    Matrix model_view;
    Matrix projection;
    GLfloat model_projection[16];

    int findDeadParticle();
    void sortParticles();
    void bindShaders();
    void setGPUBuffers(Particle *particle, int particle_index);
    Vector getRandVector();
    Color getRandColor();
    virtual void computeParticles(){};

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
    GLuint texture;
    GLuint texture_ID;
    GLuint squareVerticesID;
    GLuint xyzsID;
    GLuint colorID;
    int last_used_particle;

    void getCameraMatrices();
};

#endif /* particle_system.h */