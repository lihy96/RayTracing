#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

class Plane:public Primitive
{
public:

	~Plane(){};
	int intersect(const Ray& ray, IntersectResult& result) override{
		double cosA = -MyVec3::dot(normalVec, ray.getDir());	
		if (cosA > 0){
			double delta = MyVec3::dot(normalVec, ray.getOri());
			double b = od + delta;	// 照相机到平面的距离
			double dist = b / cosA;
			if(dist < result.distance){
				result.distance = dist; 
				result.intersectPoint = ray.getOri() + result.distance * ray.getDir();
				result.normalVec = normalVec;
				result.ma = ma;
				result.primi = this;
				return 1;
			}
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
			if(side1 && side2)
				return 1;
		}
	
		return 0;
		
	}
	//int getType()  const override{
	//	return Config::PLANE_TYPE;
	//}

	AABB getAABB() override {
		return AABB(MyVec3(-10000.0, -10000.0, -10000.0), MyVec3(20000.0, 20000.0, 20000.0));
	}

	Color getColorTexture(MyVec3& a_Pos) {
		Color retval;
		if (ma->GetTexture())
		{
			//cout << "have plane texture " << endl;
			Texture* t = ma->GetTexture();

			MyVec3 m_UAxis = MyVec3( normalVec.y, normalVec.z, -normalVec.x );
			MyVec3 m_VAxis = MyVec3::cross(m_UAxis, normalVec );

			double u = MyVec3::dot( a_Pos, m_UAxis ) * ma->GetUScale();
			double v = MyVec3::dot( a_Pos, m_VAxis ) * ma->GetVScale();
			retval = t->GetTexel( u, v ) * ma->getColor();
		}
		else{
			//cout << "no plane texture" << endl;
			retval = ma->getColor();
		}
		return retval;
	}

//	const Chess& material() const{	return ma;	}
	Material* material() {	return ma;	}

	//	constructor
	Plane():
	Primitive(Config::PLANE_TYPE), normalVec(MyVec3(0, 1, 0)), od(0), ma(0){}

	Plane(const MyVec3& normalVec, double od):
	Primitive(Config::PLANE_TYPE),normalVec(MyVec3(normalVec).unit()), od(od), ma(0){}

	Plane(const MyVec3& normalVec, double od, Material* m):
	Primitive(Config::PLANE_TYPE),normalVec(MyVec3(normalVec).unit()), od(od), ma(m){}

private:
	MyVec3 normalVec;
	//平面到坐标原点的距离
	double od;
	Material* ma;
};

#endif