#ifndef LIGHT_H
#define LIGHT_H

#include "scene.h"
#include "sphere.h"
#include "myVec3.h"
#include "color.h"

class Light
{
public:
	Light(const Color& color):
	color(color){}
	const Color& getColor() const {	return color;	}
	void setColor(const Color& color){	this->color = color;}
private:
	Color color;
};

class PointLight:public Light
{
public:
	PointLight(double x, double y, double z, double r, const Color& color):
	Light(color),
	position(MyVec3(x, y, z), r){}
	PointLight(const MyVec3& center, double r, const Color& color):
	Light(color),
	position(center, r){}
	PointLight(const Sphere& position, const Color& color):
	Light(color),
	position(position){}
	const Sphere& get() const{	return position;	}
private:
	Sphere position;
};
#endif