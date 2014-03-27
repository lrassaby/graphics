/*
	File Name: entity.cpp
	Description:
	Author:

	Purpose: Base class to be inherited from for 3D objects that can be transformed
	Examples:
*/
#include "entity.h"

entity::entity(){
	x = 0;
	y = 0;
	z = 0;
	sx = 1.0;
	sy = 1.0;
	sz = 1.0;
}

float entity::getXPosition(){ return x; }
float entity::getYPosition(){ return y; }
float entity::getZPosition(){ return z; }

float entity::setPosition(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

float entity::setXPosition(float _x){ x = _x; }
float entity::setYPosition(float _y){ y = _y; }
float entity::setZPosition(float _z){ z = _z; }

float entity::getXScale(){ return sx; }
float entity::getYScale(){ return sy; }
float entity::getZScale(){ return sz; }

float entity::setScale(float _sx, float _sy, float _sz){
	sx = _sx;
	sy = _sy;
	sz = _sz;
}
float entity::setXScale(float _sx){ sx = _sx; }
float entity::setYScale(float _sy){ sy = _sy; }
float entity::setZScale(float _sz){ sz = _sz; }