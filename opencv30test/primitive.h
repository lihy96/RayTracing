#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "AABB.h"

class Ray;
class IntersectResult;


class Primitive
{
public:
	Primitive(){};
	~Primitive(){};
	virtual int intersect(const Ray& ray, IntersectResult& result) const = 0;
	virtual int isIntersectWithBox(const AABB& a_Box) = 0;
	virtual int getType() = 0;
	virtual AABB getAABB() = 0;
};


#endif
