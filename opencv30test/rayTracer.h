#ifndef RAYTRACER_H
#define RAYTRACER_H
#define DEBUG

#ifdef DEBUG
#include "all.h"
#endif

#include "debug.h"
#include "myMat.h"
#include "myObj.h"
#include "ray.h"
#include "myVec3.h"
#include "camera.h"
#include "intersectResult.h"
#include "primitive.h"
#include "config.h"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class RayTracer
{
public:
	RayTracer(){};
	~RayTracer(){};

	void run(MyMat&, Camera&, MyObj&);//���� ������� �����е�����
	const Color rayTraceRecursive(const Ray& ray, const MyObj& scene, int recursiveDepth, double irefIndex);
	int findNearest(const Ray& ray,const MyObj& scene, IntersectResult& result);
};
const double DEVIANCE = 0.01;
#endif