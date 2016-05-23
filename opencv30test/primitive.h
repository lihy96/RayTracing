#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "AABB.h"

class Ray;
class IntersectResult;


class Primitive
{
public:
	Primitive() : id(0){};
	~Primitive(){};
	virtual int intersect(const Ray& ray, IntersectResult& result) = 0;
	virtual int isIntersectWithBox(const AABB& a_Box) = 0;
	virtual int getType() const = 0;
	virtual AABB getAABB() = 0;
	const int getLastRay() const { return id; }
	int& setID() { return id; }

	int id;
};


#endif
