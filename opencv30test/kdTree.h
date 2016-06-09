#ifndef KDTREE_H
#define KDTREE_H
#include <vector>
#include "ray.h"
#include "AABB.h"

class Primitive;
class IntersectResult;
class AABB;

#define MIN_OBJECT_COUNT 2

class KdTreeNode {
private:
	int depth;
	int axisRetries;
	int axis;
	AABB bounds;
	std::vector<Primitive*> objects;
	KdTreeNode* left;
	KdTreeNode* right;

	void build();
	int toggleAxis(){
		return axis == 0 ? 1 : (axis == 1 ? 2 : 0);
	}
	double getSplitValue();


public:
	KdTreeNode() {};
	KdTreeNode(int depth_, int axis_, std::vector<Primitive*>& objects_) :
	depth(depth_), axis(axis_), objects(objects_) {
		axisRetries = 0;
		left = right = NULL;
		build();
	}

	int findNearest(const Ray&, IntersectResult& );
	//int getClosestObjectIntersection(const Ray&, IntersectResult&);
};




/*
#include <vector>
#include "AABB.h"
#include "primitive.h"
#include "config.h"
#include "myObj.h"

//class KdTreeNode
//{
//public:
//
//
//	double posSplit;
//};

// 分类方案
class SplitOpt
{
public:
	SplitOpt(){};
	~SplitOpt(){};

	int childnum[3];
	double posPartition;
};

std::vector<SplitOpt*> splitPool;

class KdTreeNode
{
public:
	KdTreeNode(int _depth, int _newAxis, std::vector<Primitive*>& _vec) : depth(_depth), axis(_newAxis), vec(_vec)
	{
		axisChangeTimes = 0;
		left = right = 0;
		build();
	};
	~KdTreeNode(){};
	
	int depth;
	int axis;
	int axisChangeTimes;
	std::vector<Primitive*> vec;

	
	int changeAxis();	
	KdTreeNode* left;
	KdTreeNode* right;
	
	AABB* bound;
	double posSplit;	//分割点


	void build();
};
*/




#endif

