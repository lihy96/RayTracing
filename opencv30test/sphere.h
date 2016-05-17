#ifndef SPHERE_H
#define SPHERE_H

#include "myVec3.h"
#include "ray.h"
#include "intersectResult.h"
#include "material.h"

class Sphere
{
public:
	Sphere() : center(MyVec3(0,0,0)) , R(0) ,ma(WHITE, 0, 0, 0, 0){ init(); };
	Sphere(const MyVec3& _center, double _R) : center(_center) , R(_R) ,ma(WHITE, 0, 0, 0, 0){ init(); };
	Sphere(const MyVec3& _center, double _R, const Phong& _ma):center(_center) , R(_R) ,ma(_ma){ init();};
	~Sphere(){};
	
	bool intersect(const Ray& ray, IntersectResult& result) const{	
		//	|| o + t*d - c|| = r   《=》 || v + t*d|| = r
		//	t = - dot(d, v) + - sqrt( dot(d,v)^2 - (||v|| - r^2) ) 取负号为焦点
		//	 dot(d, v) > 0 ： 光线不向着物体发射 or dot(d,v)^2 - (||v|| - r^2) < 0 ：判别式小于零 =》没有交点
		MyVec3 v = ray.getOri() - center;
		double judge1 = dot(ray.getDir(), v);
		if(judge1 >  0)
			return false;
		else{
			double judge2 = judge1 * judge1 - v.module_2() + R_2;
			if(judge2 < 0)
				return false;
			else{
				double x2 = -judge1 - sqrt(judge2);
				// x1 = -judge1 + sqrt(judge2);
				result.intersectPoint = ray.getOri() + x2 * ray.getDir();
				result.distance = x2;
				result.normalVec = (result.intersectPoint - center).unit();
				return true;
			}
		}
	}
	Phong& material() {	return ma;	}
	const MyVec3& getCenter() const{	return center;	}
private:
	void init(){ R_2 = R * R; };
	MyVec3 center;
	double R,R_2;
	Phong ma;
};

#endif
