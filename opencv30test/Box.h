#ifndef BOX_H
#define BOX_H

#include "MyVec3.h"
class IntersectResult;

class Box
{
public:
	Box(){};
	~Box() {};
	virtual bool isContainPoint(const MyVec3& p) = 0;
	virtual bool isIntersect(const Box& m) = 0;
	void tm(IntersectResult& re);
};

#endif