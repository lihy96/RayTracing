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

	std::vector<Sphere*> sphere_vec;
	std::vector<Plane*> plane_vec;
	std::vector<PointLight*> plight_vec;
};

#endif