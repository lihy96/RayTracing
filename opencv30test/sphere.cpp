#include "sphere.h"

/*
bool Sphere::intersect(const Ray& ray, IntersectResult& result) const{
		//http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html ʽ5.4
		//	|| o + t*d - c|| = r   ��=�� || v + t*d|| = r
		//	t = - dot(d, v) + - sqrt( dot(d,v)^2 - (||v|| - r^2) ) ȡ����Ϊ����
		//	 dot(d, v) > 0 �� ���߲��������巢�� or dot(d,v)^2 - (||v|| - r^2) < 0 ���б�ʽС���� =��û�н���
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
}*/


