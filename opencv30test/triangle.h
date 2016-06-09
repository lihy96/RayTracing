#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitive.h"
#include "myVec3.h"
#include "material.h"
#include "intersectResult.h"

class Triangle : public Primitive
{
public:
	Triangle () {};
	Triangle (const MyVec3& v0, const MyVec3& v1, const MyVec3& v2, const MyVec3& _fxl, Phong* _ma) :
	normalVec(_fxl), ma(_ma){vertex[0]=v0; vertex[1]=v1; vertex[2]=v2;	};
	~Triangle () {};

	int intersect(const Ray& ray, IntersectResult& result) override{
		id = ray.getID();
		MyVec3 e1 = vertex[1] - vertex[0];
		MyVec3 e2 = vertex[2] - vertex[0];
		MyVec3 q = MyVec3::cross(ray.getDir(), e2);
		double a = MyVec3::dot(e1, q);
		if ((a > -1e-6) && (a < 1e-6))
			return 0;
		double f = 1.0 / a;
		MyVec3 s = ray.getOri() - vertex[0];
		double u = f * MyVec3::dot(s, q);
		if (u < 0.0)
			return 0;
		MyVec3 r = MyVec3::cross(s, e1);
		double v = f * MyVec3::dot(ray.getDir(), r);
		if ((v < 0.0) || (u + v > 1.0))
			return 0;
		double t = f * MyVec3::dot(e2, r);
		if (t < 0)
			return 0;
		if (t < result.distance - 1e-6){
			result.distance = t;
			result.intersectPoint = ray.getOri() + t * ray.getDir();
			result.primi = this;
			result.normalVec = normalVec;
			result.ma = ma;
		}
		else if ((t - result.distance < 1e-6) && (t - result.distance > -1e-6)){
			result.normalVec = result.normalVec + normalVec;
			MyVec3::normalize(result.normalVec);
		}
		return 1;
	}

	

	int isIntersectWithBox(const AABB& box) override{
		double hx = box.getSize().x * 0.5;
		double hy = box.getSize().y * 0.5;
		double hz = box.getSize().z * 0.5;
		MyVec3 o(box.getPos().x + hx, box.getPos().y + hy, box.getPos().z + hz);
		MyVec3 w, c, one, ano;
		for (int i = 0;i < 3;i++){
			one = vertex[i] - o;
			ano = vertex[(i == 2) ? 0 : (i + 1)] - o;
			w = 0.5 * (ano - one);
			c = one + w;
			if (!(fabs(c.x) > fabs(w.x) + hx))
				return true;
			if (!(fabs(c.y * w.z - c.z * w.y) > (hy * fabs(w.z) + hz * fabs(w.y))))
				return true;
			if (!(fabs(c.y) > fabs(w.y) + hy))
				return true;
			if (!(fabs(c.x * w.z - c.z * w.x) > (hx * fabs(w.z) + hz * fabs(w.x))))
				return true;
			if (!(fabs(c.z) > fabs(w.z) + hz))
				return true;
			if (!(fabs(c.x * w.y - c.y * w.x) > (hx * fabs(w.y) + hy * fabs(w.x))))
				return true;
		}

		return false;
	}

	int getType()  const override{
		//cout << "meme" << endl;
		return Config::TRI_TYPE;
	}

	AABB getAABB() override {
		double minx = INF, maxx = -INF;
		double miny = INF, maxy = -INF;
		double minz = INF, maxz = -INF;
		for (int i = 0; i < 3; i++){
			minx = min(minx, vertex[i].x); maxx = max(maxx, vertex[i].x);
			miny = min(miny, vertex[i].y); maxy = max(maxy, vertex[i].y);
			minz = min(minz, vertex[i].z); maxz = max(maxz, vertex[i].z);
		}
		return AABB(MyVec3(minx, miny, minz), MyVec3(maxx - minx, maxy - miny, maxz - minz));	
	}

	Material* material() {	return ma;	}

	Color getColorTexture(MyVec3& a_Pos){
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


	MyVec3 normalVec;
	MyVec3 vertex[3];
	Phong* ma;
};



#endif