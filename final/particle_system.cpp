#include "particle_system.h"
#include "shader.h"
#include <stdio.h>
#include <algorithm>
#include "ppm.h"

#include "common/texture.hpp"
#include "common/shader.hpp"

ParticleSystem::ParticleSystem()
{
    initialized = false;
    m_max_particles = 10000;
    max_particles = m_max_particles;
    last_used_particle = 0;
    spread = 1.0f; 
    radius = 1.5f;
    srand (time(NULL));
}

ParticleSystem::~ParticleSystem() 
{
    cleanup();
    particles.clear();
    if (initialized && system_type != POINTS) {
        delete [] position_size_data;
        delete [] color_data;
    }
}

void ParticleSystem::initialize()
{
    initialized = true;
    Shader manager;

    /* sync data */
    gravity_y = gravity[Y];
    particle_direction[X] = main_direction[X];
    particle_direction[Y] = main_direction[Y];
    particle_direction[Z] = main_direction[Z];

    /* TODO: set up camera position...to set in particle system */
    particles.resize(max_particles);
    getCameraMatrices();

    if (system_type != POINTS) {
        position_size_data = new GLfloat[max_particles * 4];
        color_data = new GLubyte[max_particles * 4];

        // Create and compile our GLSL program from the shaders
        programID = manager.loadShader(vertex_shader.c_str(), fragment_shader.c_str());

        // Vertex shader
        CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
        CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
        ViewProjMatrixID = glGetUniformLocation(programID, "VP");

        // fragment shader
        texture_ID  = glGetUniformLocation(programID, "myTextureSampler");

        /* get handle for common buffers */
        squareVerticesID = glGetAttribLocation(programID, "squareVertices");
        xyzsID = glGetAttribLocation(programID, "xyzs");
        colorID = glGetAttribLocation(programID, "color");

        /* load dds files */
        if (system_type == DDS){
            texture = loadDDS(texture_file.c_str());
        }

        /* load ppm files and initialize extra age buffer */
        else if (system_type == IMAGE) {
            age_data = new GLubyte[max_particles];
            ageID = glGetAttribLocation(programID, "age");
            ppm image(texture_file);
            texture = image.createAsTexture();
            glGenBuffers(1, &particles_age_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, particles_age_buffer);
            // Initialize with empty (NULL) buffer : it will be updated later, each frame.
            glBufferData(GL_ARRAY_BUFFER, max_particles * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
        }

        glGenBuffers(1, &billboard_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        // The VBO containing the positions and sizes of the particles
        glGenBuffers(1, &particles_position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        // Initialize with empty (NULL) buffer : it will be updated later, each frame.
        glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

        // The VBO containing the colors of the particles
        glGenBuffers(1, &particles_color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        // Initialize with empty (NULL) buffer : it will be updated later, each frame.
        glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    }

    last_time = glutGet(GLUT_ELAPSED_TIME);
}

void ParticleSystem::setGPUBuffers(Particle *particle, int particle_index)
{
    int i = 4 * particle_index;
    position_size_data[i + X] = particle->pos[X];
    position_size_data[i + Y] = particle->pos[Y];
    position_size_data[i + Z] = particle->pos[Z];
    position_size_data[i + SIZE] = particle->size;

    color_data[i + R] = particle->color.r * 255;
    color_data[i + G] = particle->color.g * 255;
    color_data[i + B] = particle->color.b * 255;
    color_data[i + A] = particle->color.a * 255;

    if (system_type == IMAGE) {
        int age_data_byte = 63 - (63.0f * particle->lifetime) / 5;
        if (age_data_byte > 63) age_data_byte = 63;
        if (age_data_byte < 0) age_data_byte = 0;
        age_data[particle_index] = age_data_byte;
    }
}

void ParticleSystem::drawParticles() 
{
    gravity = Vector(0.0, gravity_y, 0.0);
    main_direction = Vector(particle_direction[X], particle_direction[Y], particle_direction[Z]);
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed = (current_time - last_time) * 0.001f;
    last_time = current_time;

    getCameraMatrices();

    if (max_particles != m_max_particles) {
        max_particles = m_max_particles;
        particles.resize(max_particles);

        delete [] position_size_data;
        delete [] color_data;
        position_size_data = new GLfloat[max_particles * 4];
        color_data = new GLubyte[max_particles * 4];

        if (system_type == IMAGE) {
            delete [] age_data;
            age_data = new GLubyte[max_particles];
        }
    }
    computeParticles();
    if (system_type != POINTS) {
        sortParticles();
        bindShaders();
    }
}

/*
 * findUnusedParticle - returns the index of the first dead particle 
 * (i.e. life < 0) in the particles vector.
 */
 int ParticleSystem::findDeadParticle()
 {
    for(int i = last_used_particle; i < max_particles; i++) {
        if (particles[i].lifetime < 0) {
            last_used_particle = i;
            return i;
        }
    }
    for(int i = 0; i < last_used_particle; i++) {
    	if (particles[i].lifetime <= 0) {
                last_used_particle = i;
            return i;
        }
    }
    return 0; // All particles are taken, override the first one
}

void ParticleSystem::sortParticles()
{
    std::sort(&particles[0], &particles[max_particles]);
}

void ParticleSystem::bindShaders()
{
    /* Update the buffers that OpenGL uses for rendering */
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, active_particles * sizeof(GLfloat) * 4, position_size_data);

    if (system_type == IMAGE) {
        glBindBuffer(GL_ARRAY_BUFFER, particles_age_buffer);
        glBufferData(GL_ARRAY_BUFFER, max_particles * sizeof(GLbyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, active_particles * sizeof(GLbyte), age_data);
    }


    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, active_particles * sizeof(GLubyte) * 4, color_data);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Use our shader
    glUseProgram(programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(texture_ID, 0);

    // Same as the billboards tutorial
    glUniform3f(CameraRight_worldspace_ID, model_view[0][0], model_view[1][0], model_view[2][0]);
    glUniform3f(CameraUp_worldspace_ID, model_view[0][1], model_view[1][1], model_view[2][1]);

    glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &model_projection[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(squareVerticesID);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(
        squareVerticesID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void *)0           // array buffer offset
    );


    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(xyzsID);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(
        xyzsID,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : x + y + z + size => 4
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void *)0                         // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(colorID);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(
        colorID,                          // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : r + g + b + a => 4
        GL_UNSIGNED_BYTE,                 // type
        GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0,                                // stride
        (void *)0                         // array buffer offset
    );

    if (system_type == IMAGE){
        glEnableVertexAttribArray(ageID);
        glBindBuffer(GL_ARRAY_BUFFER, particles_age_buffer);
        glVertexAttribPointer(
            ageID,                          // attribute. No particular reason for 1, but must match the layout in the shader.
            1,                                // size : r + g + b + a => 4
            GL_UNSIGNED_BYTE,                 // type
            GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
            0,                                // stride
            (void *)0                         // array buffer offset
        );
        glVertexAttribDivisorARB(ageID, 1); // age : one per quad  -> 1       
    }
    
    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
    glVertexAttribDivisorARB(squareVerticesID, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisorARB(xyzsID, 1); // positions : one per quad (its center)                 -> 1
    glVertexAttribDivisorARB(colorID, 1); // color : one per quad                                  -> 1
                             

    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to :
    // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.
    glDrawArraysInstancedARB(GL_TRIANGLE_STRIP, 0, 4, active_particles);
    //int x = glGetError(); 
    //fprintf(stderr, "error code %s\n", gluErrorString(x));

    glDisableVertexAttribArray(squareVerticesID);
    glDisableVertexAttribArray(xyzsID);
    glDisableVertexAttribArray(colorID);
    if (system_type == IMAGE) {
        glDisableVertexAttribArray(ageID);
    }
}

Vector ParticleSystem::getRandVector()
{
    float phi = (rand() / (float)RAND_MAX) * 2 * M_PI;
    float costheta = (2 * (rand() / (float)RAND_MAX)) - 1;
    float u = (rand() / (float)RAND_MAX);

    float theta = acos(costheta);
    float r = radius * pow(u, (1/3));

    float x = r * sin(theta) * cos(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(theta);

    return Vector(x, y, z);
}

/*
 * getCameraMatrices - sets the model_view and model_projection matrices and 
 * camera_position Point.
 */
void ParticleSystem::getCameraMatrices()
{
    GLfloat mv[16];
    GLfloat p[16];
    glm::mat4 projection;
    glm::mat4 inverse_model_view;

    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glGetFloatv(GL_PROJECTION_MATRIX, p);

    model_view = glm::make_mat4(mv);
    projection = glm::make_mat4(p);
    model_projection = projection * model_view;
    inverse_model_view = glm::inverse(model_view);

    camera_position = Point(inverse_model_view[3][0], 
                            inverse_model_view[3][1], 
                            inverse_model_view[3][2]);

}

void ParticleSystem::cleanup() 
{
    glDeleteBuffers(1, &particles_position_buffer);
    glDeleteBuffers(1, &billboard_vertex_buffer);
    glDeleteBuffers(1, &particles_color_buffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &texture_ID);
}