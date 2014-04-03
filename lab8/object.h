/*  =================== File Information =================
	File Name: object.h
	Description:
	Author: Michael Shah
	Last Modified: 4/2/14

	Purpose: 
	Usage:	

	Further Reading resources: 
	===================================================== */
#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glui.h>
#include "Algebra.h"
#include "ppm.h"

/*
	A Very simple structure that holds
	translation, scale, and rotation values.

*/
struct objectKeyFrame{
	float tx,ty,tz; // translation x,y,z
	float sx,sy,sz; // scale x,y,z
	float angle,rx,ry,rz; // rotate x,y,z
};

/*
	This object renders a piece of geometry ('a sphere by default')
	that has one texture that can be drawn on.

*/
class object{
	public:
		/*	===============================================
		Desc:
		Precondition: 
		Postcondition:
		=============================================== */ 
		object(int _id);
		/*	===============================================
		Desc:
		Precondition: 
		Postcondition:
		=============================================== */ 
		~object();
		/*	===============================================
		Desc:
		Precondition: 
		Postcondition:
		=============================================== */
		GLuint loadTexture(int width, int height, char* pixels);
		/*	===============================================
		Desc:	This instantiates an image to be rendered.
				
				If memory has already been allocated for this,
				then our contract is to delete the previous image,
				and overwrite it with the new one.
		Precondition: 
		Postcondition:
		=============================================== */ 
		void setTexture(int textureNumber,std::string _fileName);
		/*	===============================================
		Desc:	Draws three handles on an object of a specified size
		Precondition: 
		Postcondition:
		=============================================== */ 
		void drawHandles(float size);

		/*	===============================================
		Desc:	Draw two planes for each of the ppms texture
		Precondition: 
		Postcondition:
		=============================================== */ 
		void render();
		/*	===============================================
		Desc:	Draw the actual rendered spheres
		Precondition: 
		Postcondition:
		=============================================== */ 
		void drawTexturedSphere();
		/*	===============================================
		Desc:			Calls into this function modify a previously loaded ppm's
						color array.

						Note that this does NOT change the original ppm image at all.

						Also note, that a new OpenGL Texture is generated everytime, we
						paint on the texture.  There could probably be a buffer to improve
						the performance of this call.
		Precondition: 
		Postcondition:
		=============================================== */ 
		void paintTexture(int x, int y, char r, char g, char b);

		/*	===============================================
		Desc:	A simple frame function that sets objects transformations.
				It will get the objects current translate, scale, and rotation values
				and store them in the frame.
		Precondition: 
		Postcondition:
		=============================================== */ 
		void setFrame(unsigned int frameNumber);
		
		/*	===============================================
		Desc:	Outputs values of a key frame

				This could be improved by actually returning the
				actual struct to an actual objectKeyFrame struct
				in the future.

				For the purpose of this lab though, it is used to output values
				to help users debug.  An overloaded function that returns 
				an objectKeyFrame, could be the basis for also saving frames
				to a file, and then loading a scene.
		Precondition: 
		Postcondition:
		=============================================== */ 
		void getFrame(unsigned int frameNumber);

		/*	===============================================
		Desc:	Outputs translation values of each keyframe to the user
		Precondition: 
		Postcondition:
		=============================================== */ 		
		void debugTranslationFrame(unsigned int frameNumber);

		/*	===============================================
		Desc:	Plays the specified frame.
		Precondition: 
		Postcondition:
		=============================================== */ 
		void playFrame(unsigned int frameNumber);

		/*	===============================================
		Desc:	Animates a model from start to finish
		Precondition: 
		Postcondition:
		=============================================== */ 
		void animate(int startFrame, int endFrame);


		/*	===============================================
		Desc:	Sets an objects x,y,z position
		Precondition: 
		Postcondition:
		=============================================== */ 
		void setPosition(float x, float y, float z);

		/*
			Object ID
		*/
		int id;	// This is a unique id that we can reference our object by
		float radius; // default radius of our sphere object

		/*	===============================================
		Desc:	Public variables for controlling an objects translate, scale,
				and rotate values.

				Ideally these should all be in get/set methods, but for conciseness
				of code we can leave these public(Although since any floating point value
				can be put in, we don't really need any special get/set methods).
		Precondition: 
		Postcondition:
		=============================================== */ 
		float tx,ty,tz; // translation x,y,z
		float sx,sy,sz; // scale x,y,z
		float angle,rx,ry,rz; // rotate x,y,z

		// If this is true, then call the drawHandles(float) function 
		// in the render() function
		bool drawHandle;

		// Dynamic array of keyframes
		// By default, we allocate 50 of these frames for our object
		// 
		// Note: This is dangerous to make this public!
		// 		 It is done so for lab purposes only. See an example in animateScene() of how this is used in main.cpp
		objectKeyFrame* keyframe;
				
	private:
		// The first texture image
		// This should be a white, black, pink, or other solid image that
		// we can draw on.
		ppm* baseTexture;
		// A second texture image that can be loaded
		// This is a second texture that we can blend onto our sphere
		// This is demonstrating multiple textures or 'multitexturing' as it
		// is called in the graphics world.
		ppm* blendTexture;

		// Create a texture id that uniquly identifies each texture image
		// I believe OpenGL will use these id's when we generate a texture
		// to uniquely identify which array of pixels to map to an image.
		GLuint baseTextureID;
		GLuint blendTextureID;

		// Keep track of which frame our object is on
		unsigned int currentFrame;
		// Keeps track of how many frames this model has.
		unsigned int totalKeyFrames;

};

#endif