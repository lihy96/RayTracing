#ifndef LIGHT_H
#define LIGHT_H

#include "scene.h"
#include "sphere.h"
#include "myVec3.h"
#include "color.h"
#include "primitive.h"

class Light : public Primitive
{
public:
	Light(const Color& color):
	color(color){}
	const Color& getColor() const {	return color;	}
	void setColor(const Color& color){	this->color = color;}
	//int intersect(const Ray& ray, IntersectResult& result) const ;
	//int isIntersectWithBox(const AABB& a_Box) ;
	//AABB getAABB() ;
private:
	Color color;
};

class PointLight : public Light
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
	
	const Sphere& get() const{ return position; }

	int intersect(const Ray& ray, IntersectResult& result) const override {
		return position.intersect(ray, result);
	}
	int isIntersectWithBox(const AABB& a_Box) override {
		return position.isIntersectWithBox(a_Box);
	}
	int getType() override {
		return Config::LIGHT_TYPE;
	}
	AABB getAABB() override {
		return position.getAABB();
	}

private:
	Sphere position;//光源的本质是一个球
};
#endif