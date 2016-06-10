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

	void Update(Triangle* tri); // ����
	bool Cantain(MyVec3 O);
	double CalnArea();
	double Collide(MyVec3 ray_O, MyVec3 ray_V);
};

class TriangleNode {
public:
	Triangle** tris; //ÿ���ڵ��ж����Ƭ
	int size, plane; //
	double split; //�ָ��
	TriangleBox box; //����
	TriangleNode* leftNode; //��ڵ�
	TriangleNode* rightNode;//�ҽڵ�

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
	IntersectResult Collide(Ray ray);//���ߺ�kdtree�ཻ�Ľ��
	int intersect(Ray ray,IntersectResult& result);

};


#endif

