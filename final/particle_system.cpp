#include "particle_system.h"
#include "shader.h"
#include <algorithm>

ParticleSystem::ParticleSystem()
{
    max_particles = 1000;
    last_used_particle = 0;
    last_time = glutGet(GLUT_ELAPSED_TIME);
    position_size_data = new GLfloat[max_particles * 4];
    color_data = new GLubyte[max_particles * 4];
    Shader manager;

// Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Create and compile our GLSL program from the shaders
    programID = manager.loadShader(vertex_shader.c_str(), fragment_shader.c_str());

    // Vertex shader
    CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
    CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
    ViewProjMatrixID = glGetUniformLocation(programID, "VP");

    // fragment shader
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Get a handle for our buffers
    squareVerticesID = glGetAttribLocation(programID, "squareVertices");
    xyzsID = glGetAttribLocation(programID, "xyzs");
    colorID = glGetAttribLocation(programID, "color");   
    

    for(int i= 0; i< max_particles; i++){
        particles[i].lifetime = -1.0f;
        particles[i].cameradistance = -1.0f;
    }

    //Texture = loadDDS("particle.DDS");

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

void ParticleSystem::drawParticles() {}

/*
 * Finds a Particle in ParticlesContainer which isn't used yet. (i.e. life < 0);
 */
 int ParticleSystem::findUnusedParticle()
 {
    for(int i = last_used_particle; i < max_particles; i++) {
        if (particles[i].lifetime < 0) {
            last_used_particle = i;
            return i;
        }
    }
    for(int i = 0; i < last_used_particle; i++) {
    	if (particles[i].lifetime < 0) {
            last_used_particle = i;
            return i;
        }
    }
    return 0; // All particles are taken, override the first one
}

void ParticleSystem::SortParticles(){
    std::sort(&particles[0], &particles[max_particles]);
}

void ParticleSystem::bindShaders()
{
    position_size_data = new GLfloat[max_particles * 4];
    color_data = new GLubyte[max_particles * 4];
    /* Update the buffers that OpenGL uses for rendering */

    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, active_particles * sizeof(GLfloat) * 4, position_size_data);

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, active_particles * sizeof(GLubyte) * 4, color_data);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Use our shader
    glUseProgram(programID);

    // Bind our texture in Texture Unit 0
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    //glUniform1i(TextureID, 0);

    GLfloat mv[16];
    GLfloat p[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glGetFloatv(GL_PROJECTION_MATRIX, p);
    Matrix model_view = Matrix(mv);
    Matrix perspective = Matrix(p);
    Matrix model_perspective = model_view * perspective;

    // Same as the billboards tutorial
    glUniform3f(CameraRight_worldspace_ID, model_view[0], model_view[4], model_view[8]);
    glUniform3f(CameraUp_worldspace_ID, model_view[1], model_view[5], model_view[9]);

    glUniformMatrix4dv(ViewProjMatrixID, 1, GL_FALSE, model_perspective.unpack());

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

    glDisableVertexAttribArray(squareVerticesID);
    glDisableVertexAttribArray(xyzsID);
    glDisableVertexAttribArray(colorID);
}