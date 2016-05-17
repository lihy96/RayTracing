#ifndef PLANE_H
#define PLANE_H

class Plane
{
public:

	~Plane(){};

	//	cosA = b / c 
	//=>c = b / cosA
	int intersect(const Ray& ray, IntersectResult& result) const
	{
		double cosA = -dot(normalVec, ray.getDir());	
		if (cosA > 0){
			double delta = dot(normalVec, ray.getOri());
			double b = od + delta;
			result.distance = b / cosA;
			result.intersectPoint = ray.getOri() + result.distance * ray.getDir();
			result.normalVec = normalVec;
			result.ma = ma;
			return 1;
		}
		return 0;
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
	//	the distance from origin to plane
	double od;
	Material* ma;
};

#endif