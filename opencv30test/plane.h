#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

class Plane:public Primitive
{
public:

	~Plane(){};
	int intersect(const Ray& ray, IntersectResult& result) const override{
		double cosA = -MyVec3::dot(normalVec, ray.getDir());	
		if (cosA > 0){
			double delta = MyVec3::dot(normalVec, ray.getOri());
			double b = od + delta;	// 照相机到平面的距离
			result.distance = b / cosA; 
			result.intersectPoint = ray.getOri() + result.distance * ray.getDir();
			result.normalVec = normalVec;
			result.ma = ma;
			result.primi = this;
			return 1;
		}
		return 0;
	}
	int isIntersectWithBox(const AABB& a_Box) override{
		MyVec3 v[2];
		v[0] = a_Box.getPos(), v[1] = a_Box.getPos_2();
		int side1 = 0,side2 = 0;
		for (int i = 0; i < 8; i++ )
		{
			MyVec3 p( v[i & 1].x, v[(i >> 1) & 1].y, v[(i >> 2) & 1].z);
			if ((MyVec3::dot( p, normalVec ) + od) < 0) 
				side1++; 
			else 
				side2++;
		}
		if ((side1 == 0) || (side2 == 0)) 
			return 0;
		else 
			return 1;		
	}
	int getType() override{
		return Config::PLANE_TYPE;
	}

	AABB getAABB() override {
		return AABB(MyVec3(-INF, -INF, -INF), MyVec3(2 *INF, 2 * INF, 2 * INF));
	}

//	const Chess& material() const{	return ma;	}
	const Material* material() const{	return ma;	}

	//	constructor
	Plane():
	normalVec(MyVec3(0, 1, 0)), od(0), ma(0){}

	Plane(const MyVec3& normalVec, double od):
	normalVec(MyVec3(normalVec).unit()), od(od), ma(0){}

	Plane(const MyVec3& normalVec, double od, Chess& m):
	normalVec(MyVec3(normalVec).unit()), od(od), ma(&m){}

	Plane(const MyVec3& normalVec, double od, Phong& m):
	normalVec(MyVec3(normalVec).unit()), od(od), ma(&m){}

	Plane(const MyVec3& normalVec, double od, Chess* m):
	normalVec(MyVec3(normalVec).unit()), od(od), ma(m){}

	Plane(const MyVec3& normalVec, double od, Phong* m):
	normalVec(MyVec3(normalVec).unit()), od(od), ma(m){}

private:
	MyVec3 normalVec;
	//平面到坐标原点的距离
	double od;
	Material* ma;
};

#endif