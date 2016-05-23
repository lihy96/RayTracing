#ifndef RAY_H
#define RAY_H

#include "myVec3.h"

class Ray
{
	MyVec3 origin,direction;
public:
	Ray():origin(MyVec3(0, 0, 0)), direction(MyVec3(0, 0, 1)){ id = number++; };
	Ray(const MyVec3& ori, const MyVec3& dir): origin(ori), direction(MyVec3(dir).unit()){ id = number++; };
	~Ray(){};

	const MyVec3& getOri() const { return origin; }
	const MyVec3& getDir() const { return direction; }
	const int getID() const { return id; }
	void setOri(const MyVec3& ori){	origin = ori;	}	
	void setDir(const MyVec3& dir){	direction = MyVec3(dir).unit(); }

	int id;
	static int number;
};

#endif