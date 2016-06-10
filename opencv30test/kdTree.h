#ifndef KDTREE_H
#define KDTREE_H
#include <vector>
#include "myVec3.h"
#include "ray.h"
#include "AABB.h"

const int KD_MAX_THREADS = 6;

class Triangle;
class IntersectResult;

class TriangleBox {
public:
	MyVec3 minPos, maxPos;
	
	TriangleBox();
	~TriangleBox() {}

	void Update(Triangle* tri); // 更新
	bool Cantain(MyVec3 O);
	double CalnArea();
	double Collide(MyVec3 ray_O, MyVec3 ray_V);
};

class TriangleNode {
public:
	Triangle** tris; //每个节点中多个面片
	int size, plane; //
	double split; //分割点
	TriangleBox box; //盒子
	TriangleNode* leftNode; //左节点
	TriangleNode* rightNode;//右节点

	TriangleNode();
	~TriangleNode();
};

class TriangleTree {
	TriangleNode* root;

	void DeleteTree(TriangleNode* node);
	void SortTriangle(Triangle** tris, int l, int r, int coord, bool minCoord);
	void DivideNode(TriangleNode* node);
	IntersectResult TravelTree(TriangleNode* node, MyVec3 ray_O, MyVec3 ray_V);

public:
	TriangleTree();
	~TriangleTree();

	TriangleNode* GetRoot() { return root; }
	void BuildTree();
	IntersectResult Collide(Ray ray);//光线和kdtree相交的结果
	int intersect(Ray ray,IntersectResult& result);

};


#endif

