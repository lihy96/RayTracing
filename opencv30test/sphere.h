#ifndef SPHERE_H
#define SPHERE_H

#include "myVec3.h"
#include "ray.h"
#include "intersectResult.h"
#include "material.h"
#include "primitive.h"
#include "config.h"

class Sphere:public Primitive
{
public:
	Sphere() : center(MyVec3(0,0,0)) , R(0) ,ma(WHITE, 0, 0, 0, 0){ init(); };
	Sphere(const MyVec3& _center, double _R) : center(_center) , R(_R) ,ma(WHITE, 0, 0, 0, 0){ init(); };
	Sphere(const MyVec3& _center, double _R, const Phong& _ma):center(_center) , R(_R) ,ma(_ma){ init();};
	~Sphere(){};
	
	int intersect(const Ray& ray, IntersectResult& result) const override{	
		//	|| o + t*d - c|| = r   ��=�� || v + t*d|| = r
		//	t = - dot(d, v) + - sqrt( dot(d,v)^2 - (||v|| - r^2) ) ȡ����Ϊ����
		//	 dot(d, v) > 0 �� ���߲��������巢�� or dot(d,v)^2 - (||v|| - r^2) < 0 ���б�ʽС���� =��û�н���
		MyVec3 v = ray.getOri() - center;
		double judge1 = MyVec3::dot(ray.getDir(), v);
		if(judge1 >  0)
			return 0;
		else{
			double judge2 = judge1 * judge1 - v.module_2() + R_2;
			if(judge2 < 0)
				return 0;
			else{
				judge2 = sqrt(judge2);
				double x1 = -judge1 + judge2;
				double x2 = -judge1 - judge2;
				if(x2 > 0){//�����ڹ����ڲ�
					result.intersectPoint = ray.getOri() + x2 * ray.getDir();
					result.distance = x2;
					result.normalVec = (result.intersectPoint - center).unit();
					result.ma = &ma;
					result.primi = this;
					return 1;
				}else{	
					result.intersectPoint = ray.getOri() + x1 * ray.getDir();
					result.distance = x1;
					result.normalVec = (result.intersectPoint - center).unit();
					result.ma = &ma;
					result.primi = this;
					return -1;
				}
			}
		}
	}

	int isIntersectWithBox(const AABB& a_Box) override{ 
		double dmin = 0;
		MyVec3 v1 = a_Box.getPos(), v2 = a_Box.getPos_2(); //���ӵ���������
		if (center.x < v1.x) 
			dmin = dmin + (center.x - v1.x) * (center.x - v1.x);
		else if (center.x > v2.x)
			dmin = dmin + (center.x - v2.x) * (center.x - v2.x);

		if (center.y < v1.y)
			dmin = dmin + (center.y - v1.y) * (center.y - v1.y);
		else if (center.y > v2.y)
			dmin = dmin + (center.y - v2.y) * (center.y - v2.y);

		if (center.z < v1.z)
			dmin = dmin + (center.z - v1.z) * (center.z - v1.z);
		else if (center.z > v2.z)
			dmin = dmin + (center.z - v2.z) * (center.z - v2.z);
		return (dmin <= R_2);
	}

	int getType() override{
		return Config::SPHERE_TYPE;
	}

	AABB getAABB() override{
		MyVec3 size(R, R, R);
		return AABB(center - size, R * 2);
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