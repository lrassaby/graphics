/*
	File Name: entity.h
	Description:
	Author:

	Purpose: Base class to be inherited from for 3D objects that can be transformed
	Examples:
*/
#ifndef ENTITY_H
#define ENTITY_H

class entity{

public:
	/*
	Constructor
	*/
	entity();

	float getXPosition();
	float getYPosition();
	float getZPosition();

	float setPosition(float x, float y, float z);
	float setXPosition(float _x);
	float setYPosition(float _y);
	float setZPosition(float _z);

	float getXScale();
	float getYScale();
	float getZScale();

	float setScale(float _sx, float _sy, float _sz);
	float setXScale(float _sx);
	float setYScale(float _sx);
	float setZScale(float _sx);
	

private:
		// translate values
		float x,y,z;
		// scale values
		float sx,sy,sz;
};

#endif