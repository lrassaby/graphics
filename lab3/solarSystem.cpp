#include <GL/glui.h>
#include "solarSystem.h"
#include <time.h>
#include <stdlib.h>


solarSystem::solarSystem(){

}

solarSystem::~solarSystem(){

}

void makePlanet(float rotation, float orbit, float size, 
                float r, float g, float b, bool rings, int moons) {
	glPushMatrix();
		glRotatef(rotation, 0,1,0);
		glColor3f(r, g, b);
		glTranslatef(orbit, 0.0f, 0.0f);
		glutSolidSphere(size,10,10);
	glPopMatrix();
	for (int i = 0; i < moons; i++) {
		glPushMatrix();
			glRotatef(rotation, 0,1,0);
			glColor3f(.5f, .5f, .5f);
			glTranslatef(orbit, 0.0f, 0.0f);

			glRotatef(rotation + (float) i * 100, 0,1,0);
			glTranslatef(size + .15, 0.0f, 0.0f);

			glutSolidSphere(size / 5,10,10);
		glPopMatrix();
	}
}


// Render this with push and pop operations
// or alternatively implement helper functions
// for each of the planets with parameters.
void solarSystem::render(){
	
	// Some ideas for constants that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitSpeed = 0.1;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 1;

	glPushMatrix();

		// The Sun (though not a planet)
		makePlanet(rotation, 0, 1.2, 0.6, 0.4, 0.0, false, 0);

		// The Planets
		makePlanet(rotation * 5, 2, .2, 1.0, 0.0, 0.0, false, 1);
		makePlanet(rotation * 3, 3, .3, 0.8, 0.2, 0.66, false, 0);
		makePlanet(rotation * 10, 4, .3, 0.37, 0.91, 0.1, true, 0);
		makePlanet(rotation * 28, 5, .8, 0.2, 0.44, 0.1, false, 5);
		makePlanet(rotation * 5, 7, .5, 0.0, 0.68, 0.68, true, 1);
		makePlanet(rotation * 59, 9, .1, 0.8, 0.2, 0.4, false, 0);
		

	glPopMatrix();


	rotation+= 0.01;
	orbitSpeed+=0.05;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}
