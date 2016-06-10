#ifndef INTERSECTRESULT_H
#define INTERSECTRESULT_H

#include "myVec3.h"
#include "material.h"
#include "primitive.h"

class IntersectResult
{
public:
	IntersectResult() : 
		intersectPoint(0,0,0),
		normalVec(0,0,0),
		distance(1000000000000.0),
		isInObj(false){};
	~IntersectResult(){};
	
	MyVec3 intersectPoint, normalVec;
	bool isInObj;
	double distance;
	const Material* ma;
	Primitive* primi;
};
const double INF = 1000000000000.0;
#endif

