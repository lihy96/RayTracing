#ifndef INTERSECTRESULT_H
#define INTERSECTRESULT_H
#include "myVec3.h"
#include "material.h"
class IntersectResult
{
public:
	IntersectResult() : 
		intersectPoint(0,0,0),
		normalVec(0,0,0),
		distance(0.0){};
	~IntersectResult(){};
	
	MyVec3 intersectPoint, normalVec;
	double distance;
	const Material* ma;
};
const double INF = 1000000000000.0;
#endif

