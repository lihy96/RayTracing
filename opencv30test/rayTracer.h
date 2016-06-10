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
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


class RayTracer
{
public:
	RayTracer(){};
	~RayTracer(){};

	void run(MyMat&, Camera&);//屏， 照相机， 场景中的物体
	void render(MyMat&, Camera&);

	void initGrid(const MyObj& scene);

	void addPrimiToBox(Primitive* primi, const MyVec3& loBound, const MyVec3& hiBound);
	//phong use
	const Color rayTraceRecursive(const Ray& ray, int recursiveDepth, double irefIndex,double sampleTime,double sampleDown);
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
	void readObj(char* file, MyObj& scene,MyVec3 O, MyVec3 size, MyVec3 angles);

	AABB wholeSpace;
	MyVec3 unit;
	MyObj allGrids[Config::GRIDSIZE+1][Config::GRIDSIZE+1][Config::GRIDSIZE+1];

	MyObj scene;
	void addScene(MyObj& _scene){scene = _scene;}

	//KdTreeNode* root;
};

/*
template<typename T>
int RayTracer::nearest(T& it,const Ray& ray, const MyObj& scene, IntersectResult& result, int& re){
	int in_out = 0;
	if (it->getLastRay() != ray.getID()){
		IntersectResult temp;
		if (in_out = it->intersect(ray, temp))
			if (temp.distance < result.distance){
				result = temp;
				re = in_out;
			}													
	}
	return in_out;
}

template<typename T>
void RayTracer::addPrimiToBox(T& primi, const MyVec3& loBound, const MyVec3& hiBound){

	AABB bigBOX = primi->getAABB();
	//cout << bigBOX.getPos() << " "<<bigBOX.getPos_2() << endl;exit(4);
	int x1 = (bigBOX.getPos().x - loBound.x) / unit.x;
	int x2 = (bigBOX.getPos_2().x - loBound.x) / unit.x + 1;
	x1 = x1 < 0 ? 0 : x1;
	x2 = x2 > Config::GRIDSIZE ? Config::GRIDSIZE : x2;
	int y1 = (bigBOX.getPos().y - loBound.y) / unit.y;
	int y2 = (bigBOX.getPos_2().y - loBound.y) / unit.y + 1;
	y1 = y1 < 0 ? 0 : y1;
	y2 = y2 > Config::GRIDSIZE ? Config::GRIDSIZE : y2;
	int z1 = (bigBOX.getPos().z - loBound.z) / unit.z;
	//cout << bigBOX.getPos_2().z <<  loBound.z << unit.z;exit(1);
	int z2 = (bigBOX.getPos_2().z - loBound.z) / unit.z + 1;
	//cout << z2 << endl;exit(1);
	z1 = z1 < 0 ? 0 : z1;
	z2 = z2 > Config::GRIDSIZE ? Config::GRIDSIZE : z2;
	
	cout << x1 << x2 << y1 << y2 << z1 << z2 << endl;
	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			for (int z = z1; z < z2; z++){
				MyVec3 l(loBound.x + x * unit.x, loBound.y + y * unit.y, loBound.z + z * unit.z);
				AABB cell(l, unit);
				cout << l << endl;
				if (primi->isIntersectWithBox(cell)){						
					allGrids[x][y][z].push_back(primi);
					//cout << x << y << z << endl;
				}

			}	

}*/
#endif