#ifndef AABB_H
#define AABB_H

#include <vector>
#include "ray.h"
#include "Box.h"
#include "config.h"

class IntersectResult ;

class AABB : public Box
{
public:
	AABB():pos(MyVec3(0,0,0)), size(MyVec3(0,0,0)), pos_2(MyVec3(0,0,0)){};
	AABB(const MyVec3& _pos, const MyVec3& _size) : pos(_pos), size(_size), pos_2(_pos + _size) {};
	~AABB(){};

	const MyVec3& getPos() const {return pos;}
	const MyVec3& getSize() const {return size;}
	const MyVec3& getPos_2() const {return pos_2;}

	//static const AABB getAABB(const Sphere&);
	//static const AABB getAABB(const Plane&);
	//static const AABB getAABB(const PointLight&);

	//盒子是否包围一个点
	bool isContainPoint(const MyVec3& p) { 
		MyVec3 v1 = pos, v2 = pos + size;
		return ( (p.x > (v1.x - EPS)) && (p.x < (v2.x + EPS)) &&
			     (p.y > (v1.y - EPS)) && (p.y < (v2.y + EPS)) &&
				 (p.z > (v1.z - EPS)) && (p.z < (v2.z + EPS)) );
	}
	
	//盒子是否与另一个盒子相交
	bool isIntersect(const Box& _m) {
		const AABB &m = *static_cast<const AABB*>(&_m);
		//const AABB& m = dynamic_cast<const AABB&>(_m);
		MyVec3 v1 = m.getPos(), v2 = m.getPos_2();
		MyVec3 v3 = pos, v4 = pos + size;
		return ((v4.x > v1.x) && (v3.x < v2.x) && // x-axis overlap
				(v4.y > v1.y) && (v3.y < v2.y) && // y-axis overlap
				(v4.z > v1.z) && (v3.z < v2.z));   // z-axis overlap
	}

	int intersect(const Ray& ray, IntersectResult& result);
	
	MyVec3 pos,size,pos_2;
};


#endif