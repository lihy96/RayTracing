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
	Sphere() : Primitive(Config::SPHERE_TYPE),center(MyVec3(0,0,0)) , R(0) ,ma(0){ init(); };
	Sphere(const MyVec3& _center, double _R) : Primitive(Config::SPHERE_TYPE),center(_center) , R(_R) ,ma(0){ init(); };
	Sphere(const MyVec3& _center, double _R, Material* _ma): Primitive(Config::SPHERE_TYPE),center(_center) , R(_R) ,ma(_ma){ init();};
	~Sphere(){};
	
	int intersect(const Ray& ray, IntersectResult& result) override{	
		//	|| o + t*d - c|| = r   《=》 || v + t*d|| = r
		//	t = - dot(d, v) + - sqrt( dot(d,v)^2 - (||v|| - r^2) ) 取负号为焦点
		//	 dot(d, v) > 0 ： 光线不向着物体发射 or dot(d,v)^2 - (||v|| - r^2) < 0 ：判别式小于零 =》没有交点

		id = ray.getID();


		MyVec3 v = center - ray.getOri();
		double judge = MyVec3::dot(ray.getDir(), v);
		double judge1 = judge * judge - MyVec3::dot(v, v) + R_2;
		if(judge1 >= 0){
			judge1 = std::sqrt(judge1);
			double x1 = judge + judge1;
			double x2 = judge - judge1;

			if(x2 > 0){//交点在光线内部
				if (x2 < result.distance){
					result.intersectPoint = ray.getOri() + x2 * ray.getDir();
					result.distance = x2;
					result.normalVec = (result.intersectPoint - center).unit();
					result.ma = ma;
					result.primi = this;
					return 1;
				}
			}else if(x1 > 0){
				if (x1 < result.distance){
					result.intersectPoint = ray.getOri() + x1 * ray.getDir();
					result.distance = x1;
					result.normalVec = (result.intersectPoint - center).unit();
					result.ma = ma;
					result.primi = this;
					return -1;
				}
			}
			
		}
		return 0;
	}

	int isIntersectWithBox(const AABB& a_Box) override{ 
		double dmin = 0;
		MyVec3 v1 = a_Box.getPos(), v2 = a_Box.getPos_2(); //盒子的两个顶点
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

	//int getType() const override{
	//	return Config::SPHERE_TYPE;
	//}

	AABB getAABB() override{
		MyVec3 size(R, R, R);
		return AABB(center - size, 2 * size);
	}
	
	Material* material() {	return ma;	}
	const MyVec3& getCenter() const{	return center;	}
	
	const Color getColorTexture(MyVec3& a_Pos) {
		Color retval;
		if (!ma->GetTexture()) {
			//cout << "no sphere texture" << endl;
			retval = ma->getColor(); 
		}
		else
		{
			//cout << "have sphere texture" << endl;
			MyVec3 vp = ((double)(1 / R)) * (a_Pos - center);
			double phi = acosf( -MyVec3::dot( vp, YPLUS ) );
			double u, v = phi * ma->GetVScaleReci() * (1.0f / PI);
			double theta = (acosf( MyVec3::dot( XPLUS, vp ) / sinf( phi ))) * (2.0f / PI);
			if (MyVec3::dot( ZPLUS, vp ) <= 0) u = (1.0f - theta) * ma->GetUScaleReci();
								 else u = theta * ma->GetUScaleReci();
			retval = ma->GetTexture()->GetTexel( u, v ) * ma->getColor();
		}
		return retval;	
		
	}



	void init(){ R_2 = R * R; };
	MyVec3 center;
	double R,R_2;
	Material* ma;
};

#endif
