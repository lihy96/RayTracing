#ifndef MYOBJ_H
#define MYOBJ_H

#include <vector>
#include "sphere.h"
#include "plane.h"
#include "light.h"

class MyObj
{
public:
	MyObj(){};
	~MyObj(){};

	void push_back(Sphere* ele){	sphere_vec.push_back(ele);	}
	void push_back(Plane* ele){	plane_vec.push_back(ele);	}
	void push_back(PointLight* ele){	plight_vec.push_back(ele);	}
	//void push_back(Primitive* ele) {
	//	if(ele->getType() == Config ::LIGHT_TYPE)
	//		plight_vec.push_back(static_cast<PointLight*>(ele));
	//	else if(ele->getType() == Config::PLANE_TYPE)
	//		plane_vec.push_back(static_cast<Plane*>(ele));
	//	else if(ele->getType() == Config::SPHERE_TYPE)
	//		sphere_vec.push_back(static_cast<Sphere*>(ele));
	//	else
	//		exit(99);
	//}

	std::vector<Sphere*> sphere_vec;
	std::vector<Plane*> plane_vec;
	std::vector<PointLight*> plight_vec;
};

#endif