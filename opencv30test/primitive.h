#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "AABB.h"

class Ray;
class IntersectResult;
class Material;

class Primitive
{
public:

	Primitive(int _type) : id(0),type(_type){};
	~Primitive(){};
	virtual int intersect(const Ray& ray, IntersectResult& result) = 0;
	virtual int isIntersectWithBox(const AABB& a_Box) = 0;
	virtual Material* material()  = 0;
	//virtual int getType() const = 0;
	virtual AABB getAABB() = 0;
	const int getLastRay() const { return id; }
	int& setID() { return id; }
	int getType() const {	return type;	}

	int id;
	int type;
};


#endif
