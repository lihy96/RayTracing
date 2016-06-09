#ifndef MYOBJ_H
#define MYOBJ_H

#include <vector>
#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "triangle.h"

class MyObj
{
public:
	MyObj(){};
	~MyObj(){};

	//void push_back(Sphere* ele){	sphere_vec.push_back(ele);	}
	//void push_back(Plane* ele){	plane_vec.push_back(ele);	}
	//void push_back(PointLight* ele){	plight_vec.push_back(ele);	}
	//void push_back(Triangle* ele){	tri_vec.push_back(ele);	}

	void push_back(Primitive* ele) {
		if(ele->getType() == Config ::LIGHT_POINT_TYPE)
			plight_vec.push_back(static_cast<PointLight*>(ele));
		else if(ele->getType() == Config::PLANE_TYPE)
			plane_vec.push_back(static_cast<Plane*>(ele));
		else if(ele->getType() == Config::SPHERE_TYPE)
			sphere_vec.push_back(static_cast<Sphere*>(ele));
		else if(ele->getType() == Config::LIGHT_BOX_LIGHT)
			blight_vec.push_back(static_cast<BoxLight*>(ele));
		else if(ele->getType() == Config::TRI_TYPE)
			tri_vec.push_back(static_cast<Triangle*>(ele));
		else{
			cout << "ahadf " << endl;
			cout.flush();
			exit(99);
		}
			
	}

	std::vector<Sphere*> sphere_vec;
	std::vector<Plane*> plane_vec;
	std::vector<PointLight*> plight_vec;
	std::vector<BoxLight*> blight_vec;
	std::vector<Triangle*> tri_vec;
};

#endif