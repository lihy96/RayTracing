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
#include "AABB.h"
#include "config.h"
#include "kdTree.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "opencv.h"

using namespace cv;
using namespace std;


class RayTracer
{
public:
	RayTracer(){};
	~RayTracer(){};

	void run(MyMat&, Camera&,int lo = 0, int hi = 600);//屏， 照相机， 场景中的物体
	void render(MyMat&, Camera&,int lo = 0, int hi = 600);

	void initGrid(const MyObj& scene);

	void addPrimiToBox(Primitive* primi, const MyVec3& loBound, const MyVec3& hiBound);
	//phong use
	const Color rayTraceRecursive(const Ray& ray,  IntersectResult& result,int recursiveDepth, double irefIndex,double sampleTime,double sampleDown);
	//pace tracing use
	const Color rayTraceRecursive(const Ray& ray, IntersectResult& result, int recursiveDepth, double irefIndex, int E = 1);

	int findNearestNoAABB(const Ray& ray, IntersectResult& result);
	int findNearest(const Ray& ray, IntersectResult& result);
	int nearest(Primitive* primi,const Ray& ray, IntersectResult& result, int& re);

	double calSoftShade(BoxLight* light, MyVec3 ip, MyVec3& dir, const MyObj& scene, double sampleTime, double sampleDown);
	
	//void setCamera(const Camera& _cam){ camera = _cam; }
	//void setScene(const MyObj& _myobj) { scene = _myobj; }

	//纹理贴图
	void addTexture(MyObj& scene);

	//景深
	void castRayWithDeepOfFied(Camera&, MyObj&, Ray&){};

	//文本读入
	void readScene(char* file, MyObj& scene);
	void readObj(const char* file, MyObj& scene,MyVec3 O, MyVec3 size, MyVec3 angles,Material* material);

	AABB wholeSpace;
	MyVec3 unit;
	MyObj allGrids[Config::GRIDSIZE+1][Config::GRIDSIZE+1][Config::GRIDSIZE+1];

	MyObj scene;
	void addScene(MyObj& _scene){scene = _scene;}
};

#endif